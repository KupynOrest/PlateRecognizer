//
//  SnapshotStatistics.cpp
//  PlateDetection
//
//  Created by Орест on 11.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#include "SnapshotStatistics.hpp"
#include <math.h>
#include <iostream>
#include <limits>

double SnapshotStatistics::getPlateValue(CarRect plate, vector<int> projection, int peak) {
    //α =0.15⋅α1 +0.25⋅α2 +0.4⋅α3 +0.4⋅α4
    double alpha1 = plate.y2 - plate.y1;
    double alpha2 = 1/peak;
    double alpha4 = fabs((double)((plate.x2 - plate.x1) / (plate.y2 - plate.y1) - 5));
    double value = 0;
    for (int i = plate.y1; i < plate.y2; i++) {
        value += projection[i];
    }
    
    double alpha3 = 1/value;
    double result = 0.15*alpha1 + 0.25*alpha2 + 0.4*alpha3 + 0.25*alpha4;
    cout<<"Result is - "<<result<<", Plate -"<<plate.x1<<", "<<plate.x2<<": y: -"<<plate.y1<<", "<<plate.y2<<endl;
    return result;
}