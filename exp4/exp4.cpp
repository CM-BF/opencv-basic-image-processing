#include<iomanip>
#include<iostream>
#include<stdio.h>
#include<cmath>
#include<algorithm>
#include<complex>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace complex_literals;


int main(int argc, char ** argv)
{
    Mat raw_image = imread("../../data/rect1.bmp", IMREAD_GRAYSCALE);

    if(raw_image.empty()){
        cout << "Cannot open the rect1!" << endl;
        return 4;
    }

    int N = raw_image.rows;
    Mat image;
    raw_image.convertTo(image, CV_64FC1);

    // ***************** 先在y方向做变换 ************* //

    Mat tmpMatrix = Mat::zeros(image.size(), CV_64FC2);

    for(int x=0; x<N; x++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int y=0; y<N; y++){
                double exponent = 2*M_PI*v*y/N;
                complex<double> factor = cos(exponent)
                                        - sin(exponent)*1i; // exp(-j...)
                tmpSum += image.at<double>(x, y) * factor; 
            }
            tmpMatrix.at<Vec2d>(x, v)[0] = tmpSum.real();
            tmpMatrix.at<Vec2d>(x, v)[1] = tmpSum.imag();
        }
    }

    // **************** 对x方向做变换，得到结果 ********* //

    Mat raw_Fourier_image = Mat::zeros(image.size(), CV_64FC2);

    for(int u=0; u<N; u++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int x=0; x<N; x++){
                double exponent = 2*M_PI*u*x/N;
                complex<double> factor = cos(exponent)
                                        - sin(exponent)*1i; // exp(-j...)
                tmpSum += tmpMatrix.at<Vec2d>(x, v)[0] * factor
                        + tmpMatrix.at<Vec2d>(x, v)[1] * complex<double>{0.0, 1.0} * factor;
            }
            tmpSum /= N;
            raw_Fourier_image.at<Vec2d>(u, v)[0] = tmpSum.real();
            raw_Fourier_image.at<Vec2d>(u, v)[1] = tmpSum.imag();
        }
    }

    // **************** 得其幅度谱（频谱）、相谱 *********** //

    Mat Fourier_image_frequence = Mat::zeros(image.size(), CV_64FC1);
    Mat Fourier_image_phase = Mat::zeros(image.size(), CV_64FC1);

    MatIterator_<double> itf, itp;
    MatIterator_<Vec2d> itr;
    for(itf=Fourier_image_frequence.begin<double>(), itr=raw_Fourier_image.begin<Vec2d>(), itp=Fourier_image_phase.begin<double>();
        itf!=Fourier_image_frequence.end<double>() && itr!=raw_Fourier_image.end<Vec2d>() && itp!=Fourier_image_phase.end<double>();
        itf++, itr++, itp++){
        *itf = log(sqrt(pow((*itr)[0], 2) + pow((*itr)[1], 2)) + 1); // D=log[|F(u,v)|+1],  |F(u,v)| = (R^2 + I^2)^(1/2)
        *itp = atan((*itr)[1]/(*itr)[0]);
    }
    
    normalize(Fourier_image_frequence, Fourier_image_frequence, 0, 1, NORM_MINMAX);

    // ************************* 平移中心 ************************* //
    Mat Fourier_image_frequence_center = Fourier_image_frequence.clone();
    for(int x=0; x<N; x++){
        for(int y=0; y<N; y++){
            if(x < N/2 && y < N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence.at<double>(x + N/2, y + N/2);
            if(x >= N/2 && y < N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence.at<double>(x - N/2, y + N/2);
            if(x < N/2 && y >= N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence.at<double>(x + N/2, y - N/2);
            if(x >= N/2 && y >= N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence.at<double>(x - N/2, y - N/2);
        }
    }
    // cout << format(Fourier_image_frequence, Formatter::FMT_NUMPY) << endl;

    namedWindow("Frequence", WINDOW_AUTOSIZE);
    imshow("Frequence", Fourier_image_frequence_center);

    waitKey(0);

    return 0;
}