//
//  PDProjection.h
//  PlateDetection
//
//  Created by Орест on 11.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PDProjection : NSObject {
}

@property (nonatomic, strong) NSMutableArray *projection;
- (void)rankFilter:(NSInteger)size;

@end
