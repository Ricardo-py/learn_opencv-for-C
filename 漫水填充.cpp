#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;

//全局变量声明
Mat g_srcImage, g_dstImage, g_grayImage, g_maskImage;
int g_nlowDifference = 20, g_nupDifference = 20;//负差、正差最大值
bool g_bIsColor = true;//是否为彩色标识符布尔值
bool g_bUseMask = false;//是否显示掩膜窗口的布尔值
int g_nFillMode = 1;//漫水填充模式
int g_nConnectivity = 4;//表示floodFill函数标识符第八位的连通值
int g_nNewMaskVal = 255;//新的重新绘制的像素值

//鼠标消息onMouse回调函数
static void onMouse(int event, int x, int y, int, void*)
{
	if (event != CV_EVENT_LBUTTONDOWN)//如果鼠标左键没有按下，便返回
		return;
	//调用floodFill函数之前的参数准备部分**********
	Point seed = Point(x, y);
	int lowDifference = g_nFillMode == 0 ? 0 : g_nlowDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nlowDifference
	int upDifference = g_nFillMode == 0 ? 0 : g_nupDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nupDifference
	//标识符的0～7位为g_nConnectivity，8～15位为g_nNewMaskVal左移8位的值，16～23位为CV_FLOODFILL_FIXED_RANGE或者0
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) + (g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
	//随机生成b、g、r的值
	int b = (unsigned)theRNG() & 255;//随即返回一个0~255之间的值
	int g = (unsigned)theRNG() & 255;//随即返回一个0~255之间的值
	int r = (unsigned)theRNG() & 255;//随即返回一个0~255之间的值

	Rect ccomp;//定义重绘区域的最小边界矩形区域
	//在重绘区域像素的新值，若是彩色图模式，取Scalar(b,g,r);若是灰度图模式，取Scalar(r*0.299+g*0.587+b*0.114)
	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r * 0.299 + g * 0.587 + b * 0.114);
	Mat dst = g_bIsColor ? g_dstImage : g_grayImage;//目标图的赋值

	int area;

	//正式调用floodFill函数**********************
	if (g_bUseMask)
	{
		threshold(g_maskImage, g_maskImage, 1, 128, CV_THRESH_BINARY);
		area = floodFill(dst, g_maskImage, seed, newVal, &ccomp, Scalar(lowDifference, lowDifference, lowDifference), Scalar(upDifference, upDifference, upDifference), flags);
		imshow("mask", g_maskImage);
	}
	else
	{
		area = floodFill(dst, seed, newVal, &ccomp, Scalar(lowDifference, lowDifference, lowDifference), Scalar(upDifference, upDifference, upDifference), flags);
	}
	imshow("Renderings", dst);
	cout << area << "个像素被重绘\n";

}
//main函数
int main()
{
	g_srcImage = imread("C:/Users/Richard/Desktop/timg.jpg");//载入源图
	if (!g_srcImage.data) { printf("读取图片srcImage错误～！\n"); return false; }

	g_srcImage.copyTo(g_dstImage);//复制源图到目标图
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);//转换三通道的image0到灰度图
	g_maskImage.create(g_srcImage.rows + 2, g_srcImage.cols + 2, CV_8UC1);//利用Image0的尺寸来初始化掩膜mask
	namedWindow("Renderings", CV_WINDOW_AUTOSIZE);
	createTrackbar("Low difference:", "Renderings", &g_nlowDifference, 255, 0);
	createTrackbar("Up difference:", "Renderings", &g_nupDifference, 255, 0);

	setMouseCallback("Renderings", onMouse, 0);//鼠标回调函数

	while (1)//循环轮询按键
	{
		imshow("Renderings", g_bIsColor ? g_dstImage : g_grayImage);//显示效果图

		int c = waitKey(0);//获取键盘按键
		if ((c & 255) == 27)//判断ESC是否按下，若按下便退出
		{
			cout << "程序退出......\n";
			break;
		}

		switch ((char)c)
		{
		case '1'://如果键盘‘1’被按下，效果图在灰度图，彩色之间互换
			if (g_bIsColor)//若原来为彩色，转换为灰度图，并且将掩膜mask所有元素设置为0
			{
				cout << "键盘'1'被按下，切换彩色／灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
				cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
				g_maskImage = Scalar::all(0);
				g_bIsColor = false;
			}
			else//若原来为灰度图，便将原来的彩色图在此赋值给image，并且将掩膜mask所有元素置0
			{
				cout << "键盘‘1’被按下， 切换彩色／灰度模式，当前操作为将【灰度模式】切换为【彩色模式】";
				g_srcImage.copyTo(g_dstImage);
				g_maskImage = Scalar::all(0);
				g_bIsColor = true;
			}
			break;
		case '2'://如果键盘‘2’被按下，显示／隐藏掩膜窗口
			if (g_bUseMask)
			{
				destroyWindow("mask");
				g_bUseMask = false;
			}
			else
			{
				namedWindow("mask", 0);
				g_maskImage = Scalar::all(0);
				imshow("mask", g_maskImage);
				g_bUseMask = true;
			}
			break;
		case '3'://如果键盘'3'被按下，恢复原始图像
			cout << "键盘‘3’被按下，恢复原始图像\n";
			g_srcImage.copyTo(g_dstImage);
			cvtColor(g_dstImage, g_grayImage, COLOR_BGR2GRAY);
			g_maskImage = Scalar::all(0);
			break;
		case '4'://如果键盘‘4’被按下，使用空范围的漫水填充
			cout << "键盘‘4’被按下，使用空范围的漫水填充\n";
			g_nFillMode = 0;
			break;
		case '5'://如果键盘‘5’被按下，使用渐变、固定范围的漫水填充
			cout << "键盘'5'被按下，使用渐变、固定范围的漫水填充\n";
			g_nFillMode = 1;
			break;
		case '6'://如果键盘‘6’被按下，使用渐变、浮动范围的漫水填充
			cout << "键盘‘6’被按下，使用渐变、浮动范围的漫水填充\n";
			g_nFillMode = 2;
			break;
		case '7'://如果键盘‘7’被按下，操作标识符的低八位使用4位的连接模式
			cout << "键盘‘7’被按下，操作标识符的低八位使用4位的连接模式\n";
			g_nConnectivity = 4;
			break;
		case '8'://如果键盘‘8’被按下，操作标识符的低八位使用8位的连接模式
			cout << "键盘‘8’被按下，操作标识符的低八位使用8位的连接模式\n";
			g_nConnectivity = 8;
			break;

		}

	}

	return 0;
}
