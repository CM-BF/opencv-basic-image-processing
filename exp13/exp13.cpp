#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>

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

    FILE *fp;
    int x1, x2;
    if((fp = fopen("../input.txt", "r")) == NULL) cout << "Cannot open the input file!"<< endl;
    fscanf(fp, "%d %d\n", &x1, &x2);

    int count[256]={0};
    for(auto it=image.begin<uchar>(); it != image.end<uchar>(); it++)
    {
        count[*it]++;
    }
    int range = x2 - x1;
    int max = 0;
    for(int i=x1; i <= x2; i++)
    {
        if(count[i] > max)
            max = count[i];
    }
    Mat I = Mat::zeros(Size(256, 256), CV_8UC1);

    for(int i=x1; i <= x2; i++)
    {
        int ix = (i - x1) * 256 / range;
        int value = 255 - (count[i] * 256 / max) * 0.9;
        line(I, Point(ix, 255), Point(ix, value), Scalar(255));
    }

    namedWindow("Origin", WINDOW_AUTOSIZE);
    imshow("Origin", image);
    namedWindow("Column", WINDOW_AUTOSIZE);
    imshow("Column", I);
    
    waitKey(0);
    return 0;
}