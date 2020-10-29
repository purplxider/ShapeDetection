#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;


const double PI = 3.14159265358979323846;


void Gaussian(Mat* pSrc, Mat* pDst);
int CornerNumDetection(Mat* image,double th);

int main() {
	Mat image[4];
	image[0] = imread("c:\Nexon\1.jpg");
	image[1] = imread("c:\Nexon\2.jpg");
	//image[2] = imread("3.jpg");
	//image[3] = imread("4.jpg");
	double R, G, B, gray;
	int numimage = 2;


	for (int z = 0; z < numimage; z++) {

		for (int j = 0; j < image[z].rows; j++) {
			for (int i = 0; i < image[z].cols; i++) {
				R = image[z].at<cv::Vec3b>(j, i)[2];
				G = image[z].at<cv::Vec3b>(j, i)[1];
				B = image[z].at<cv::Vec3b>(j, i)[0];
				gray = (R + G + B) / 3;
				image[z].at<cv::Vec3b>(j, i)[2] = gray;
				image[z].at<cv::Vec3b>(j, i)[1] = gray;
				image[z].at<cv::Vec3b>(j, i)[0] = gray;

			}
		}
	}
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


	imshow("image1", image[0]);
	imshow("image", image[1]);
	waitKey();
	return 0;
}




void Gaussian(Mat* pSrc, Mat* pDst)
{
	int mask[5][5] = {
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
			pDst->at<uchar>(j, i) =
				pSrc->at<uchar>(j - 2, i - 2) * mask[0][0]
				+ pSrc->at<uchar>(j - 2, i - 1) * mask[0][1]
				+ pSrc->at<uchar>(j - 2, i) * mask[0][2]
				+ pSrc->at<uchar>(j - 2, i + 1) * mask[0][3]
				+ pSrc->at<uchar>(j - 2, i + 2) * mask[0][4]
				+ pSrc->at<uchar>(j - 1, i - 2) * mask[1][0]
				+ pSrc->at<uchar>(j - 1, i - 1) * mask[1][1]
				+ pSrc->at<uchar>(j - 1, i) * mask[1][2]
				+ pSrc->at<uchar>(j - 1, i + 1) * mask[1][3]
				+ pSrc->at<uchar>(j - 1, i + 2) * mask[1][4]
				+ pSrc->at<uchar>(j, i - 2) * mask[2][0]
				+ pSrc->at<uchar>(j, i - 1) * mask[2][1]
				+ pSrc->at<uchar>(j, i) * mask[2][2]
				+ pSrc->at<uchar>(j, i + 1) * mask[2][3]
				+ pSrc->at<uchar>(j, i + 2) * mask[2][4]
				+ pSrc->at<uchar>(j + 1, i - 2) * mask[3][0]
				+ pSrc->at<uchar>(j + 1, i - 1) * mask[3][1]
				+ pSrc->at<uchar>(j + 1, i) * mask[3][2]
				+ pSrc->at<uchar>(j + 1, i + 1) * mask[3][3]
				+ pSrc->at<uchar>(j + 1, i + 2) * mask[3][4]
				+ pSrc->at<uchar>(j + 2, i - 2) * mask[4][0]
				+ pSrc->at<uchar>(j + 2, i - 1) * mask[4][1]
				+ pSrc->at<uchar>(j + 2, i) * mask[4][2]
				+ pSrc->at<uchar>(j + 2, i + 1) * mask[4][3]
				+ pSrc->at<uchar>(j + 2, i + 2) * mask[4][4];
		}
	}
}

//harrisconerdection algorithm ���
int CornerNumDetection(Mat* image, double th)
{
	int i, j, x, y;

	int w = image->cols; //width
	int h = image->rows; //height
	//newimage
	//Mat imagex2, imagey2, imagexy;
	Mat imagex2(image->size(), CV_64F, Scalar(0));
	Mat imagey2(image->size(), CV_64F, Scalar(0));
	Mat imagexy(image->size(), CV_64F, Scalar(0));

	double tx, ty;
	for (j = 1; j < h - 1; j++) // width
	{
		for (i = 1; i < w - 1; i++) // height
		{
			// 1. (fx)*(fx), (fx)*(fy), (fy)*(fy) ���

			tx = (image->at<uchar>(j - 1, i + 1) + image->at<uchar>(j, i + 1) + image->at<uchar>(j + 1, i + 1)
				- image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j, i - 1) - image->at<uchar>(j + 1, i - 1)) / 6.f;

			ty = (image->at<uchar>(j + 1, i - 1) + image->at<uchar>(j + 1, i) + image->at<uchar>(j + 1, i + 1)
				- image->at<uchar>(j - 1, i - 1) - image->at<uchar>(j - 1, i) - image->at<uchar>(j - 1, i + 1)) / 6.f;

			imagex2.at<double>(j, i) = tx * tx;
			imagey2.at<double>(j, i) = ty * ty;
			imagexy.at<double>(j, i) = tx * ty;
		}
	}


	// ���ͷ� ����þ� �� ����
	Mat imageGdx2(image->size(), CV_64F, Scalar(0));
	Mat imageGdy2(image->size(), CV_64F, Scalar(0));
	Mat imageGdxy(image->size(), CV_64F, Scalar(0));
	Gaussian(&imagex2, &imageGdx2);
	Gaussian(&imagey2, &imageGdy2);
	Gaussian(&imagexy, &imageGdxy);


	Mat newimage(image->size(), CV_64F, Scalar(0));
	double k = 0.04; // k = 0.04 ~ 0.06
	for (j = 2; j < h - 2; j++)
	{
		for (i = 2; i < w - 2; i++)
		{
			//���͸��� �̹����� �ظ��� �ڳ� ����⸦ ���� ���ο��̹��� ����
			//newimage = det(i1 * i2) - k * (i1+i2)
			newimage.at<double>(j, i) = (imageGdx2.at<double>(j, i) * imageGdy2.at<double>(j, i)
				- imageGdxy.at<double>(j, i) * imageGdxy.at<double>(j, i))
				- k * (imageGdx2.at<double>(j, i) + imageGdy2.at<double>(j, i)) * (imageGdx2.at<double>(j, i) + imageGdy2.at<double>(j, i));

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