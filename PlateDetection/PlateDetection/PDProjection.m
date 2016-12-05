//
//  PDProjection.m
//  PlateDetection
//
//  Created by Орест on 11.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import "PDProjection.h"

@implementation PDProjection
@synthesize projection;

- (void)rankFilter:(NSInteger)size {
    for (NSInteger i = size/2; i < projection.count - size/2; i++) {
        NSInteger result = 0;
        for (NSInteger j = i - size/2; j < i + size/2; j++) {
            result += [projection[j] integerValue];
        }
        result /= size;
        projection[i] = [NSNumber numberWithInteger:result];
    }
}

@end
