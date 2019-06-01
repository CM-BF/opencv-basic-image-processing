#include<iostream>
#include<complex>
#include<cmath>
#include<iomanip>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace complex_literals;

int main(int argc, char ** argv)
{
    complex<double> i = 1. + 1i;
    complex<double> j = 1. - 1i;
    complex<double> result = i * 8.;
    Mat mat = Mat::ones(128, 128, CV_64FC2);
    auto a = mat.at<Vec2d>(1, 1)[0];
    cout << a << endl;
    return 0;
}