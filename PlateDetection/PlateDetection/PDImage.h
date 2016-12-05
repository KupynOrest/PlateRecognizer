//
//  PDImage.h
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "Pixel.h"

@interface PDImage : NSObject {
    NSInteger width;
    NSInteger height;
    Pixel** pixels;
}

- (id)initWithImage:(UIImage*)image;
- (UIImage *)getGrayscaleImage;
- (UIImage *)sobolEdgeDetection;
- (UIImage *)horizontalEdgeDetection;
- (UIImage *)verticalEdgeDetection;
- (UIImage *)prewitttEdgeDetection;
- (UIImage *)bandImage;

//Projections
- (NSArray*)getHorizontalProjection;
- (NSArray*)getVerticalProjection;

//test
-(NSArray*)getChars;

@property Pixel**_pixels;

@end
