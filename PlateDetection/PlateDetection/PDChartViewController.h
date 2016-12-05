//
//  PDChartViewController.h
//  PlateDetection
//
//  Created by Орест on 06.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PDImage.h"
@class PDProjection;
@interface PDChartViewController : UIViewController {
    IBOutlet UIImageView *mImageView;
    PDProjection *verticalOne;
    PDProjection *horizontalOne;
    PDImage *mImage;
    int counter;
}

@property (nonatomic, strong) NSArray *horizontalProjection;
@property (nonatomic, strong) NSArray *verticalProjection;
@property (nonatomic, strong) UIImage *edgeImage;

@end
