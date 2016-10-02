//
//  PDPixel.h
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

class Pixel {
public:
    int red;
    int green;
    int blue;
    int alpha;
    void grayscale();
    Pixel(int r, int g, int b, int a);
    Pixel();
private:
    int getGrayValue();
};
