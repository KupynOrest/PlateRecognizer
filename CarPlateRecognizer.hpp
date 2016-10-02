//
//  CarPlateRecognizer.hpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef CarPlateRecognizer_hpp
#define CarPlateRecognizer_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "CommonDataTypes.hpp"
class Pixel;
class CarSnapshot;
class Filter;
class PeakFinder;
class SnapshotStatistics;

using namespace std;

class CarPlateRecognizer {
public:
    CarPlateRecognizer(Pixel **pixels, int w, int h);
    CarRect getPlatePosition();
    string recognizePlateNumber();
    
    //temp
    vector<PeakSegment> getBands();
    testStruct getPlateImage(int&w, int&h);
private:
    CarPlateRecognizer() {}
    PeakFinder *peakFinder;
    CarSnapshot *snapshot;
    Filter *filter;
    SnapshotStatistics *stats;
    
    int width;
    int height;
    
    bool isValid(PeakSegment plate, PeakSegment band);
};

#endif /* CarPlateRecognizer_hpp */
