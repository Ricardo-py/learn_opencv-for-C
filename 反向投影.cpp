#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

#define INPUT_TITLE "input image"
#define OUTPUT_TITLE "直方图计算"

using namespace cv;
using namespace std;

int global_bin = 10;
Mat hsv_Image, H_image, S_image, V_image;

void onChangeBin(int, void*)
{
	int histSize = MAX(global_bin,2);
	float range[] = { 0, 180 };
	const float* sizeRange = { range };

	int channels = { 0 };

	Mat hist;
	calcHist(&H_image, 1, &channels, Mat(), hist, 1, &histSize, &sizeRange, true, false);

	Mat dst;
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
	calcBackProject(&H_image, 1, &channels, hist, dst, &sizeRange, 1.0, true);

	//namedWindow("result", WINDOW_NORMAL);
	imshow("反向投影图", dst);
}

int main()
{
	//Mat image = (Mat_<float>(4,4) << 1, 2, 3, 4, 5, 6, 7, 7, 9, 8, 0, 1, 5, 6, 7, 6);
	Mat srcImage = imread("C:/Users/Richard/Desktop/hand1.jpg");
	//image.convertTo(image, CV_32F);
	if (srcImage.empty())
	{
		cout << "图像加载失败" << endl;
		return -1;
	}
	/*Mat hsvImage,hueImage,temp;
	cvtColor(srcImage, temp, COLOR_BGR2HSV);
	srcImage.copyTo(hsvImage);
	namedWindow("hsvImage", WINDOW_NORMAL);
	imshow("hsvImage", hsvImage);
	hueImage.create(temp.size(),temp.depth());

	int ch[] = { 0,0 };
	mixChannels(&hsvImage, 1, &hueImage, 1, ch, 1);
	namedWindow("hueImage", WINDOW_NORMAL);
	imshow("hueImage", hueImage);

	//cout << "执行" << endl;
	//cout << image.channels() << endl;
	//cout << image << endl;
	Mat hist;
	const int channels[] = { 0 };
	int histSize = 10;
	float range[] = { 0,180};
	const float* histRange = { range };

	calcHist(&hueImage, 1, channels, Mat(), hist, 1, &histSize, &histRange, true, false);

	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	Mat dst;
	//image.copyTo(dst);
	calcBackProject(&hueImage,1, channels, hist, dst, &histRange, 1.0,true);

	namedWindow("反向投影图", WINDOW_NORMAL);

	imshow("反向投影图", dst);
	//cout << dst << endl;

	//cout << hist << endl;*/


	cvtColor(srcImage, hsv_Image, CV_BGR2HSV);
	namedWindow("H_image", WINDOW_NORMAL);
	namedWindow("S_image", WINDOW_NORMAL);
	namedWindow("V_image", WINDOW_NORMAL);

	H_image.create(srcImage.size(), srcImage.depth());
	S_image.create(srcImage.size(), srcImage.depth());
	V_image.create(srcImage.size(), srcImage.depth());

	////分离出h色彩空间
	int ch[][3] = { {0, 0},{1, 0},{2,0} };
	mixChannels(&hsv_Image,1, &H_image,1, ch[0],1);
	mixChannels(&hsv_Image,1, &S_image,1, ch[1],1);
	mixChannels(&hsv_Image,1, &V_image,1, ch[2],1);

	imshow("H_image", H_image);
	imshow("S_image", S_image);
	imshow("V_image", V_image);
	namedWindow("反向投影图", WINDOW_NORMAL);
	createTrackbar("反向投影图", "反向投影图", &global_bin, 180, onChangeBin);


	//normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
	


	waitKey();
	return 0;
}