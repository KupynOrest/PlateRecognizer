//
//  PDImageAnalysys.cpp
//  PlateDetection
//
//  Created by Орест on 10.02.16.
//  Copyright © 2016 Lemberg. All rights reserved.
//

#include "CarSnapshot.hpp"
#include <math.h>
#include <stdlib.h>
#include <numeric>

CarSnapshot::CarSnapshot(Pixel **pPixels, int w, int h): pixels(pPixels), width(w), height(h) {
    grayScale();
}

void CarSnapshot::grayScale() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixels[i][j].grayscale();
        }
    }
}

Pixel** CarSnapshot::applyAdaptiveThresholding(CarRect plate, int &w, int &h) {
    
    h = plate.y2 - plate.y1;
    w = plate.x2 - plate.x1;
    
    int size = w * h;
    int S = w >> 3;
    int S2 = S >> 1;
    float T = 0.15f;
    float IT = 1.0f - T;
    
    int* integral = new int[size];
    int* threshold = new int[size];
    int* data = new int[size];
    
    Pixel **plateBitmap = getBitmap(plate);
    
    //Note: copying source image(im) to a vector to use the same code from Eugene
    int index = 0;
    for (int l = 0; l < h; l++)
        for (int c = 0; c < w; c++)
            data[index++] = plateBitmap[l][c].red;
    
    
    int i, j, diff;
    int x1 = 0, y1, x2, y2, ind1, ind2, ind3;
    int sum = 0, ind = 0;
    
    while( ind < size )
    {
        sum += data[ind];
        integral[ind] = sum;
        ind += w;
    }
    
    for( i = 1; i < w; ++i )
    {
        sum = 0;
        ind = i;
        ind3 = ind - S2;
        
        if( i > S ) x1 = i - S;
        diff = i - x1;
        for( j = 0; j < h; ++j )
        {
            sum += data[ind] & 0xFF;
            integral[ind] = integral[int(ind-1)] + sum;
            ind += w;
            
            if(i < S2) continue;
            if(j < S2) continue;
            
            y1 = (j < S ? 0 : j - S);
            
            ind1 = y1 * w;
            ind2 = j * w;
            
            if (((data[ind3])*(diff * (j - y1))) < ((integral[int(ind2 + i)] - integral[int(ind1 + i)] - integral[int(ind2 + x1)] + integral[int(ind1 + x1)])*IT))
            {
                threshold[ind3] = 0;
            } else {
                threshold[ind3] = 255;
            }
            ind3 += w;
        }
    }
    
    y1 = 0;
    for( j = 0; j < h; ++j )
    {
        i = 0;
        y2 = h - 1;
        if( j < h - S2 )
        {
            i = w - S2;
            y2 = j + S2;
        }
        
        ind = j * w + i;
        if( j > S2 ) y1 = j - S2;
        ind1 = y1 * w;
        ind2 = y2 * w;
        diff = y2 - y1;
        for( ; i < w; ++i, ++ind )
        {
            
            x1 = ( i < S2 ? 0 : i - S2);
            x2 = i + S2;
            
            // check the border
            if (x2 >= w) x2 = w - 1;
            
            if (((data[ind])*((x2 - x1) * diff)) < ((integral[int(ind2 + x2)] - integral[int(ind1 + x2)] - integral[int(ind2 + x1)] + integral[int(ind1 + x1)])*IT))
            {
                threshold[ind] = 0;
            } else {
                threshold[ind] = 255;
            }
        }
    }
    index = 0;
    for (int l = 0; l < h; l++) {
        for (int c = 0; c < w; c++) {
            plateBitmap[l][c].red = plateBitmap[l][c].green = plateBitmap[l][c].blue = threshold[index++];
        }
    }
    
    return plateBitmap;
}

vector<int> CarSnapshot::getPlateHorizontalProjection(CarRect plate) {
    vector<int> result;
    int h = plate.y2 - plate.y1;
    int w = plate.x2 - plate.x1;
    Pixel **plateBitmap = getBitmap(plate);
    
    int value = 0;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
                Pixel pixel = plateBitmap[j][i];
                value += pixel.red;
            }
        result.push_back(value);
        value = 0;
    }
    return result;
}

