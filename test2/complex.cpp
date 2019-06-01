#include<iostream>
#include<complex>
#include<cmath>
#include<iomanip>
// #include<opencv2/opencv.hpp>

// using namespace cv;
using namespace std;
using namespace complex_literals;

int main(int argc, char ** argv)
{
    complex<double> i = 1i;
    complex<double> j = 1. - 1i;
    // complex<double> result = j * 8i;
    // Mat mat = Mat::ones(128, 128, CV_64FC2);
    //mat.at<Vec2d>(1, 1)[0] *
    cout << j * 8i  << endl;
    return 0;
}
