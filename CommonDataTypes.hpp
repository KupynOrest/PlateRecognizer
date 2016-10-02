//
//  CommonDataTypes.hpp
//  PlateDetection
//
//  Created by Орест on 11.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef CommonDataTypes_hpp
#define CommonDataTypes_hpp

#include <stdio.h>

using namespace std;

//Test func
class Pixel;
#include <vector>

struct CarPoint {
    int x;
    int y;
    CarPoint(int pX, int pY): x(pX), y(pY) {};
    
    friend bool operator==(const CarPoint& lhs, const CarPoint& rhs){
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct PeakSegment {
    int left;
    int peak;
    int right;
};

struct CarRect {
    int x1;
    int x2;
    int y1;
    int y2;
    int vPeak;
};


struct testStruct {
    Pixel **pixels;
    vector<CarRect> chars;
};

#endif /* CommonDataTypes_hpp */
