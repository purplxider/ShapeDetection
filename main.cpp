#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

using namespace std;
using namespace cv;

#include "stdafx.h"
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

const double PI = 3.14159265358979323846;


void GaussianFiltering(Mat* pSrc, Mat* pDst, double sigma);
void HarrisCornerDetection(Mat* image, vector<Point>* corners, double th);

int main()
{
	Mat image = imread("../_res/harris_test.bmp", IMREAD_GRAYSCALE);
	CV_Assert(!image.empty());

	Mat cornerimage(image.size(), CV_8UC3);
	cvtColor(image, cornerimage, CV_GRAY2BGR);

	double th = 20000;
	vector<Point> vecCorner;
	HarrisCornerDetection(&image, &vecCorner, th);

	// 코너 그리기
	for (int l = 0; l < (int)vecCorner.size(); l++)
	{
		circle(cornerimage, vecCorner[l], 3, Scalar(255, 0, 0), CV_FILLED);
	}

	imshow("Input Image", image);
	imshow("Harris Corner Image", cornerimage);

	waitKey(0);

	return 0;
}


void GaussianFiltering(Mat* pSrc, Mat* pDst, double sigma)
{
	int dim = static_cast<int>(8 * sigma + 1.0);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	GaussianBlur(*pSrc, *pDst, Size(dim, dim), sigma, sigma);
}


void HarrisCornerDetection(Mat* image, vector<Point>* corners, double th)
{
	int i, j, x, y;

	int w = image->cols;
	int h = image->rows;


	//-------------------------------------------------------------------------
	// 1. (fx)*(fx), (fx)*(fy), (fy)*(fy) 계산
	//-------------------------------------------------------------------------
	Mat imagedx2(image->size(), CV_64F, Scalar(0));
	Mat imagedy2(image->size(), CV_64F, Scalar(0));
	Mat imagedxy(image->size(), CV_64F, Scalar(0));

	double tx, ty;
	for (j = 1; j < h - 1; j++)
	{
		for (i = 1; i < w - 1; i++)
		{
			tx = (image->at<uchar>(j - 1, i + 1) + image->at<uchar>(j, i + 1) + image->at<uchar>(j + 1, i + 1)
				- image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j, i - 1) - image->at<uchar>(j + 1, i - 1)) / 6.0;

			ty = (image->at<uchar>(j + 1, i - 1) + image->at<uchar>(j + 1, i) + image->at<uchar>(j + 1, i + 1)
				- image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j - 1, i) - image->at<uchar>(j - 1, i + 1)) / 6.0;

			imagedx2.at<double>(j, i) = tx * tx;
			imagedy2.at<double>(j, i) = ty * ty;
			imagedxy.at<double>(j, i) = tx * ty;
		}
	}


	//-------------------------------------------------------------------------
	// 2. 가우시안 필터링
	//-------------------------------------------------------------------------
	double sigma = 1.0;
	Mat imageGdx2(image->size(), CV_64F, Scalar(0));
	Mat imageGdy2(image->size(), CV_64F, Scalar(0));
	Mat imageGdxy(image->size(), CV_64F, Scalar(0));
	GaussianFiltering(&imagedx2, &imageGdx2, sigma);
	GaussianFiltering(&imagedy2, &imageGdy2, sigma);
	GaussianFiltering(&imagedxy, &imageGdxy, sigma);


	//-------------------------------------------------------------------------
	// 3. 코너 응답 함수 생성
	//-------------------------------------------------------------------------
	Mat crf(image->size(), CV_64F, Scalar(0));
	double k = 0.04;
	for (j = 2; j < h - 2; j++)
	{
		for (i = 2; i < w - 2; i++)
		{
			crf.at<double>(j, i) = (imageGdx2.at<double>(j, i) * imageGdy2.at<double>(j, i) - imageGdxy.at<double>(j, i) * imageGdxy.at<double>(j, i))
				- k * (imageGdx2.at<double>(j, i) + imageGdy2.at<double>(j, i)) * (imageGdx2.at<double>(j, i) + imageGdy2.at<double>(j, i));

		}
	}

	//-------------------------------------------------------------------------
	// 4. 임계값보다 큰 국지적 최대값을 찾아 코너 포인트로 결정
	//-------------------------------------------------------------------------
	double cvf_value = 0;
	for (j = 2; j < h - 2; j++)
	{
		for (i = 2; i < w - 2; i++)
		{
			cvf_value = crf.at<double>(j, i);
			if (cvf_value > th)
			{
				if (cvf_value > crf.at<double>(j - 1, i) && cvf_value > crf.at<double>(j - 1, i + 1) &&
					cvf_value > crf.at<double>(j, i + 1) && cvf_value > crf.at<double>(j + 1, i + 1) &&
					cvf_value > crf.at<double>(j + 1, i) && cvf_value > crf.at<double>(j + 1, i - 1) &&
					cvf_value > crf.at<double>(j, i - 1) && cvf_value > crf.at<double>(j - 1, i - 1))
				{
					corners->push_back(Point(i, j));
				}
			}
		}
	}
}

int main() {
    Mat image[4];
    image[0] = imread("1.jpg");
    image[1] = imread("2.jpg");
    //image[2] = imread("3.jpg");
    //image[3] = imread("4.jpg");
    double R, G, B, gray;
    for (int z = 0; z < 2 ; z++) {

        for (int j = 0; j < image[z].rows ; j++) {
            for (int i = 0; i < image[z].cols; i++) {
                R = image[z].at<cv::Vec3b>(j, i)[2];
                G = image[z].at<cv::Vec3b>(j, i)[1];
                B = image[z].at<cv::Vec3b>(j, i)[0];
                gray = (R + G + B )/3;
                image[z].at<cv::Vec3b>(j, i)[2] = gray;
                image[z].at<cv::Vec3b>(j, i)[1] = gray;
                image[z].at<cv::Vec3b>(j, i)[0] = gray;

            }
        }
    }


    imshow("image1", image[0]);
    imshow("image", image[1]);
    waitKey();
    return 0;
}