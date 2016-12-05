//
//  CarPlateRecognizer.cpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#include "CarPlateRecognizer.hpp"
#include "CarSnapshot.hpp"
#include "PeakFinder.hpp"
#include "SnapshotStatistics.hpp"
#include "CharSegment.hpp"
#include <numeric>

CarPlateRecognizer::CarPlateRecognizer(Pixel **pPixels, int w, int h): width(w), height(h) {
    snapshot = new CarSnapshot(pPixels, w, h);
    filter = new Filter();
    stats = new SnapshotStatistics();
}

string CarPlateRecognizer::recognizePlateNumber() {
    CarRect plate = getPlatePosition();
    int w, h;
    Pixel **bitmap = snapshot->applyAdaptiveThresholding(plate, w, h);
    
    //Number position was found, now processing chars
    snapshot = new CarSnapshot(bitmap,w,h);
    
    CarRect newPlate;
    newPlate.x1=newPlate.y1 = 0;
    newPlate.x2 = w - 1;
    newPlate.y2 = h - 1;
    peakFinder->setProjection(snapshot->getPlateHorizontalProjection(newPlate));
    vector<PeakSegment> chars = peakFinder->getPeaks(0.7, 0.86);
    sort(chars.begin(), chars.end(), [=](PeakSegment a, PeakSegment b) {
        return a.peak < b.peak;
    });
    
    int xStart = 0, xEnd = 0;
    for (PeakSegment peak: chars) {
        newPlate.x1 = xStart;
        newPlate.x2 = xEnd = peak.peak;
        
        CharSegment *segment = new CharSegment(xEnd - xStart, h - 1, snapshot->getBitmap(newPlate));
        CarRect charPosition = segment->getCharPosition();
        xStart = xEnd;
    }
    return "";
}

CarRect CarPlateRecognizer::getPlatePosition() {
    vector<CarRect> carPlates;
    vector<int> verticalProjection = snapshot->getVerticalProjection();
    peakFinder = new PeakFinder(filter->rankFilter(9, verticalProjection));
    vector<PeakSegment> bands = peakFinder->getPeaks(0.55);
    for (PeakSegment band : bands) {
        vector<int> horizontalProjection = snapshot->getHorizontalProjection(band.left, band.right);
        peakFinder->setProjection(filter->rankFilter(band.right - band.left,horizontalProjection));
        vector<PeakSegment> plates = peakFinder->getPeaks(0.42);
        for (PeakSegment plate: plates) {
            if (isValid(plate, band) ) {
                CarRect temp;
                temp.x1 = plate.left;
                temp.y1 = band.left;
                temp.x2 = plate.right;
                temp.y2 = band.right;
                temp.vPeak = band.peak;
                carPlates.push_back(temp);
            }
        }
    }
    
    sort(carPlates.begin(), carPlates.end(), [=](CarRect a, CarRect b) {
        double a1 = stats->getPlateValue(b, verticalProjection, verticalProjection[b.vPeak]);
        double a2 = stats->getPlateValue(a, verticalProjection, verticalProjection[a.vPeak]);
        return a1 > a2;
    });
    
    for (CarRect &plate: carPlates) {
        PeakSegment s = peakFinder->getDerivativePeak(snapshot->getPlateHorizontalProjection(plate));
        plate.x1+= s.left;
        plate.x2 = plate.x1 + s.right - s.left;
        
    }
    return carPlates[0];
}

testStruct CarPlateRecognizer::getPlateImage(int &w, int &h) {
    CarRect res = getPlatePosition();
    Pixel **bitmap = snapshot->applyAdaptiveThresholding(res, w, h);
    
    //test
    vector<CarRect> charss;
    
    
    snapshot = new CarSnapshot(bitmap,w,h);
    CarRect newPlate;
    newPlate.x1=newPlate.y1 = 0;
    newPlate.x2 = w - 1;
    newPlate.y2 = h - 1;
    
    

    
    peakFinder->setProjection(snapshot->getPlateHorizontalProjection(newPlate));
    vector<PeakSegment> chars = peakFinder->getPeaks(0.7, 0.86);
    sort(chars.begin(), chars.end(), [=](PeakSegment a, PeakSegment b) {
        return a.peak < b.peak;
    });
    
    int xStart = 0, xEnd = 0;
    for (PeakSegment peak : chars) {
        
        newPlate.x1 = xStart;
        newPlate.x2 = xEnd = peak.peak;
        
        CharSegment *segment = new CharSegment(xEnd - xStart, h - 1, snapshot->getBitmap(newPlate));
        CarRect band = segment->getCharPosition();
        
        band.x1 += xStart;
        band.x2 += xStart;
        charss.push_back(band);
        
        //TODO: Small Neural Network OCR
        
        xStart = xEnd;
    }
    
    testStruct finalResult;
    finalResult.pixels = bitmap;
    finalResult.chars = charss;

    return finalResult;
}

bool CarPlateRecognizer::isValid(PeakSegment plate, PeakSegment band) {
    int rectWidth = plate.right - plate.left;
    int rectHeight = band.right - band.left;
    if (rectWidth < rectHeight) {
        return false;
    } else if (rectWidth/rectHeight <= 2 || rectWidth/rectHeight >= 15) {
        return false;
    } else if (rectHeight >= height/2 || rectHeight <= height/15) {
        return false;
    }
    return true;
}

vector<PeakSegment> CarPlateRecognizer::getBands() {
    vector<int> verticalProjection = snapshot->getVerticalProjection();
    peakFinder = new PeakFinder(verticalProjection);
    return peakFinder->getPeaks(0.55);
}