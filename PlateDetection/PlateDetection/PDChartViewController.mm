 //
//  PDChartViewController.m
//  PlateDetection
//
//  Created by Орест on 06.11.15.
//  Copyright © 2015 Lemberg. All rights reserved.
//

#import "PDChartViewController.h"
#import "FSLineChart.h"
#import "UIColor+FSPalette.h"
#import "PDProjection.h"

@implementation PDChartViewController

@synthesize horizontalProjection, verticalProjection, edgeImage;

- (void)viewDidLoad {
    [super viewDidLoad];
    mImageView.image = edgeImage;
    horizontalOne = [PDProjection new];
    horizontalOne.projection = [NSMutableArray arrayWithArray:horizontalProjection];
    
    verticalOne = [PDProjection new];
    verticalOne.projection = [NSMutableArray arrayWithArray:verticalProjection];
    
    [self.view addSubview:[self chart1]];
    [self.view addSubview:[self chart2]];
    [self showImage];
}

- (void)showImage {
    UIImage *img = [UIImage imageNamed:@"main.png"];
    mImage = [[PDImage alloc] initWithImage:img];
    NSArray *plateImages = [mImage getChars];
    mImageView.image = plateImages[0];
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Creating the charts

-(FSLineChart*)chart1 {
    // Generating some dummy data
    
    // Creating the line chart
    FSLineChart* lineChart = [[FSLineChart alloc] initWithFrame:CGRectMake(20, 60, 350, 166)];
    
    lineChart.gridStep = 3;
    
    lineChart.labelForIndex = ^(NSUInteger item) {
        return [NSString stringWithFormat:@"%lu",(unsigned long)item];
    };
    
    lineChart.labelForValue = ^(CGFloat value) {
        return [NSString stringWithFormat:@"%.f", value];
    };
    [horizontalOne rankFilter:9];
    [lineChart setChartData:verticalOne.projection];
    
    return lineChart;
}

-(FSLineChart*)chart2 {
    // Generating some dummy data
    // Creating the line chart
    FSLineChart* lineChart = [[FSLineChart alloc] initWithFrame:CGRectMake(20, 260, 350, 166)];
    
    lineChart.gridStep = 4;
    lineChart.color = [UIColor fsOrange];
    
    lineChart.labelForIndex = ^(NSUInteger item) {
        return [NSString stringWithFormat:@"%lu",(unsigned long)item];
    };
    
    lineChart.labelForValue = ^(CGFloat value) {
        return [NSString stringWithFormat:@"%.f", value];
    };
    [verticalOne rankFilter:9];
    [lineChart setChartData:horizontalOne.projection];
    
    return lineChart;
}

@end