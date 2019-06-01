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
    float x1, y1, x2, y2;
    if((fp = fopen("../input.txt", "r")) == NULL) cout << "Cannot open the input file!"<< endl;
    fscanf(fp, "%f %f %f %f\n", &x1, &y1, &x2, &y2);

    Mat I = image.clone();
    for(auto it=I.begin<uchar>(); it != I.end<uchar>(); it++)
    {
        if(*it <= x1){
            *it = y1 * (*it) / x1;
            continue;
        }
        if(*it > x1 && *it < x2){
            *it = (y2 - y1)/(x2 - x1) * (*it - x1) + y1;
            continue;
        }
        if(*it >= x2){
            *it = (255 - y2)/(255 - x2) * (*it - x2) + y2;
            continue;
        }
    }

    namedWindow("Origin", WINDOW_AUTOSIZE);
    imshow("Origin", image);
    namedWindow("Strenth", WINDOW_AUTOSIZE);
    imshow("Strenth", I);
    
    waitKey(0);
    return 0;
}