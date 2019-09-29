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

int main()
{
	Image2Video();
	waitKey();

	return 0;
}