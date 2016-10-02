//
//  CharSegment.hpp
//  PlateDetection
//
//  Created by Орест on 09.03.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef CharSegment_hpp
#define CharSegment_hpp

#include <stdio.h>
#include <vector>
#include "CommonDataTypes.hpp"
class Pixel;

using namespace std;

class Piece: public vector<CarPoint> {
public:
    void setStats();
    CarRect getRect();
    
    friend bool operator<(const Piece& l, const Piece& r)
    {
        return l.cost > r.cost; // keep the same order
    }
private:
    int topLeftX;
    int topRightX;
    int topLeftY;
    int topRightY;
    int cost;
};

class CharSegment {
public:
    CharSegment();
    CharSegment(int w, int h, Pixel**bitmap);
    CarRect getCharPosition();
private:
    Pixel** segmentBitmap;
    int width;
    int height;
    vector<CarPoint> blackPoints;
    vector<Piece> pieces;
    
    void createNewPiece();
    bool validPoint(vector<CarPoint> piece, CarPoint point);
};

#endif /* CharSegment_hpp */
