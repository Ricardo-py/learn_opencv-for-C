#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include <string>

#define INPUT_TITLE "input image"
#define OUTPUT_TITLE "直方图计算"

using namespace cv;
using namespace std;

string num2str(int i)
{
	char ss[5];
	sprintf_s(ss, "%04d", i);
	return ss;
}


void Image2Video()
{
	//Mat image = imread("E:/计算机视觉/Tracking_dataset/BlurBody集合/BlurBody/img/0001.jpg");
/*if (image.empty())
{
	cout << "图像加载失败" << endl;
	return -1;
}*/
	int isColor = 1;
	int fps = 30;
	//int image_width = image.rows;
	//int image_height = image.cols;
	//cout << image_width << endl;
	//cout << image_height << endl;
	namedWindow("window_normal", WINDOW_NORMAL);

	//特别注意这个Size，如果size不一致的话是写入不了视频的
	VideoWriter writer("E:/output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(640, 480), isColor);

	for (int i = 1; i < 335; i++)
	{
		string image_path = "E:/计算机视觉/Tracking_dataset/BlurBody集合/BlurBody/img/" + num2str(i) + ".jpg";
		cout << num2str(i) << endl;
		Mat temp_image = imread(image_path);

		if (temp_image.empty())
		{
			cout << "读入图像失败" << endl;
			break;
		}
		imshow("window_normal", temp_image);
		waitKey();
		writer.write(temp_image);
	}
	writer.release();

	//imshow("window_normal", image);

	//cout << num2str(21) << endl;
}

void Video2Image()
{
	Mat frame;
	char outfile[50];

	VideoCapture cap("E:/output.avi");
	if (!cap.isOpened())
	{
		cout << "打开视频失败!" << endl;
	}

	int totalFrame = cap.get(CV_CAP_PROP_FRAME_COUNT);
	for (int i = 1; i <= totalFrame; i++)
	{
		cap >> frame;
		if (frame.empty())
		{
			cout << "图片为空" << endl;
			break;
		}
		sprintf_s(outfile, "E:/images/%d.jpg", i);
		imwrite(outfile, frame);
	}
	cap.release();
	destroyAllWindows();
}

int main()
{
	Video2Image();
	waitKey();
	return 0;
}
