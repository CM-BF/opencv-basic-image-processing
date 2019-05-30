#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);

    if(image.empty()){
        printf("Could not open the image!\n");
        return 4;
    }

    //cout<< "image:" << endl << format(image, Formatter::FMT_NUMPY) << endl;
    float k, b;
    FILE *fp;
    if((fp = fopen("../input.txt", "r")) == NULL) cout << "Cannot open the input file!" <<endl;
    fscanf(fp, "%f %f\n", &k, &b);

    Mat I=image.clone();
    for(auto it=I.begin<uchar>(); it != I.end<uchar>(); it++)
    {
        float tmp = k*(*it) + b;
        *it = int(tmp > 255? 255 : (tmp < 0? 0 : tmp));
    }
    namedWindow("Origin", WINDOW_AUTOSIZE);
    imshow("Origin", image);
    namedWindow("Linear transformer", WINDOW_AUTOSIZE);
    imshow("Linear transformer", I);
    waitKey(0);
    return 0;
}