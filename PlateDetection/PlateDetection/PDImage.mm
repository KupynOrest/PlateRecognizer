//
//  PDImage.m
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import "PDImage.h"
#include <algorithm>
#include "CarPlateRecognizer.hpp"

@implementation PDImage

@synthesize _pixels = pixels;

- (id)initWithImage:(UIImage*)image {
    if (self = [super init]) {
        width = image.size.width;
        height = image.size.height;
        double *bitmap = [self convertUIImageToBitmapRGBA8:image];
        int wCounter = 0;
        int hCounter = 0;
        pixels = new Pixel*[height];
        for(int i = 0; i < height; ++i)
            pixels[i] = new Pixel[width];
        for (int i = 0; i < width * height * 4; i+=4) {
            Pixel *pixel = new Pixel(bitmap[i], bitmap[i+1], bitmap[i+2], bitmap[i+3]);
            
            pixels[hCounter][wCounter++] = *pixel;
            if (wCounter == width) {
                wCounter = 0;
                hCounter++;
            }
        }
    }
    return self;
}

- (UIImage *)getGrayscaleImage {
    [self grayscale];
    return [self convertToImage:pixels];
}

- (UIImage *)sobolEdgeDetection {
    [self grayscale];
    return [self convertToImage:[self applySobolOperator]];
}

- (UIImage *)horizontalEdgeDetection {
    [self grayscale];
    Pixel  **horProjection = [self applyHorizontalOperator];
    return [self convertToImage:horProjection];
}
- (UIImage *)verticalEdgeDetection {
    [self grayscale];
    return [self convertToImage:[self applyVerticalOperator]];
}
- (UIImage *)prewitttEdgeDetection {
    [self grayscale];
    return [self convertToImage:[self applyPrewittOperator]];
}

- (UIImage *)bandImage {
    return [self convertToImage:[self findBands]];
}

- (NSArray*)getChars {
    CarPlateRecognizer *recognizer = new CarPlateRecognizer(pixels, (int)width, (int)height);
    int w, h;
    testStruct res = recognizer->getPlateImage(w, h);
    NSMutableArray *result = [NSMutableArray new];
    [result addObject:[self convertToImage:res.pixels width:w height:h]];
    for (CarRect charSymbol : res.chars) {
        //[result addObject:[self convertToImage:res.pixels width:w height:h rect:charSymbol]];
    }
    return [NSArray arrayWithArray:result];
    return nil;
}

- (Pixel**) findBands {
    Pixel **result = new Pixel*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new Pixel[width];
    
    CarPlateRecognizer *recognizer = new CarPlateRecognizer(pixels, (int)width, (int)height);
    vector<PeakSegment> bands = recognizer->getBands();
    CarRect band = recognizer->getPlatePosition();
    BOOL isRed = NO;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            isRed = 0;
            if ((i == band.y1 || i == band.y2) && (j >= band.x1 && j <= band.x2)) {
                isRed = YES;
            } else if ((j == band.x1 || j == band.x2) && (i >= band.y1 && i <= band.y2)) {
                isRed = YES;
            }
            if (isRed) {
                result[i][j].red = 255;
                result[i][j].green = 0;
                result[i][j].blue = 0;
                result[i][j].alpha = 0;
            } else {
                result[i][j] = pixels[i][j];
            }
        }
    }
    return result;
}

- (UIImage*)convertToImage:(Pixel **)pPixels width:(int)w height:(int)h {
    unsigned char *bitmapData = (unsigned char *)malloc(sizeof(unsigned char) * w * 4 * h);
    int bitmapCounter = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel pixel = pPixels[i][j];
            bitmapData[bitmapCounter++] = pixel.red;
            bitmapData[bitmapCounter++] = pixel.green;
            bitmapData[bitmapCounter++] = pixel.blue;
            bitmapData[bitmapCounter++] = pixel.alpha;
        }
    }
    return [self convertBitmapRGBA8ToUIImage:bitmapData
                                   withWidth:w
                                  withHeight:h];
}

