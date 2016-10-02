//
//  PeakFinder.hpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#ifndef PeakFinder_hpp
#define PeakFinder_hpp

#include <stdio.h>
#include <vector>
#include "CommonDataTypes.hpp"

using namespace std;

class PeakFinder {
public:
    PeakFinder(vector<int> pProjection): projection(pProjection) {};
    vector<PeakSegment> getPeaks(double peakFoot);
    vector<PeakSegment> getPeaks(double peakFoot, const double maxPeakIndex);
    PeakSegment getDerivativePeak(vector<int>pProjection);
    void setProjection(vector<int> pProjection);
private:
    vector<int>projection;
    vector<PeakSegment> peaks;
    
    PeakSegment createPeakSegment(int peak, double peakFoot);
    vector<int>::iterator maxElement(vector<int>::iterator first, vector<int>::iterator last);
    bool shouldCheck(int index);
};

#endif /* PeakFinder_hpp */
