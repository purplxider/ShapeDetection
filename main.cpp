Mat image[4];
image[0] = imread("1.jpg");
image[1] = imread("2.jpg");
//image[2] = imread("3.jpg");
//image[3] = imread("4.jpg");
double R, G, B, gray;
for (int z = 0; z < 2; z++) {

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


imshow("image1", image[0]);
imshow("image", image[1]);
return 0;