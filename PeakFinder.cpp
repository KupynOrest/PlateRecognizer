//
//  PeakFinder.cpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#include "PeakFinder.hpp"

#include <iostream>

static const int k_PeaksNumber = 3;
static const double k_PeakDiffMultiplication = 0.2;
static const double k_ConstDeriv = 0.8;
static const int k_DerivativeDiff = 4;
static const int k_MaxChars = 15;

vector<PeakSegment> PeakFinder::getPeaks(double peakFoot) {
    if (!peaks.empty()) {
        peaks.clear();
    }
    for (int i = 0; i < k_PeaksNumber; i++) {
        int peak = (int)distance(projection.begin(), maxElement(projection.begin(), projection.end()));
        peaks.push_back(createPeakSegment(peak, peakFoot));
    }
    return peaks;
}

vector<PeakSegment> PeakFinder::getPeaks(double peakFoot, const double maxPeakIndex) {
    if (!peaks.empty()) {
        peaks.clear();
    }
    for (int i = 0; i < 4; i++) {
        projection[i] = projection[projection.size() - 1 - i] = 0;
    }
    int maxElem = *max_element(projection.begin(), projection.end());
    for (int i = 0; i < k_MaxChars; i++) {
        vector<int>::iterator it = maxElement(projection.begin(), projection.end());
        int peak = (int)distance(projection.begin(), it);
        if (maxElem * maxPeakIndex < *it) peaks.push_back(createPeakSegment(peak, peakFoot));
        else break;
    }
    
    PeakSegment lastPeak;
    lastPeak.peak = (int)projection.size() - 5;
    peaks.push_back(lastPeak);
    return peaks;
}

PeakSegment PeakFinder::getDerivativePeak(vector<int>pProjection) {
    PeakSegment result;
    vector<int>derivativeProj(pProjection.size());
    for (int i = 0; i < pProjection.size(); i++) {
        if (i < k_DerivativeDiff) {
            derivativeProj[i] = (pProjection[i] - pProjection[i + k_DerivativeDiff]) / k_DerivativeDiff;
        } else {
            derivativeProj[i] = (pProjection[i] - pProjection[i - k_DerivativeDiff]) / k_DerivativeDiff;
        }
    }
    
    int maxElement = *max_element(derivativeProj.begin(), derivativeProj.end());
    int minElement = *min_element(derivativeProj.begin(), derivativeProj.end());
    int left = (int)distance(derivativeProj.begin(),find_if(derivativeProj.begin(), derivativeProj.end(), [=](int n) {
        return (n <= minElement * k_ConstDeriv);
    }));
    
    using RevIt = reverse_iterator<vector<int>::iterator>;
    RevIt r_end(derivativeProj.begin());
    RevIt r_begin(derivativeProj.end());

    int right =  (int)distance(derivativeProj.begin(),find_if(r_begin, r_end, [=](int n) {
        return (n >= maxElement * k_ConstDeriv);
    }).base());
    
    if (left < derivativeProj.size()/2 && right > derivativeProj.size()/2) {
        result.left = left;
        result.right = right;
        cout<<"Right peaks!!!! l = "<<left<<", right = "<<right<<endl;
    } else {
        int left = (int)distance(derivativeProj.begin(),find_if(derivativeProj.begin(), derivativeProj.end(), [=](int n) {
            return (n >= maxElement * k_ConstDeriv);
        }));
        
        int right =  (int)distance(derivativeProj.begin(),find_if(r_begin, r_end, [=](int n) {
            return (n <= minElement * k_ConstDeriv);
        }).base());
        if (left < derivativeProj.size()/2 && right > derivativeProj.size()/2) {
            result.left = left;
            result.right = right;
            cout<<"Right peaks!!!! l = "<<left<<", right = "<<right<<endl;
        } else {
            cout<<"Wrong peaks!!!! l = "<<left<<", right = "<<right<<endl;
        }
    }
    return result;
}

PeakSegment PeakFinder::createPeakSegment(int peak, double peakFoot) {
    PeakSegment result;
    result.peak = peak;
    result.left = 0;
    result.right = (int)projection.size() - 1;
    for (int i = peak; i > 0; i--) {
        if (projection[i] <= projection[peak] * peakFoot) {
            result.left = i;
            break;
        }
    }
    
    for (int i = peak; i < projection.size(); i++) {
        if (projection[i] <= projection[peak] * peakFoot) {
            result.right = i;
            break;
        }
    }
    int diff = result.right - result.left;
    result.left -= k_PeakDiffMultiplication * diff;   /*CONSTANT*/
    result.right += k_PeakDiffMultiplication * diff;
    result.left = max(0, result.left);
    int lastPoint = (int)projection.size() - 1;
    result.right = min(lastPoint, result.right);
    return result;
}

vector<int>::iterator PeakFinder::maxElement(vector<int>::iterator first, vector<int>::iterator last) {
    if (first==last) return last;
    vector<int>::iterator largest = first;
    
    while (++first!=last)
        if (shouldCheck((int)(first - projection.begin())) && *largest<*first)
            largest=first;
    return largest;
}

bool PeakFinder::shouldCheck(int index) {
    if (index <= projection.size()/10 || index >= projection.size() - projection.size()/10) {
        return false;
    }
    for (vector<PeakSegment>::iterator it = peaks.begin(); it != peaks.end(); it++) {
        PeakSegment peak = *it;
        if (peak.left <= index && peak.right >= index) {
            return false;
        }
    }
    return true;
}

void PeakFinder::setProjection(vector<int> pProjection) {
    projection = pProjection;
}