- (UIImage*)convertToImage:(Pixel **)pPixels width:(int)w height:(int)h rect:(CarRect)band {
    unsigned char *bitmapData = (unsigned char *)malloc(sizeof(unsigned char) * w * 4 * h);
    int bitmapCounter = 0;
    bool isOur = false;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            isOur = false;
            if ((i >= band.y1 && i <= band.y2) && (j >= band.x1 && j <= band.x2)) {
                isOur = true;
            }
            
            if (isOur) {
                Pixel pixel = pPixels[i][j];
                bitmapData[bitmapCounter++] = pixel.red;
                bitmapData[bitmapCounter++] = pixel.green;
                bitmapData[bitmapCounter++] = pixel.blue;
                bitmapData[bitmapCounter++] = pixel.alpha;
            }
        }
    }
    return [self convertBitmapRGBA8ToUIImage:bitmapData
                                   withWidth:band.x2 - band.x1 + 1
                                  withHeight:band.y2 - band.y1 + 1];
}

- (UIImage*)convertToImage:(Pixel **)pPixels {
    unsigned char *bitmapData = (unsigned char *)malloc(sizeof(unsigned char) * width * 4 * height);
    int bitmapCounter = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel pixel = pPixels[i][j];
            bitmapData[bitmapCounter++] = pixel.red;
            bitmapData[bitmapCounter++] = pixel.green;
            bitmapData[bitmapCounter++] = pixel.blue;
            bitmapData[bitmapCounter++] = pixel.alpha;
        }
    }
    return [self convertBitmapRGBA8ToUIImage:bitmapData
                                   withWidth:(int)width
                                  withHeight:(int)height];
}

- (NSArray*)getHorizontalProjection {
    UIImage *img = [[self getChars] firstObject];
    Pixel** horProjection = [self pixelsFromImage:[[self getChars] firstObject]];
    return [self getHorizontalProjection:horProjection w:img.size.width h:img.size.height];}

- (NSArray*)getVerticalProjection {
    UIImage *img = [[self getChars] firstObject];
    Pixel** verProjection = [self pixelsFromImage:[[self getChars] firstObject]];
    return [self getVerticalProjection:verProjection w:img.size.width h:img.size.height];
}

- (void)grayscale {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixels[i][j].grayscale();
        }
    }
}

- (Pixel **)pixelsFromImage:(UIImage *)img {
    Pixel **result;
    int lWidth = img.size.width;
    int lHeight = img.size.height;
    double *bitmap = [self convertUIImageToBitmapRGBA8:img];
    int wCounter = 0;
    int hCounter = 0;
    result = new Pixel*[lWidth];
    for(int i = 0; i < lHeight; ++i)
        result[i] = new Pixel[lWidth];
    for (int i = 0; i < lWidth * lHeight * 4; i+=4) {
        Pixel *pixel = new Pixel(bitmap[i], bitmap[i+1], bitmap[i+2], bitmap[i+3]);
        
        result[hCounter][wCounter++] = *pixel;
        if (wCounter == lWidth) {
            wCounter = 0;
            hCounter++;
        }
    }
    return result;
}

- (NSArray *)getHorizontalProjection:(Pixel **)pPixels w:(int)w h:(int)h {
    NSMutableArray *result = [NSMutableArray new];
    NSInteger value = 0;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            Pixel pixel = pPixels[j][i];
            value += pixel.red;
        }
        [result addObject:[NSNumber numberWithInteger:value]];
        value = 0;
    }
    return [NSArray arrayWithArray:result];
}

