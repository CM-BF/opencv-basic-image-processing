#include"Column.h"

Mat Column(Mat &image, int x1, int x2)
{
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
    return I;
}
