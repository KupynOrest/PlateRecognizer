//
//  ViewController.h
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import <UIKit/UIKit.h>
@class PDImage;
@interface ViewController : UIViewController {
    IBOutlet UIImageView *rawImage;
    IBOutlet UIImageView *processedImage;
    IBOutlet UIImageView *edgeDetection;
    
    PDImage *mImage;
}

@end

