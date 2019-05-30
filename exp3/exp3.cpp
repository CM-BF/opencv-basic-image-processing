#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<vector>
#include<algorithm>

#define N 3

using namespace std;
using namespace cv;

Mat stretch(Mat &image)
{
    int min = 10000, max = 0;
    for(auto it=image.begin<short>(); it!=image.end<short>(); it++){
        if(*it < min){
            min = *it;
        }
        if(*it > max){
            max = *it;
        }
    }
    double k = 255.0 / (max - min);
    double b = -k * min;
    for(auto it=image.begin<short>(); it != image.end<short>(); it++){
        *it = (short)(k * (*it) + b);
    }

    Mat result;
    image.convertTo(result, CV_8UC1);
    return result;
}

Mat mask(const Mat &image, int kernel[][N])
{
    Mat result;
    result.create(image.size(), CV_16SC1);

    int nrows = image.rows;
    int ncols = image.cols;

    int w = (N - 1) / 2;
    

    for(int i=1; i<nrows-1; i++){
        for(int j=1; j<ncols-1; j++){
            int sum = 0;
            for(int x=i - w; x<=i + w; x++){
                for(int y=j - w; y<=j + w; y++){
                    sum += kernel[x - i + w][y - j + w] * image.at<uchar>(x, y);
                }
            }
            result.at<short>(i, j) = (short)(sum);
        }
    }
    
    
    return result;
}

int main(int argc, char ** argv)
{
    Mat image = imread("../../data/blood1.bmp", IMREAD_GRAYSCALE);

    if(image.empty()){
        cout << "Cannot open the image!" << endl;
        return 4;
    }

// ******************** Sobel ***********************//
    
    int Skernel1[N][N] = {  {-1, -2, -1}, 
                            {0, 0, 0}, 
                            {1, 2, 1}};
    Mat Sobel_image1 = mask(image, Skernel1);
    int Skernel2[N][N] = {  {-1, 0, 1}, 
                            {-2, 0, 2},
                            {-1, 0, 1}};
    Mat Sobel_image2 = mask(image, Skernel2);
    Mat Sobel_tmpimage = Sobel_image1.clone();
    for(auto it=Sobel_tmpimage.begin<short>(), it2=Sobel_image2.begin<short>();
        it!=Sobel_tmpimage.end<short>() && it2!=Sobel_image2.end<short>(); it++, it2++){
        if(*it < *it2){
            *it = *it2;
        }
    }
    // stretch
    Mat Sobel_image;
    
    Sobel_tmpimage.convertTo(Sobel_image, CV_8UC1);
    cout << Sobel_image << endl;

// ******************** Laplace ********************** //

int Lkernel1[N][N] = {{0, 1, 0},
                     {1, -4, 1},
                     {0, 1, 0}};
Mat Laplace1_tmp = mask(image, Lkernel1);
Mat Laplace1 = stretch(Laplace1_tmp);

int Lkernel2[N][N] = {{-1, -1, -1},
                     {-1, 8, -1},
                     {-1, -1, -1}};
Mat Laplace2_tmp = mask(image, Lkernel2);
Mat Laplace2 = stretch(Laplace2_tmp);



// ******************** Show image ******************** //
    namedWindow("Ori_image", WINDOW_AUTOSIZE);
    imshow("Ori_image", image);
    namedWindow("Sobel_image", WINDOW_AUTOSIZE);
    imshow("Sobel_image", Sobel_image);
    namedWindow("Laplace_image1", WINDOW_AUTOSIZE);
    imshow("Laplace_image1", Laplace1);
    namedWindow("Laplace_image2", WINDOW_AUTOSIZE);
    imshow("Laplace_image2", Laplace2);
    

    waitKey(0);

    return 0;
}