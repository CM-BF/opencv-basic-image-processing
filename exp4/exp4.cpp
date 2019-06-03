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
    Mat raw_image = imread("../../data/rect2.bmp", IMREAD_GRAYSCALE);

    if(raw_image.empty()){
        cout << "Cannot open the image!" << endl;
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
        *itf = sqrt(pow((*itr)[0], 2) + pow((*itr)[1], 2)); // |F(u,v)| = (R^2 + I^2)^(1/2)
        *itp = atan2((*itr)[0], (*itr)[1]);
        if(*itp < 0)
            *itp += 2*M_PI;
    }
    

    // ************************* 压缩范围，归一化，平移中心 ************************* //
    Mat Fourier_image_frequence_process = Fourier_image_frequence.clone();
    Mat Fourier_image_frequence_center = Fourier_image_frequence.clone();
    MatIterator_<double> itc;

    for(itf=Fourier_image_frequence.begin<double>(), itc=Fourier_image_frequence_process.begin<double>();
     itf!=Fourier_image_frequence.end<double>() && itc!=Fourier_image_frequence_process.end<double>();
        itf++, itc++){
        *itc = log(*itf + 1); // log[|F(u,v)| + 1]
    }
    
    normalize(Fourier_image_frequence_process, Fourier_image_frequence_process, 0, 1, NORM_MINMAX);

    for(int x=0; x<N; x++){
        for(int y=0; y<N; y++){
            if(x < N/2 && y < N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence_process.at<double>(x + N/2, y + N/2);
            if(x >= N/2 && y < N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence_process.at<double>(x - N/2, y + N/2);
            if(x < N/2 && y >= N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence_process.at<double>(x + N/2, y - N/2);
            if(x >= N/2 && y >= N/2)
                Fourier_image_frequence_center.at<double>(x, y) = Fourier_image_frequence_process.at<double>(x - N/2, y - N/2);
        }
    }

    // ************************* 反变换 ************************** //

    // **************** 幅度频 ********************* //
    // ***************** 先在y方向做变换 ************* //

    tmpMatrix = Mat::zeros(image.size(), CV_64FC2);

    for(int x=0; x<N; x++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int y=0; y<N; y++){
                double exponent = 2*M_PI*v*y/N;
                complex<double> factor = cos(exponent)
                                        + sin(exponent)*1i; // exp(+j...) 正反变换不同的地方
                // tmpSum += raw_Fourier_image.at<Vec2d>(x, y)[0] * factor 
                //     + raw_Fourier_image.at<Vec2d>(x, y)[0] * complex<double>{0.0, 1.0} * factor;  // 反变换幅度频和相位谱不同的地方
                tmpSum += Fourier_image_frequence.at<double>(x, y) * factor;
            }
            tmpMatrix.at<Vec2d>(x, v)[0] = tmpSum.real();
            tmpMatrix.at<Vec2d>(x, v)[1] = tmpSum.imag();
        }
    }

    // **************** 对x方向做变换，得到结果 ********* //

    Mat raw_result_image = Mat::zeros(image.size(), CV_64FC2);

    for(int u=0; u<N; u++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int x=0; x<N; x++){
                double exponent = 2*M_PI*u*x/N;
                complex<double> factor = cos(exponent)
                                        + sin(exponent)*1i; // exp(+j...) 正反变换不同的地方
                tmpSum += tmpMatrix.at<Vec2d>(x, v)[0] * factor
                        + tmpMatrix.at<Vec2d>(x, v)[1] * complex<double>{0.0, 1.0} * factor;
            }
            tmpSum /= N;
            raw_result_image.at<Vec2d>(u, v)[0] = tmpSum.real();
            raw_result_image.at<Vec2d>(u, v)[1] = tmpSum.imag();
        }
    }

    // ************************* 压缩范围，归一化，平移中心 ************************* //
    Mat result_image = Mat::zeros(raw_result_image.size(), CV_64FC1);

    for(itr=raw_result_image.begin<Vec2d>(), itc=result_image.begin<double>();
     itr!=raw_result_image.end<Vec2d>() && itc!=result_image.end<double>();
        itr++, itc++){
        *itc = log(sqrt(pow((*itr)[0], 2) + pow((*itr)[1], 2)) + 1); // 求模
        // *itc = (*itr)[0];
    }

    
    normalize(result_image, result_image, 0, 1, NORM_MINMAX);


    // **************** 相谱 ********************* //
    // ***************** 先在y方向做变换 ************* //

    tmpMatrix = Mat::zeros(image.size(), CV_64FC2);

    for(int x=0; x<N; x++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int y=0; y<N; y++){
                double exponent = 2*M_PI*v*y/N;
                complex<double> factor = cos(exponent)
                                        + sin(exponent)*1i; // exp(+j...) 正反变换不同的地方
                // tmpSum += raw_Fourier_image.at<Vec2d>(x, y)[0] * factor 
                //     + raw_Fourier_image.at<Vec2d>(x, y)[1] * complex<double>{0.0, 1.0} * factor;  // 反变换幅度频和相位谱不同的地方
                tmpSum += Fourier_image_phase.at<double>(x, y) * factor;
            }
            tmpMatrix.at<Vec2d>(x, v)[0] = tmpSum.real();
            tmpMatrix.at<Vec2d>(x, v)[1] = tmpSum.imag();
        }
    }

    // **************** 对x方向做变换，得到结果 ********* //

    Mat raw_result_image2 = Mat::zeros(image.size(), CV_64FC2);

    for(int u=0; u<N; u++){
        for(int v=0; v<N; v++){
            complex<double> tmpSum = 0;
            for(int x=0; x<N; x++){
                double exponent = 2*M_PI*u*x/N;
                complex<double> factor = cos(exponent)
                                        + sin(exponent)*1i; // exp(+j...) 正反变换不同的地方
                tmpSum += tmpMatrix.at<Vec2d>(x, v)[0] * factor
                        + tmpMatrix.at<Vec2d>(x, v)[1] * complex<double>{0.0, 1.0} * factor;
            }
            tmpSum /= N;
            raw_result_image2.at<Vec2d>(u, v)[0] = tmpSum.real();
            raw_result_image2.at<Vec2d>(u, v)[1] = tmpSum.imag();
        }
    }

    // ************************* 压缩范围，归一化 ************************* //
    Mat result_image2 = Mat::zeros(raw_result_image.size(), CV_64FC1);

    for(itr=raw_result_image2.begin<Vec2d>(), itc=result_image2.begin<double>();
     itr!=raw_result_image2.end<Vec2d>() && itc!=result_image2.end<double>();
        itr++, itc++){
        *itc = log(sqrt(pow((*itr)[0], 2) + pow((*itr)[1], 2)) + 1); // 求模
        // *itc = (*itr)[1];
    }

    
    normalize(result_image2, result_image2, 0, 1, NORM_MINMAX);


    namedWindow("Origin", WINDOW_AUTOSIZE);
    imshow("Origin", image);
    namedWindow("Frequence", WINDOW_AUTOSIZE);
    imshow("Frequence", Fourier_image_frequence_center);
    namedWindow("Fre_Origin", WINDOW_AUTOSIZE);
    imshow("Fre_Origin", result_image);
    namedWindow("Pha_Origin", WINDOW_AUTOSIZE);
    imshow("Pha_Origin", result_image2);

    waitKey(0);

    return 0;
}