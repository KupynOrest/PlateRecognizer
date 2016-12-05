//
//  PDPixel.m
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#include "Pixel.h"

Pixel::Pixel(int r, int g, int b, int a):red(r), green(g), blue(b), alpha(a){}

Pixel::Pixel():red(255), green(255), blue(255), alpha(1) {};

void Pixel::grayscale() {
    red = green = blue = getGrayValue();
}

int Pixel::getGrayValue() {
    return (red + green + blue) / 3;
}
