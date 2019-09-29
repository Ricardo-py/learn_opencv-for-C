#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;


int g_histSize = 30,histSize;
Mat srcImage, hueImage;

void calcHistandBackH()
{
	Mat hist;
	int channels = 0;
	histSize = g_histSize;
	float ranges[] = { 0,180 };
	const float* sizeRanges = { ranges };
	calcHist(&hueImage, 1, &channels, Mat(), hist, 1, &histSize, &sizeRanges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	Mat dst;
	calcBackProject(&hueImage, 1, &channels, hist, dst, &sizeRanges, 1.0, true);

	imshow("反向直方图", dst);
}




void onChange(int, void *)
{
	calcHistandBackH();
}


//直方图反向投影
int main()
{
	srcImage = imread("E:/计算机视觉/Tracking_dataset/Boy/img/0001.jpg");

	if (srcImage.empty())
	{
		cout << "图像加载失败" << endl;
		return -1;
	}

	namedWindow("原始图像", WINDOW_NORMAL);
	imshow("原始图像", srcImage);
	
	Mat hsvImage;
	cvtColor(srcImage, hsvImage, CV_BGR2HSV);

	//分离hue图像
	int ch[] = { 0,0 };
	cout << hueImage.channels() << endl;
	hueImage = Mat(hsvImage.size(),hsvImage.depth());
	mixChannels(hsvImage, hueImage,ch,1);

	cout << "成功" << endl;

	namedWindow("hueImage", WINDOW_NORMAL);
	imshow("hueImage", hueImage);

	Mat hist;
	int channels = 0;
	histSize = g_histSize;
	float ranges[] = { 0,180 };
	const float* sizeRanges = { ranges };
	calcHist(&hueImage, 1, &channels, Mat(), hist, 1, &histSize, &sizeRanges, true, false);

	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	Mat dst;
	calcBackProject(&hueImage, 1, &channels, hist, dst, &sizeRanges, 1.0, true);
	namedWindow("反向直方图",WINDOW_NORMAL);
	createTrackbar("Hmin", "反向直方图", &g_histSize, 180, onChange);
	imshow("反向直方图", dst);
	waitKey();







	//计算直方图

	waitKey();
	return 0;
}