#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<vector>
#include<algorithm>

#define N 3

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat image = imread("../../data/lena.bmp", IMREAD_GRAYSCALE);

    if(image.empty()){
        cout << "Cannot open the image!" << endl;
        return 4;
    }
    // add noice
    // rand initial
    srand((unsigned int)time(NULL));
    int r = rand() % 100;
    
    Mat noice_image = image.clone();
    for(auto it = noice_image.begin<uchar>(); it != noice_image.end<uchar>(); it++){
        if(rand() % 100 < 3){
            if(rand() % 2 == 0){
                *it = 0;
            }else{
                *it = 255;
            }
        }
    }

// ******************** Filter 1*********************** //

    Mat_<uchar> filter_image1 = noice_image.clone();
    int nrows = filter_image1.rows;
    int ncols = filter_image1.cols;
    uchar kernel[N][N] = {  {1, 1, 1},
                            {1, 1, 1},
                            {1, 1, 1}};
    int w = (N - 1) / 2;
    
    for(int i=1; i<nrows-1; i++){
        for(int j=1; j<ncols-1; j++){
            int sum = 0;
            for(int x=i - w; x<=i + w; x++){
                for(int y=j - w; y<=j + w; y++){
                    sum += kernel[x - i + w][y - j + w] * noice_image.at<uchar>(x, y);
                }
            }
            
            filter_image1(i, j) = (uchar)(sum / (N*N));
        }
    }
    

// ******************** Filter2 ***************************//

    Mat_<uchar> filter_image2 = noice_image.clone();
    vector<uchar> sortVec;
    
    for(int i=1; i<nrows-1; i++){
        for(int j=1; j<ncols-1; j++){
            sortVec.clear();
            for(int x=i - w; x<=i + w; x++){
                for(int y=j - w; y<=j + w; y++){
                    sortVec.push_back(noice_image.at<uchar>(x, y));
                }
            }
            sort(sortVec.begin(), sortVec.end(), less<uchar>());
            filter_image2(i, j) = (uchar)(sortVec.at(4));
        }
    }
    


// ******************** Show image ******************** //
    namedWindow("Ori_image", WINDOW_AUTOSIZE);
    imshow("Ori_image", image);
    namedWindow("Noice_image", WINDOW_AUTOSIZE);
    imshow("Noice_image", noice_image);
    namedWindow("Filter_image1", WINDOW_AUTOSIZE);
    imshow("Filter_image1", filter_image1);
    namedWindow("Filter_image2", WINDOW_AUTOSIZE);
    imshow("Filter_image2", filter_image2);

    waitKey(0);

    return 0;
}