- (NSArray *)getVerticalProjection:(Pixel **)pPixels w:(int)w h:(int)h {
    NSMutableArray *result = [NSMutableArray new];
    NSInteger value = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel pixel = pPixels[i][j];
            value += pixel.red;
        }
        [result addObject:[NSNumber numberWithInteger:value]];
        value = 0;
    }
    return [NSArray arrayWithArray:result];
}
- (Pixel **)applyHorizontalOperator {
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

- (Pixel **)applyVerticalOperator {
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

- (Pixel **)applyPrewittOperator {
    Pixel **result = new Pixel*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new Pixel[width];
    int horizFilter[3][3] = {{ -1,   -1,  -1},
        { 0,   0,  0},
        { 1,   1,  1}};
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
                        horizPixVal += pixels[i+x][j+y].red * horizFilter[x + 1][y + 1];       //Only need to focus on one of the RGB values since the output is
                        vertPixVal  += pixels[i+x][j+y].red * vertFilter[x + 1][y + 1];
                        
                    }
                }
                pixVal = abs(horizPixVal) + abs(vertPixVal);     //Calculate magnitude
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

- (Pixel **)applySobolOperator {
    Pixel **result = new Pixel*[height];
    for(int i = 0; i < height; ++i)
        result[i] = new Pixel[width];
    int horizFilter[3][3] = {{ -1,   -2,  -1},
        { 0,   0,  0},
        { 1,   2,  1}};
    int vertFilter[3][3]  = {{ -1,   0,   1},
        { -2,   0,   2},
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
                        horizPixVal += pixels[i+x][j+y].red * horizFilter[x + 1][y + 1];       //Only need to focus on one of the RGB values since the output is
                        vertPixVal  += pixels[i+x][j+y].red * vertFilter[x + 1][y + 1];
                        
                    }
                }
                pixVal = abs(horizPixVal) + abs(vertPixVal);     //Calculate magnitude
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

- (double *) convertUIImageToBitmapRGBA8:(UIImage *) image {
    CGImageRef imageRef = image.CGImage;
    
    // Create a bitmap context to draw the uiimage into
    CGContextRef context = [self newBitmapRGBA8ContextFromImage:imageRef];
    
    if(!context) {
        return NULL;
    }
    
    size_t lWidth = CGImageGetWidth(imageRef);
    size_t lHeight = CGImageGetHeight(imageRef);
    
    CGRect rect = CGRectMake(0, 0, lWidth, lHeight);
    
    // Draw image into the context to get the raw image data
    CGContextDrawImage(context, rect, imageRef);
    
    // Get a pointer to the data
    CFDataRef pixelData = CGDataProviderCopyData(CGImageGetDataProvider(image.CGImage));
    const UInt8* data = CFDataGetBytePtr(pixelData);
    double *bitmapData = (double *)malloc(sizeof(double) * lWidth * 4 * lHeight);
    for (int i = 0; i < lWidth * 4 * lHeight; i++) {
        bitmapData[i] = (double)data[i];
    }
    
    CGContextRelease(context);
    
    return bitmapData;
}

- (CGContextRef) newBitmapRGBA8ContextFromImage:(CGImageRef) image {
    CGContextRef context = NULL;
    CGColorSpaceRef colorSpace;
    uint32_t *bitmapData;
    
    size_t bitsPerPixel = 32;
    size_t bitsPerComponent = 8;
    size_t bytesPerPixel = bitsPerPixel / bitsPerComponent;
    
    size_t lWidth = CGImageGetWidth(image);
    size_t lHeight = CGImageGetHeight(image);
    
    size_t bytesPerRow = lWidth * bytesPerPixel;
    size_t bufferLength = bytesPerRow * lHeight;
    
    colorSpace = CGColorSpaceCreateDeviceRGB();
    
    if(!colorSpace) {
        NSLog(@"Error allocating color space RGB\n");
        return NULL;
    }
    
    // Allocate memory for image data
    bitmapData = (uint32_t *)malloc(bufferLength);
    
    if(!bitmapData) {
        NSLog(@"Error allocating memory for bitmap\n");
        CGColorSpaceRelease(colorSpace);
        return NULL;
    }
    
    //Create bitmap context
    context = CGBitmapContextCreate(bitmapData,
                                    lWidth,
                                    lHeight,
                                    bitsPerComponent,
                                    bytesPerRow,
                                    colorSpace,
                                    kCGImageAlphaPremultipliedLast);	// RGBA
    
    if(!context) {
        free(bitmapData);
        NSLog(@"Bitmap context not created");
    }
    
    CGColorSpaceRelease(colorSpace);
    
    return context;	
}

- (UIImage *) convertBitmapRGBA8ToUIImage:(unsigned char *) rawData
                                withWidth:(int) pWidth
                               withHeight:(int) pHeight {
    
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL,
                                                              rawData,
                                                              pWidth*pHeight*4,
                                                              NULL);
    
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef imageRef = CGImageCreate(pWidth,
                                        pHeight,
                                        8,
                                        32,
                                        4*pWidth,colorSpaceRef,
                                        bitmapInfo,
                                        provider,NULL,NO,renderingIntent);
    /*I get the current dimensions displayed here */
    NSLog(@"width=%zu, height: %zu", CGImageGetWidth(imageRef),
          CGImageGetHeight(imageRef) );
    UIImage *newImage = [UIImage imageWithCGImage:imageRef];
    /*This is where the problem lies.
     The width, height displayed are of completely different dimensions
     viz. the width is always zero and the height is a very huge number */
    return newImage;
}



@end
