#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

#define INPUT_TITLE "input image"
#define OUTPUT_TITLE "直方图计算"

using namespace cv;
using namespace std;

int main()
{
	Mat srcImage = imread("C:/Users/Richard/Desktop/timg.jpg");
	if (srcImage.empty())
	{
		cout << "图像加载失败" << endl;
		return -1;
	}
	//imshow("原始图像", srcImage);
	Mat hsvImage;
	cvtColor(srcImage, hsvImage, CV_BGR2HSV_FULL);
	//imshow("hsv色彩图像", hsvImage);

	//测试：得到BGR某个通道的图像
	vector<Mat> mv;
	split(srcImage, mv);
	mv[0] = Scalar(0);   //蓝色通道为0
	mv[1] = Scalar(0);   //绿色通道为0
	Mat bgr_planes[3];
	merge(mv, bgr_planes[2]);      //得到红色通道的图像

	split(srcImage, mv);
	mv[0] = Scalar(0);
	mv[2] = Scalar(0);
	merge(mv, bgr_planes[1]);

	split(srcImage, mv);
	mv[2] = Scalar(0);
	mv[1] = Scalar(0);
	merge(mv, bgr_planes[0]);

	imshow("1", bgr_planes[0]);
	imshow("2", bgr_planes[1]);
	imshow("3", bgr_planes[2]);
	Mat planes0 = bgr_planes[0];
	Mat planes1 = bgr_planes[1];
	Mat planes2 = bgr_planes[2];

	//测试：得到HSV某个通道的图像
	/*split(hsvImage, mv);
	Mat H = mv.at(0);
	Mat S = mv.at(1);
	Mat V = mv.at(2);
	imshow("outputH", H);
	imshow("outputS", S);
	imshow("outputV", V);*/

	/*Mat srcimg = imread("C:/Users/Richard/Desktop/timg.jpg");
	Mat Hsvimg;
	cvtColor(srcimg, Hsvimg, COLOR_BGR2HSV);


	int huebinnum = 30;//色调量化
	int saturationbinnum = 32;  //饱和度量化


	int histsize[] = { huebinnum,saturationbinnum };


	float hueRange[] = { 0,180 };//色调的变化范围
	float saturationRange[] = { 0,256 };//饱和度的变化范围


	const float* ranges[] = { hueRange,saturationRange };


	MatND dstHist;   //MatND 储存直方图的一种数据结构


	int channels[] = { 0,1 };//计算第0，1通道的直方图


	calcHist(&Hsvimg, 1, channels, Mat(), dstHist, 2, histsize, ranges, true, false);
	//进行直方图计算


	//为绘制直方图准备参数
	double maxVaule = 0;
	minMaxLoc(dstHist, 0, &maxVaule, 0, 0);//查找dstHist全局最大最小值后存入maxVaule


	int scale = 10;


	Mat histImg = Mat::zeros(saturationbinnum * scale, huebinnum * 10, CV_8UC3);


	for (int hue = 0; hue < huebinnum; hue++)
		for (int saturation = 0; saturation < saturationbinnum; saturation++)
		{
			float binValue = dstHist.at<float>(hue, saturation);//直方图直条的值


			int intensity = cvRound(binValue * 255 / maxVaule);  //强度
			rectangle(histImg, Point(hue * scale, saturation * scale), Point((hue + 1) * scale - 1, (saturation + 1) * scale - 1), Scalar::all(intensity), CV_FILLED);

		}
	cvtColor(histImg, histImg, COLOR_HSV2BGR);
	imshow("hist", histImg);*/


	//可以对每个通道分别计算直方图后画出来
	//vector<Mat> v;
	//split(srcImage, v);
	//imshow("B", v[0]);
	namedWindow(INPUT_TITLE, CV_WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_TITLE, CV_WINDOW_AUTOSIZE);

	imshow(INPUT_TITLE, srcImage);

	//分通道显示
	//vector<Mat> bgr_planes;
	//split(src, bgr_planes);

	//设定像素取值范围
	int histSize = 256;
	float range[] = { 0,256 };
	const float* histRanges = { range };
	cout << "执行" << endl;

	//三个通道分别计算直方图
	Mat b_hist, g_hist, r_hist;
	int channels[] = {0};
	//calcHist(&planes0, 1, Mat(), b_hist, &histSize, &range, true);
	calcHist(&planes0, 1, channels, Mat(), b_hist, 1, &histSize, &histRanges, true, false);
	//cout << "执行2" << endl;
	calcHist(&planes1, 1, channels, Mat(), g_hist, 1, &histSize, &histRanges, true, false);
	calcHist(&planes2, 1, channels, Mat(), r_hist, 1, &histSize, &histRanges, true, false);


	//创建直方图画布并归一化处理
	int hist_h = 400;
	int hist_w = 512;
	int bin_w = hist_w / histSize;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

	//render histogram chart  在直方图画布上画出直方图
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point((i)* bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)* bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)* bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}


	imshow(OUTPUT_TITLE, histImage);

	waitKey();
	return 0;
}