Pixel** CarSnapshot::getBitmap(CarRect plate) {
    int h = plate.y2 - plate.y1;
    int w = plate.x2 - plate.x1;
    Pixel **plateBitmap = new Pixel*[h];
    for(int i = 0; i < h; ++i)
        plateBitmap[i] = new Pixel[w];
    
    for (int i = plate.y1; i < plate.y2; i++) {
        for (int j = plate.x1; j < plate.x2; j++) {
            plateBitmap[i - plate.y1][j - plate.x1] = pixels[i][j];
        }
    }
    return plateBitmap;
}

vector<int> CarSnapshot::getHorizontalProjection( int start, int end) {
    if (start < 0 && end < 0) {
        start = 0;
        end = height - 1;
    }
    Pixel** imageBuffer = applyVerticalEdgeDetection();
    vector<int> result;
    int value = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (j >= start && j <= end) {
                Pixel pixel = imageBuffer[j][i];
                value += pixel.red;
            }
        }
        result.push_back(value);
        value = 0;
    }
    return result;
}

vector<int> CarSnapshot::getVerticalProjection(int start, int end) {
    if (start < 0 && end < 0) {
        start = 0;
        end = width - 1;
    }
    Pixel** imageBuffer = applyVerticalEdgeDetection();
    vector<int> result;
    int value = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j >= start && j <= end) {
                Pixel pixel = imageBuffer[i][j];
                value += pixel.red;
            }
        }
        result.push_back(value);
        value = 0;
    }
    return result;
}

Pixel** CarSnapshot::applyHorizontalEdgeDetection() {
    Pixel **result = new Pixel*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new Pixel[width];
    int horizFilter[3][3] = {{ -1,   -1,  -1},
        { 0,   0,  0},
        { 1,   1,  1}};
    int pixVal = 0;
    int horizPixVal = 0;
    int vertPixVal = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //NSLog(@"i  %i, j = %i", i , j);
            if ((i==0)||(i==height-1)||(j==0)||(j==width-1)) {
                result[i][j].red = result[i][j].green = result[i][j].blue = 0;
            }
            
            else
            {
                pixVal = 0;
                horizPixVal = 0;
                vertPixVal = 0;
                //calculating the X and Y convolutions
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        horizPixVal += pixels[i+x][j+y].red * horizFilter[x + 1][y + 1];       //Only need to focus on one of the RGB values since the output is
                        
                    }
                }
                pixVal = abs(horizPixVal);     //Calculate magnitude
                if (pixVal < 0) {
                    pixVal = 0;
                }
                if(pixVal > 255) {
                    pixVal = 255;
                }
                result[i][j].red = result[i][j].green = result[i][j].blue = pixVal;
            }
        }
    }
    return result;
}

Pixel** CarSnapshot::applyVerticalEdgeDetection() {
    Pixel **result = new Pixel*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new Pixel[width];
    int vertFilter[3][3]  = {{ -1,   0,   1},
        { -1,   0,   1},
        {-1,  0,  1}};
    int pixVal = 0;
    int horizPixVal = 0;
    int vertPixVal = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //NSLog(@"i  %i, j = %i", i , j);
            if ((i==0)||(i==height-1)||(j==0)||(j==width-1)) {
                result[i][j].red = result[i][j].green = result[i][j].blue = 0;
            }
            
            else
            {
                pixVal = 0;
                horizPixVal = 0;
                vertPixVal = 0;
                //calculating the X and Y convolutions
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        vertPixVal  += pixels[i+x][j+y].red * vertFilter[x + 1][y + 1];
                        
                    }
                }
                pixVal = abs(vertPixVal);     //Calculate magnitude
                if (pixVal < 0) {
                    pixVal = 0;
                }
                if(pixVal > 255) {
                    pixVal = 255;
                }
                result[i][j].red = result[i][j].green = result[i][j].blue = pixVal;
            }
        }
    }
    return result;

}

vector<int> Filter::halfFilter(vector<int> projection) {
    int sum = accumulate(projection.begin(), projection.end(), 0);
    sum/= projection.size();
    int maxElem = *max_element(projection.begin(), projection.end());
    vector<int> result(projection.size());
    copy(projection.begin(), projection.end(), result.begin());
    for (int i = 0; i < projection.size(); i++) {
        result[i] = result[i] - (2*sum - maxElem);
    }
    return result;
}

vector<int> Filter::rankFilter(int size, vector<int>projection) {
    for (int i = size/2; i < projection.size() - size/2; i++) {
        int result = 0;
        for (int j = i - size/2; j < i + size/2; j++) {
            result += projection[j];
        }
        result /= size;
        projection[i] = result;
    }
    return projection;
}