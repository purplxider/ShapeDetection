#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void Gaussian(Mat* pSrc, Mat* pDst);
int CornerNumDetection(Mat* image, double th);

int main() {
    Mat image[4];
    image[0] = imread("1.jpg", IMREAD_GRAYSCALE);
    image[1] = imread("2.jpg", IMREAD_GRAYSCALE);
    image[2] = imread("3.jpg", IMREAD_GRAYSCALE);
    image[3] = imread("4.jpg", IMREAD_GRAYSCALE);
    double R, G, B, gray;
    int numimage = 4;

    int corner[4];

    double th = 20000;
    for (int i = 0; i < numimage; i++) {
        corner[i] = CornerNumDetection(&image[i], 20000);

        if (corner[i] == 3) {
            cout << i << " object is Triangle" << endl;
        }
        else if (corner[i] == 4) {
            cout << i << " object is Square" << endl;
        }
    }

    return 0;
}




void Gaussian(Mat* pSrc, Mat* pDst)
{
    double mask[5][5] = {
       {1, 4, 6, 4, 1},
       {4, 16, 24, 16, 4},
       {6, 24, 36, 24, 6},
       {4, 16, 24, 16, 4},
       {1, 4, 6, 4, 1} };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            mask[i][j] /= 256;
        }
    }

    for (int j = 2; j < pSrc->cols - 2; j++) {
        for (int i = 2; i < pSrc->rows - 2; i++) {
            pDst->at<double>(j, i) =
                pSrc->at<double>(j - 2, i - 2) * mask[0][0]
                + pSrc->at<double>(j - 2, i - 1) * mask[0][1]
                + pSrc->at<double>(j - 2, i) * mask[0][2]
                + pSrc->at<double>(j - 2, i + 1) * mask[0][3]
                + pSrc->at<double>(j - 2, i + 2) * mask[0][4]
                + pSrc->at<double>(j - 1, i - 2) * mask[1][0]
                + pSrc->at<double>(j - 1, i - 1) * mask[1][1]
                + pSrc->at<double>(j - 1, i) * mask[1][2]
                + pSrc->at<double>(j - 1, i + 1) * mask[1][3]
                + pSrc->at<double>(j - 1, i + 2) * mask[1][4]
                + pSrc->at<double>(j, i - 2) * mask[2][0]
                + pSrc->at<double>(j, i - 1) * mask[2][1]
                + pSrc->at<double>(j, i) * mask[2][2]
                + pSrc->at<double>(j, i + 1) * mask[2][3]
                + pSrc->at<double>(j, i + 2) * mask[2][4]
                + pSrc->at<double>(j + 1, i - 2) * mask[3][0]
                + pSrc->at<double>(j + 1, i - 1) * mask[3][1]
                + pSrc->at<double>(j + 1, i) * mask[3][2]
                + pSrc->at<double>(j + 1, i + 1) * mask[3][3]
                + pSrc->at<double>(j + 1, i + 2) * mask[3][4]
                + pSrc->at<double>(j + 2, i - 2) * mask[4][0]
                + pSrc->at<double>(j + 2, i - 1) * mask[4][1]
                + pSrc->at<double>(j + 2, i) * mask[4][2]
                + pSrc->at<double>(j + 2, i + 1) * mask[4][3]
                + pSrc->at<double>(j + 2, i + 2) * mask[4][4];
        }
    }
}

//harrisconerdection algorithm 사용
int CornerNumDetection(Mat* image, double th)
{
    int i, j, x, y;

    int w = image->cols; //width
    int h = image->rows; //height
    //newimage
    Mat imagex2(image->size(), CV_64F, Scalar(0));
    Mat imagey2(image->size(), CV_64F, Scalar(0));
    Mat imagexy(image->size(), CV_64F, Scalar(0));

    double tx, ty;
    for (j = 1; j < h - 1; j++) // width
    {
        for (i = 1; i < w - 1; i++) // height
        {
            // 해리스코너검출식 사용
            // 주변의 픽셀들의 평균 구하기
            tx = (image->at<uchar>(j - 1, i + 1) + image->at<uchar>(j, i + 1) + image->at<uchar>(j + 1, i + 1)
                - image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j, i - 1) - image->at<uchar>(j + 1, i - 1)) / 6.f;

            ty = (image->at<uchar>(j + 1, i - 1) + image->at<uchar>(j + 1, i) + image->at<uchar>(j + 1, i + 1)
                - image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j - 1, i) - image->at<uchar>(j - 1, i + 1)) / 6.f;

            imagex2.at<double>(j, i) = tx * tx;
            imagey2.at<double>(j, i) = ty * ty;
            imagexy.at<double>(j, i) = tx * ty;
        }
    }


    // 필터로 가우시안 블러 적용
    Mat imagex2(image->size(), CV_64F, Scalar(0));
    Mat imagey2(image->size(), CV_64F, Scalar(0));
    Mat imagexy(image->size(), CV_64F, Scalar(0));
    Gaussian(&imagex2, &imagex2);
    Gaussian(&imagey2, &imagey2);
    Gaussian(&imagexy, &imagexy);

    Mat newimage(image->size(), CV_64F, Scalar(0));
    double k = 0.04; // k = 0.04 ~ 0.06
    for (j = 2; j < h - 2; j++)
    {
        for (i = 2; i < w - 2; i++)
        {
            //필터링한 이미지를 해리스 코너 검출기를 통해 새로운이미지 생성
            //newimage = det(i1 * i2) - k * (i1+i2)
            newimage.at<double>(j, i) = (imagex2.at<double>(j, i) * imagey2.at<double>(j, i)
                - imagexy.at<double>(j, i) * imagexy.at<double>(j, i))
                - k * (imagex2.at<double>(j, i) + imagey2.at<double>(j, i)) * (imagex2.at<double>(j, i) + imagey2.at<double>(j, i));

        }
    }


    int corners = 0;
    double cvf_value = 0;
    for (j = 2; j < h - 2; j++)
    {
        for (i = 2; i < w - 2; i++)
        {
            cvf_value = newimage.at<double>(j, i);
            if (cvf_value > th)
            {
                if (cvf_value > newimage.at<double>(j - 1, i) && cvf_value > newimage.at<double>(j - 1, i + 1) &&
                    cvf_value > newimage.at<double>(j, i + 1) && cvf_value > newimage.at<double>(j + 1, i + 1) &&
                    cvf_value > newimage.at<double>(j + 1, i) && cvf_value > newimage.at<double>(j + 1, i - 1) &&
                    cvf_value > newimage.at<double>(j, i - 1) && cvf_value > newimage.at<double>(j - 1, i - 1))
                {
                    corners++;
                }
            }
        }
    }
    return corners;
}