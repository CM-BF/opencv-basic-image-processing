#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>
#include"Column/Column.h"

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat image = imread("../../data/pout.bmp", IMREAD_GRAYSCALE);

    if(image.empty())
    {
        cout << "Cannot open the image!" << endl;
        return 4;
    }

    float count[256]={0}, n=0;
    for(auto it=image.begin<uchar>(); it != image.end<uchar>(); it++)
    {
        count[*it] += 1;
        n += 1;
    }
    
    int trans[256] = {0};
    
    count[0] = count[0]/n;
    trans[0] = int(count[0]*255 + 0.5);
    for(int i=1; i<256; i++){
        count[i] = count[i-1] + count[i]/n; 
        trans[i] = int(count[i]*255 + 0.5);
    }

    // Lookup table trans creating complited
    Mat image_ = image.clone();
    for(auto it = image_.begin<uchar>(); it != image_.end<uchar>(); it++){
        *it = trans[*it];
    }
    
    Mat I = Column(image, 0, 255);
    Mat I_ = Column(image_, 0, 255);

    namedWindow("Origin", WINDOW_AUTOSIZE);
    imshow("Origin", image);
    namedWindow("OriginColumn", WINDOW_AUTOSIZE);
    imshow("OriginColumn", I);

    namedWindow("New", WINDOW_AUTOSIZE);
    imshow("New", image_);
    namedWindow("NewColumn", WINDOW_AUTOSIZE);
    imshow("NewColumn", I_);
    
    waitKey(0);
    return 0;
}