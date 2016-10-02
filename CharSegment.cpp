//
//  CharSegment.cpp
//  PlateDetection
//
//  Created by Орест on 09.03.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#include "CharSegment.hpp"
#include "Pixel.h"
#include <iostream>
#include <stack>
#include "SnapshotStatistics.hpp"

CharSegment::CharSegment(): width(0), height(0), segmentBitmap(NULL) {};

CharSegment::CharSegment(int w, int h, Pixel**bitmap): width(w), height(h), segmentBitmap(bitmap) {};

CarRect CharSegment::getCharPosition() {
    CarRect result;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (segmentBitmap[i][j].red == 0) {
                //add to the list of black points;
                CarPoint newPoint(j ,i);
                blackPoints.push_back(newPoint);
            }
        }
    }
    
    while (!blackPoints.empty()) {
        createNewPiece();
    }
    
    sort(pieces.begin(), pieces.end());
    
    return pieces[0].getRect();
}

void CharSegment::createNewPiece() {
    stack<CarPoint> tempStack;
    Piece piece;
    
    tempStack.push(blackPoints.back());
    
    while(!tempStack.empty()) {
        CarPoint point = tempStack.top();
        tempStack.pop();
        if (validPoint(piece, point)) {
            piece.push_back(point);
            blackPoints.erase(std::remove(blackPoints.begin(), blackPoints.end(), point), blackPoints.end());
            tempStack.push(CarPoint(point.x + 1, point.y));
            tempStack.push(CarPoint(point.x - 1, point.y));
            tempStack.push(CarPoint(point.x, point.y + 1));
            tempStack.push(CarPoint(point.x, point.y - 1));
        }
    }
    pieces.push_back(piece);
    piece.setStats();
}

bool CharSegment::validPoint(vector<CarPoint>piece, CarPoint point) {
    if (point.x >= width || point.y >= height || point.x < 0 || point.y < 0) {
        return false;
    }
    if (segmentBitmap[point.y][point.x].red != 0) {
        return false;
    }
    if (find(piece.begin(), piece.end(), point) != piece.end()) {
        return false;
    }
    return true;
}


void Piece::setStats() {
    topLeftX = std::numeric_limits<int>::max(), topLeftY = std::numeric_limits<int>::max(), topRightX = 0, topRightY = 0;
    for (CarPoint point: *this) {
        topLeftX = min(point.x, topLeftX);
        topLeftY = min(point.y, topLeftY);
        topRightX = max(point.x, topRightX);
        topRightY = max(point.y, topRightY);
    }
    int width = topRightX - topLeftX + 1;
    int height = topRightY - topLeftY + 1;
    cost = (int)((width * height) - this->size());
}

CarRect Piece::getRect() {
    CarRect result;
    result.x1 = topLeftX;
    result.x2 = topRightX;
    result.y1 = topLeftY;
    result.y2 = topRightY;
    return result;
}