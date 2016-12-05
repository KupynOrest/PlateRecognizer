//
//  ViewController.m
//  PlateDetection
//
//  Created by Орест on 04.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import "ViewController.h"
#import "PDImage.h"
#import "PDChartViewController.h"
#include "CarPlateRecognizer.hpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    UIImage *testImage = [UIImage imageNamed:@"main.png"];
    rawImage.image = testImage;
    mImage = [[PDImage alloc] initWithImage:testImage];
    testImage = [mImage getGrayscaleImage];
    processedImage.image = testImage;
    testImage = [mImage verticalEdgeDetection];
    edgeDetection.image = testImage;
    
    CarPlateRecognizer *recognizer = new CarPlateRecognizer(mImage._pixels, testImage.size.width, testImage.size.height);
    //UIImage *test = [mImage plateImage];
    //recognizer->recognizePlateNumber();

    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get reference to the destination view controller
    PDChartViewController *vc = [segue destinationViewController];
    
    // Pass any objects to the view controller here, like...
    vc.horizontalProjection = [mImage getHorizontalProjection];
    vc.verticalProjection = [mImage getVerticalProjection];
}



@end
