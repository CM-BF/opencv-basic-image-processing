#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include<ctime>
using namespace cv;
using namespace std;
 
//均值滤波
void AverFiltering(const Mat &src,Mat &dst) {
	if (!src.data) return;
	//at访问像素点
	for (int i = 1; i<src.rows; ++i)
		for (int j = 1; j < src.cols; ++j) {
			if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols) {//边缘不进行处理
				dst.at<uchar>(i, j) = (src.at<uchar>(i, j) + src.at<uchar>(i - 1, j - 1) + src.at<uchar>(i - 1, j) + src.at<uchar>(i, j - 1) +
					src.at<uchar>(i - 1, j + 1) + src.at<uchar>(i + 1, j - 1) + src.at<uchar>(i + 1, j + 1) + src.at<uchar>(i, j + 1) +
					src.at<uchar>(i + 1, j)) / 9;
			}
			else {//边缘赋值
				dst.at<uchar>(i, j) = src.at<uchar>(i, j);
			}
		}
}
//图像椒盐化
void salt(Mat &image, int num) {
	if (!image.data) return;//防止传入空图
	int i, j;
	srand(time(NULL));
	for (int x = 0; x < num; ++x) {
		i = rand() % image.rows;
		j = rand() % image.cols;
		image.at<uchar>(i, j) = 255;
	}
}
int main() {
	Mat image = imread("../../data/pout.bmp", IMREAD_GRAYSCALE);
 
	Mat Salt_Image = image.clone();
	salt(Salt_Image, 3000);
 
	Mat image1(image.size(), image.type());
	//Mat image2;
	AverFiltering(Salt_Image, image1);
	//blur(Salt_Image, image2, Size(3, 3));//openCV库自带的均值滤波函数
	imshow("Origin", image);
	imshow("my", image1);
	//imshow("openCV", image2);
	waitKey();
	return 0;
}