//
//  SnapshotStatistics.hpp
//  PlateDetection
//
//  Created by Орест on 11.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef SnapshotStatistics_hpp
#define SnapshotStatistics_hpp

#include <stdio.h>
#include <vector>
#include "CommonDataTypes.hpp"
class Pixel;

using namespace std;

class SnapshotStatistics {
public:
    double getPlateValue(CarRect plate, vector<int>projection, int peak);
};

#endif /* SnapshotStatistics_hpp */
