//
//  PDImageAnalysys.hpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef PDImageAnalysys_hpp
#define PDImageAnalysys_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include "Pixel.h"
#include "CommonDataTypes.hpp"
using namespace std;

class CarSnapshot {
public:
    CarSnapshot(): pixels(NULL), width(0), height(0) {};
    CarSnapshot(Pixel **pPixels, int w, int h);
    vector<int>getPlateHorizontalProjection(CarRect plate);
    Pixel** applyAdaptiveThresholding(CarRect plate, int &w, int&h);
    vector<int>getHorizontalProjection(int begin = -1, int end = -1);
    vector<int>getVerticalProjection(int begin = -1, int end = -1);
    Pixel** getBitmap(CarRect plate);
private:
    Pixel **pixels;
    int width;
    int height;
    void grayScale();
    Pixel** applyVerticalEdgeDetection();
    Pixel** applyHorizontalEdgeDetection();
};


class Filter {
public:
    vector<int> halfFilter(vector<int>projection);
    vector<int> rankFilter(int size, vector<int>projection);
};
#endif /* PDImageAnalysys_hpp */
