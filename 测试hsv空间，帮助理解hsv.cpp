#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

string num2string(int i)
{
	char ss[5];
	sprintf_s(ss, "%04d", i);
	return ss;
}

void createImage()
{
	Mat image(Size(1280, 480), CV_32FC3, Scalar(255, 255, 255));


	int movecell = 5;
	string base_image_path = "E:/images/Rectangle";

	for (int i = 1; i < 230; i++)
	{
		rectangle(image, Point(50 + i * 5, 50), Point(100 + i * 5, 60), Scalar(0, 0, 0), -1, 4);

		rectangle(image, Point(50 + (i - 1) * 5, 50), Point(50 + i * 5, 60), Scalar(255, 255, 255), -1, 4);
		string image_path = base_image_path + "/" + num2string(i) + ".jpg";
		imwrite(image_path, image);
	}
}

void Image2Video()
{
	int fps = 30;
	VideoWriter write("E:/output3.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(1280, 480), true);
	for (int i = 1; i < 230; i++)
	{
		string image_path = "E:/images/Rectangle/" + num2string(i) + ".jpg";
		Mat image = imread(image_path);
		if (image.empty())
		{
			cout << "图像加载失败" << endl;
		}
		cout << i << endl;
		write.write(image);
	}
	write.release();
}

Mat createTempImage()
{
	Mat image(Size(900, 900), CV_8UC3);

	rectangle(image, Rect(Point(0, 0), Point(300, 300)), Scalar(255, 0, 0), -1, 4);
	rectangle(image, Rect(Point(0, 301), Point(300, 600)), Scalar(0, 255, 0), -1, 4);
	rectangle(image, Rect(Point(0, 601), Point(300, 900)), Scalar(0, 0, 255), -1, 4);

	rectangle(image, Rect(Point(301, 0), Point(600, 300)), Scalar(255, 255, 0), -1, 4);
	rectangle(image, Rect(Point(301, 301), Point(600, 600)), Scalar(255, 0, 255), -1, 4);
	rectangle(image, Rect(Point(301, 601), Point(600, 900)), Scalar(0, 255, 255), -1, 4);

	rectangle(image, Rect(Point(601, 0), Point(900, 300)), Scalar(0, 0, 0), -1, 4);
	rectangle(image, Rect(Point(601, 301), Point(900, 600)), Scalar(155, 155, 155), -1, 4);
	rectangle(image, Rect(Point(601, 601), Point(900, 900)), Scalar(75, 0, 75), -1, 4);
	return image;
}

Mat gradual_change()
{
	Mat image(Size(1080,50),CV_8UC3,Scalar(0,0,0));
	Mat hsv_image;

	cvtColor(image, hsv_image, CV_BGR2HSV_FULL);

	int channels[] = { 0,0 };
	
	Mat dst(hsv_image.size(),hsv_image.depth());
	mixChannels(hsv_image, dst, channels,1);

	for (int i = 1; i < 360; i++)
		rectangle(dst, Rect(Point((i - 1) * 3, 0),Point(i * 3, 50)), Scalar(i), -1, 4);

	return dst;
}

int main()
{
	Mat gradual_image = gradual_change();

	imshow("image", gradual_image);

	Mat srcImage = createTempImage();
	imshow("原始图像", srcImage);

	Mat hsvImage;
	cvtColor(srcImage, hsvImage, CV_BGR2HSV);
	Mat hImage = Mat(srcImage.size(), srcImage.depth());
	Mat sImage = Mat(srcImage.size(), srcImage.depth());
	Mat vImage = Mat(srcImage.size(), srcImage.depth());

	int chh[] = { 0,0 };
	int chs[] = { 1,0 };
	int chv[] = { 2,0 };

	mixChannels(hsvImage, hImage, chh,1);
	mixChannels(hsvImage, sImage, chs, 1);
	mixChannels(hsvImage, vImage, chv, 1);

	imshow("hImage", hImage);
	imshow("sImage", sImage);
	imshow("vImage", vImage);

	waitKey();


	return 0;
}