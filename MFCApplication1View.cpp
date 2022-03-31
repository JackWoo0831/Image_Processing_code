
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include <math.h>
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMFCApplication1View::OnFileOpen)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_32771, &CMFCApplication1View::HistCompute)
	ON_COMMAND(ID_32772, &CMFCApplication1View::HistEqualization)
	ON_COMMAND(ID_32773, &CMFCApplication1View::SmoothFiltering)
	ON_COMMAND(ID_32774, &CMFCApplication1View::GaussianFiltering)
	ON_COMMAND(ID_32775, &CMFCApplication1View::Sobel)
	ON_COMMAND(ID_32776, &CMFCApplication1View::Erosion)
	ON_COMMAND(ID_32777, &CMFCApplication1View::Dilation)
	ON_COMMAND(ID_32778, &CMFCApplication1View::Open)
	ON_COMMAND(ID_32779, &CMFCApplication1View::Close)
	ON_COMMAND(ID_BMP_32780, &CMFCApplication1View::OnBmp)
	ON_COMMAND(ID_32781, &CMFCApplication1View::MidValueFilter)
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
	image = 0;
	outImg = 0;
	findex = 0;

	histFlag = 0;

	eqlFlag = 0;
	eqlImg = 0;

	MeanFilterFlag= 0;//均值
	meanFilterImg= 0;

	gaussianFlag = 0;//高斯低通
	gaussianImg = 0;

	midFitlerFlag =0; //中值
	midValueImg = 0;

	sobelFlag = 0; //边缘
	sob_x = 0;
	sob_y = 0;
	outSobelx = 0;
	outSobely = 0;

	erosionFlag = 0; //腐蚀
	erosionImg = 0;

	dilationFlag = 0; //膨胀
	dilationImg = 0;

	openFlag = 0; //开运算
	openImg = 0;

	closeFlag = 0;//闭运算
	closeImg = 0;
}

CMFCApplication1View::~CMFCApplication1View()
{
	if (image)
		delete image;
	if (outImg)
		delete outImg;
	if (meanFilterImg)
		delete meanFilterImg;
	if (gaussianImg)
		delete gaussianImg;
	if (midValueImg)
		delete midValueImg;
	if (outSobelx)
		delete outSobelx;
	if (outSobely)
		delete outSobely;
	if (erosionImg)
		delete erosionImg;
	if (dilationImg)
		delete dilationImg;
}

// CMFCApplication1View 绘图

void CMFCApplication1View::OnDraw(CDC* pDC/*pDC*/)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (image == 0)
		return;

	int i, j;
	BYTE gray;
	if (rawflag == 1 || flag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = image[i * width + j];
				//显示原图像
				pDC->SetPixel(j, i, RGB(gray, gray, gray));
			}
		}
	}

	if (bmpflag == 1)
	{
		BYTE r, g, b;
		for (i = 0; i < height; i++)
			for (j = 0; j < 3 * width; j = j + 3) 
			{
				b = rgbimg[i * 3 * width + j];
				g = rgbimg[i * 3 * width + j + 1];
				r = rgbimg[i * 3 * width + j + 2];
				//显示bmp彩色图像
				pDC->SetPixelV(j / 3, i, RGB(r, g, b));
			}
	}
	
	if (histFlag == 1)
	{
		//画图像直方图
		for (i = 0; i < 256; i++)
		{
			pDC->MoveTo(width + 30 + 4 * i, height + 50);
			pDC->LineTo(width + 30 + 4 * i, height + 50 - hist[i] / 6.);
		}

		histFlag = 0;
	}

	if (eqlFlag == 1)
	{
		//显示直方图均衡后图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = eqlImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		eqlFlag = 0;
	}

	if (MeanFilterFlag == 1)
	{
		//显示均值滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = meanFilterImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		MeanFilterFlag = 0;
	}
	
	if (gaussianFlag == 1)
	{
		//显示高斯滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = gaussianImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		gaussianFlag = 0;
	}

	if (midFitlerFlag == 1)
	{
		//显示中值滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = midValueImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		midFitlerFlag = 0;
	}



	if (sobelFlag == 1)
	{
		//显示边缘图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = outSobelx[i * width + j];
				pDC->SetPixel(j + (width + 10), i, RGB(gray, gray, gray));

				gray = outSobely[i * width + j];
				pDC->SetPixel(j + (width + 10) * 2, i, RGB(gray, gray, gray));

			}
		}
		sobelFlag = 0;
	}

	if (erosionFlag == 1)
	{
		//显示腐蚀图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = erosionImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		erosionFlag = 0;
	}

	if (dilationFlag == 1)
	{
		//显示膨胀图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = dilationImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		dilationFlag = 0;
	}

	if (openFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = openImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		openFlag = 0;
	}

	if (closeFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = closeImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		closeFlag = 0;
	}
}

//点击打开文件的操作，调用下面的函数。获取图像路径，文件名
void CMFCApplication1View::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog MyFDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);

	MyFDlg.m_ofn.lpstrFilter = "(*.lst;*.raw;*.bmp)\0*.lst;*.raw;*.bmp\0";
	MyFDlg.m_ofn.lpstrInitialDir = "D:\images";

	flag = 0;
	bmpflag = 0;
	rawflag = 0;

	if (MyFDlg.DoModal() == IDOK)
	{
		CString SFName;

		SFName = MyFDlg.GetPathName(); //full name with path

		char* fnstr;
		fnstr = SFName.GetBuffer(4); //read the name from string 

		FILE* fpLst;
		int n;

		int len;
		len = strlen(fnstr);
		if (!strcmp(fnstr + len - 3, "raw")) //single raw image显示单幅图像
		{
			rawflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name
		}
		if (!strcmp(fnstr + len - 3, "lst")) //list file显示多幅图像
		{
			flag = 1;
			fpLst = fopen(fnstr, "rb");
			fscanf(fpLst, "%3d", &fnum);
			fscanf(fpLst, "%s", directory);//directory

			fnames = new char[fnum * 100];
			for (n = 0; n < fnum; n++)
				fscanf(fpLst, "%s", fnames + n * 100);// image names

			fclose(fpLst);
		}
		if (!strcmp(fnstr + len - 3, "bmp"))//显示bmp格式图像
		{
			bmpflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name

			
		}

		//打开图像文件
		findex = 0;
		readImg(findex);
	}
}

void CMFCApplication1View::readImg(int findex)
{

	char fullName[120];
	sprintf(fullName, "%s\\%s", directory, fnames + findex * 100);

	FILE* fpImg;
	fpImg = fopen(fullName, "rb");
	if (fpImg == 0)
	{
		AfxMessageBox("Cannot open the list file", MB_OK, 0);
		return;
	}

	if (bmpflag == 1)//读取bmp格式图像
	{
		int bicount;
		BITMAPFILEHEADER bmpFHeader;
		BITMAPINFOHEADER bmiHeader;

		fread(&bmpFHeader, sizeof(BITMAPFILEHEADER), 1, fpImg);
		fread(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fpImg);

		width = bmiHeader.biWidth;
		height = bmiHeader.biHeight;
		bicount = bmiHeader.biBitCount;

		BYTE* rowBuff;
		int rowLen;

		rowLen = ((width * bicount) / 8 + 3) / 4 * 4;
		rowBuff = new BYTE[rowLen];

		if (bicount == 8)
		{
			RGBQUAD bmiColors[256];
			fread(bmiColors, sizeof(RGBQUAD), 256, fpImg);
			image = new BYTE[width * height];
			rgbimg = new BYTE[3 * width * height];
			for (int i = height - 1; i >= 0; i--)
			{
				fread(rowBuff, 1, rowLen, fpImg);
				memcpy(image + i * width, rowBuff, width);
			}
			for (int i = 0; i < height; i++)
				for (int j = 0; j < 3 * width; j = j + 3)
				{
					rgbimg[i * 3 * width + j] = bmiColors[image[i * width + j / 3]].rgbBlue;
					rgbimg[i * 3 * width + j + 1] = bmiColors[image[i * width + j / 3]].rgbGreen;
					rgbimg[i * 3 * width + j + 2] = bmiColors[image[i * width + j / 3]].rgbRed;
				}
		}
		if (bicount == 24)
		{
			image = new BYTE[width * height];
			rgbimg = new BYTE[rowLen * height];
			for (int i = height - 1; i >= 0; i--)
			{
				fread(rowBuff, 1, rowLen, fpImg);
				memcpy(rgbimg + i * 3 * width, rowBuff, 3 * width);
			}
		}
		delete rowBuff;
	}

	if (rawflag == 1 || flag == 1)
	{
		fread(&width, sizeof(int), 1, fpImg);
		fread(&height, sizeof(int), 1, fpImg);
		if (image)
			delete image;

		image = new BYTE[width * height];

		fread(image, sizeof(BYTE), width * height, fpImg);
	}

	fclose(fpImg);

	////////////////////////////////////////////////////////////
	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//计算图像直方图
void CMFCApplication1View::HistCompute()
{
	// TODO: 在此添加命令处理程序代码

	histCompute(image, width, height);
	histFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//直方图均衡
void CMFCApplication1View::HistEqualization()
{
	// TODO: 在此添加命令处理程序代码
	if (eqlImg)
		delete eqlImg;
	eqlImg = new BYTE[width * height]; //存储直方图均衡之后的图像
	hisEqualiz(image, width, height, eqlImg);//直方图均衡函数
	eqlFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//均值滤波
void CMFCApplication1View::SmoothFiltering()
{
	if (meanFilterImg)//均值滤波
		delete meanFilterImg;
	meanFilterImg = new BYTE[width * height];
	meanFilter(image, width, height, meanFilterImg);//均值滤波函数
	MeanFilterFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//高斯低通滤波
void CMFCApplication1View::GaussianFiltering()
{
	if (gaussianImg)
		delete gaussianImg;
	gaussianImg = new BYTE[width * height];
	gaussian(image, width, height, gaussianImg); //高斯低通滤波函数
	gaussianFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);

}

//中值滤波
void CMFCApplication1View::MidValueFilter()
{
	if (midValueImg)
		delete midValueImg;
	midValueImg = new BYTE[width * height];

	midFindFiltering(image, width, height, midValueImg);
	midFitlerFlag=1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Sobel()
{
	// TODO: 在此添加命令处理程序代码
	if (sob_x)
		delete sob_x;
	sob_x = new int[width * height];

	if (sob_y)
		delete sob_y;
	sob_y = new int[width * height];

	if (outSobelx)
		delete outSobelx;
	outSobelx = new BYTE[width * height];//存储图像水平边缘

	if (outSobely)
		delete outSobely;
	outSobely = new BYTE[width * height];//存储图像垂直边缘

	sobel(image, width, height, sob_x, sob_y);
	tranByte(sob_x, outSobelx, width, height);//将图像水平边缘值映射至0-255，用于显示
	tranByte(sob_y, outSobely, width, height);
	sobelFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Erosion()
{
	// TODO: 在此添加命令处理程序代码
	if (erosionImg)
		delete erosionImg;
	erosionImg = new BYTE[width * height];

	BYTE* temp;
	temp = new BYTE[width * height];

	memcpy(temp, image, sizeof(BYTE) * width * height);
	erosion(temp, width, height, erosionImg);

	delete temp;
	erosionFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Dilation()
{
	// TODO: 在此添加命令处理程序代码
	if (dilationImg)
		delete dilationImg;
	dilationImg = new BYTE[width * height];

	BYTE* temp;
	temp = new BYTE[width * height];

	memcpy(temp, image, sizeof(BYTE) * width * height);
	dilation(temp, width, height, dilationImg);

	delete temp;
	dilationFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Open()
{
	// TODO: 在此添加命令处理程序代码
	if (openImg)
		delete openImg;
	openImg = new BYTE[width * height];

	BYTE* tempI;
	tempI = new BYTE[width * height];
	BYTE* tempO;
	tempO = new BYTE[width * height];

	memcpy(tempI, image, sizeof(BYTE) * width * height);
	erosion(tempI, width, height, tempO);
	dilation(tempO, width, height, openImg);

	delete tempI;
	delete tempO;
	openFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Close()
{
	// TODO: 在此添加命令处理程序代码
	if (closeImg)
		delete closeImg;
	closeImg = new BYTE[width * height];

	BYTE* tempI;
	tempI = new BYTE[width * height];
	BYTE* tempO;
	tempO = new BYTE[width * height];

	memcpy(tempI, image, sizeof(BYTE) * width * height);
	dilation(tempI, width, height, tempO);
	erosion(tempO, width, height, closeImg);

	delete tempI;
	delete tempO;
	closeFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


//////////////////////////////////   图像处理函数   ///////////////////////////////

void CMFCApplication1View::histCompute(BYTE*image, int width, int height)
{
	//计算直方图
	// 初始化hist
	for (int n = 0; n < 256; n++) {
		hist[n] = 0;
	}
	int i, j; 
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			hist[image[i * height + j]]++; // 相应灰度+1
		}
	}

}

void CMFCApplication1View::hisEqualiz(BYTE* image, int w, int h, BYTE* outImg)
{
	//直方图均衡
	// 首先计算直方图
	int thist[256] = { 0 };
	int i, j;
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			thist[image[i * h + j]]++; // 相应灰度+1
		}
	}

	// 计算新的直方图 并构造灰度映射
	int nhist[256] = { 0 }; // 新直方图
	int grayMap[256] = { 0 }; // 旧图像灰度到新图像灰度的映射, 索引表示旧图像的灰度值, 值表示新图像的灰度值

	int nIndex = 0, tempSum = 0; // 辅助变量
	const float coe = 256 / static_cast<float>(h * w); // 计算系数
	for (i = 0; i < 256; i++) {
		tempSum += thist[i];
		nIndex = int(coe * tempSum);
		if (nIndex <= 255)
			grayMap[i] = nIndex; // i -> nIndex
		else
			grayMap[i] = 255;
		// nhist[nIndex] += thist[i];
	}
	// 根据 i -> nIndex 的映射输出新图像
	for (i = 0; i < h; i++) {
		for (j = 0;j < w; j++) {
			outImg[i * h + j] = static_cast<unsigned char> (grayMap[image[i * h + j]]);
		}
	}
}


void CMFCApplication1View::meanFilter(BYTE* image, int width, int height, BYTE* outImg)
{
	/*
	image: 输入图像 一维数组
	width: 图像宽度
	height: 高度
	outImg: 输出图像 一维数组
	*/
	//均值滤波

	
	const int edge = 5; // 边长
	const int size = edge * edge; // 3x3卷积核
	// 定义均值卷积核
	int kernel[size] = { 0 };
	for (int i = 0; i < size; i++) {
		kernel[i] = 1;
	}

	int padding = int(edge / 2); // 计算需要padding的大小 为edge对2的整数除法
	int widthNew = width + 2 * padding;
	int heightNew = height + 2 * padding;
	const int sizeOfImagePadding = widthNew * heightNew; // padding之后的size

	BYTE* imagePadding = new BYTE[sizeOfImagePadding]; // padding之后的图片
	// 计算padding之后的图片
	for (int row = 0; row < heightNew; row++) {
		for (int col = 0; col < widthNew; col++) {
			
			if (row < padding || col < padding) // 左、上方padding赋0
				imagePadding[row * widthNew + col] = 0;
			else if (row > height - 1 + padding || col > width - 1 + padding) // 右 下方padding赋0 
				imagePadding[row * widthNew + col] = 0;
			else // 其余不动
				imagePadding[row * widthNew + col] = image[(row - padding) * width + (col - padding)];
		}
	}

	BYTE* block = new BYTE[size]; // 定义运算块
	// 开始计算卷积
	for (int row = padding; row < heightNew - padding; row++) {
		for (int col = padding; col < widthNew - padding; col++) {

			// 先计算block
			for (int itemp = 0; itemp < edge; itemp++) {
				for (int jtemp = 0; jtemp < edge; jtemp++) {
					block[itemp * edge + jtemp] = imagePadding[(row - padding + itemp) * widthNew + (col - padding + jtemp)];
				}
			}

			// 计算卷积

			outImg[(row - padding) * width + (col - padding)] = convolution(kernel, size, block);
		}
	}

	delete[] imagePadding;
	delete[] block; 

} 


int CMFCApplication1View::convolution(int* operatr, int size, BYTE* block)
{
	/*
	operatr: 算子 一维数组
	size: size x size卷积核 假设为正方形
	block: 与算子对应的块 一维 unsigned char[]
	*/
	int value = 0; // 结果
	int i = 0; // 循环变量

	int elem = 0; // 归一化因子 为kernel各元素和

	// 卷积运算
	for (i = 0; i < size; i++) {
		elem += operatr[i];
		value += operatr[i] * block[i];
	}

	
	value = int(value / elem);
	return value;

}

void CMFCApplication1View::gaussian(BYTE* image, int width, int height, BYTE* outImg)
{
	//高斯滤波
	const float pi = 3.1415; // pi
	const float sigma = 1; // 方差
	const int edge = 7; // 核边长
	const int size = edge * edge; // 核尺寸


	// 首先计算高斯核
	int padding = int(edge / 2); // 计算需要padding的大小 为edge对2的整数除法
	double gaussianKernel[size] = { 0 };
	for (int i = 0; i < edge; i++) {

		for (int j = 0; j < edge; j++) {
			double part1 = pow((i - padding), 2), part2 = pow((j - padding), 2);
			double g = exp(-(part1 + part2) / 2 * pow(sigma, 2));
			g /= 2 * pi * pow(sigma, 2);
			gaussianKernel[i * edge + j] = g;
		}
	}

	int gaussianKernelInt[size] = { 0 }; // 转为整数核 因此要 归一化 左上角系数必须为1
	double firstElem = 1 / gaussianKernel[0]; // 归一化系数
	// 开始归一化
	for (int i = 0; i < edge; i++) {
		for (int j = 0; j < edge; j++) {
			gaussianKernelInt[i * edge + j] = int(gaussianKernel[i * edge + j] * firstElem);
		}
	}

	// 其余步骤和均值滤波相同
	int widthNew = width + 2 * padding;
	int heightNew = height + 2 * padding;
	const int sizeOfImagePadding = widthNew * heightNew; // padding之后的size

	BYTE* imagePadding = new BYTE[sizeOfImagePadding]; // padding之后的图片
	// 计算padding之后的图片
	for (int row = 0; row < heightNew; row++) {
		for (int col = 0; col < widthNew; col++) {

			if (row < padding || col < padding) // 左、上方padding赋0
				imagePadding[row * widthNew + col] = 0;
			else if (row > height - 1 + padding || col > width - 1 + padding) // 右 下方padding赋0 
				imagePadding[row * widthNew + col] = 0;
			else // 其余不动
				imagePadding[row * widthNew + col] = image[(row - padding) * width + (col - padding)];
		}
	}

	BYTE* block = new BYTE[size]; // 定义运算块
	// 开始计算卷积
	for (int row = padding; row < heightNew - padding; row++) {
		for (int col = padding; col < widthNew - padding; col++) {

			// 先计算block
			for (int itemp = 0; itemp < edge; itemp++) {
				for (int jtemp = 0; jtemp < edge; jtemp++) {
					block[itemp * edge + jtemp] = imagePadding[(row - padding + itemp) * widthNew + (col - padding + jtemp)];
				}
			}

			// 计算卷积

			outImg[(row - padding) * width + (col - padding)] = convolution(gaussianKernelInt, size, block);
		}
	}

	delete[] imagePadding;
	delete[] block;


}


void CMFCApplication1View::midFindFiltering(BYTE* image, int width, int height, BYTE* outImg)
{
	//中值滤波
	const int edge = 3; // 边长
	const int size = edge * edge; // size
	BYTE* block = new BYTE[size]; // 中值滤波不需要额外的kernel 直接初始化block


	int bound = int(edge / 2); 

	// 其余过程和均值滤波相同
	// TODO: 可以不padding, 边缘的点只有部分点参与计算会更准确

	int padding = int(edge / 2); // 计算需要padding的大小 为edge对2的整数除法
	int widthNew = width + 2 * padding;
	int heightNew = height + 2 * padding;
	const int sizeOfImagePadding = widthNew * heightNew; // padding之后的size

	BYTE* imagePadding = new BYTE[sizeOfImagePadding]; // padding之后的图片
	// 计算padding之后的图片
	
	for (int row = 0; row < heightNew; row++) {
		for (int col = 0; col < widthNew; col++) {

			if (row < padding || col < padding) // 左、上方padding赋0
				imagePadding[row * widthNew + col] = 0;
			else if (row > height - 1 + padding || col > width - 1 + padding) // 右 下方padding赋0 
				imagePadding[row * widthNew + col] = 0;
			else // 其余不动
				imagePadding[row * widthNew + col] = image[(row - padding) * width + (col - padding)];
		}
	}

	for (int row = 0; row < heightNew; row++) {
		for (int col = 0; col < widthNew; col++) {

			// 先计算block
			for (int itemp = 0; itemp < edge; itemp++) {
				for (int jtemp = 0; jtemp < edge; jtemp++) {
					block[itemp * edge + jtemp] = imagePadding[(row - padding + itemp) * widthNew + (col - padding + jtemp)];
				}
			}


			outImg[(row - padding) * width + (col - padding)] = MidValueFind(size, block);
			// outImg[(row - padding) * width + (col - padding)] = 0;
		}
	}
	
	// delete[] imagePadding;
	delete[] block;
}



int CMFCApplication1View::MidValueFind(int num, BYTE* d)
{
	int value = 0;

	int i, j;
	int temp;
	for (i = 0; i < num - 1; i++)
		for (j = i + 1; j < num; j++)
		{
			if (d[i] < d[j])
			{
				temp = d[i];
				d[i] = d[j];
				d[j] = temp;
			}
		}

	return d[num / 2];

}


void CMFCApplication1View::sobel(BYTE* window, int width, int height, int* sob_x, int* sob_y)
{
	/*
	一阶差分 Sobel算子
	window: 输入图像
	wid, hei: 高 宽
	sob_x, sob_y: Sobel算子运算后的x梯度和y梯度 
	*/

	// 定义Sobel核
	int so_x[9] = {-1, 0, 1,
	               -2, 0, 2,
				   -1, 0, 1}; //horizontal

	int so_y[9] = {-1, -2, -1,
	                0,  0,  0,
	                1,  2,  1};

	// 将Sobel核与图像作卷积 此处不再padding
	// BYTE* block = new BYTE[9];
	BYTE block[9] = { 0 };

	const int edge = 3;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++){

			if (row == 0 || col == 0 || row == (height - 1) || col == (width - 1)) {
				sob_x[row * width + col] = 0;
				sob_y[row * width + col] = 0;
			}

			else {
				// 计算block
				for (int i = 0; i < edge; i++) {
					for (int j = 0; j < edge; j++) {
						block[i * edge + j] = window[(row - 1 + i) * width + (col - 1 + j)];

					}
				}

				// 计算卷积
				sob_x[row * width + col] = convolution_without_norm(so_x, 9, block);
				sob_y[row * width + col] = convolution_without_norm(so_y, 9, block);
			}

		}
	}

	// delete[] block;

}

int CMFCApplication1View::convolution_without_norm(int* operatr, int size, BYTE* block) {
	/*
	operatr: 算子 一维数组
	size: size x size卷积核 假设为正方形
	block: 与算子对应的块 一维 unsigned char[]
	*/
	int value = 0; // 结果
	int i = 0; // 循环变量


	// 卷积运算
	for (i = 0; i < size; i++) {
		value += operatr[i] * block[i];
	}

	return value;
}

void CMFCApplication1View::erosion(BYTE* image, int w, int h, BYTE* outImg)
{
	/*
	腐蚀 针对二值图像 采用全1 3x3kernel
	*/
	
	const int repeatTimes = 5; // 重复次数
	memcpy(outImg, image, sizeof(BYTE) * w * h); // 先把image赋给outImg

	const int eroThresh = 2; // 邻域最多允许有多少0

	for (int repeat = 0; repeat < repeatTimes; repeat++) {
		/*
		相对于之前kernel移动的方式 此处逆向做 遍历图中的每一个点 如果该点邻域全都是255 则该点仍为255 否则设为0
		实际中便于使用 可以不这么严格 设定一个阈值, 来规定邻域最多允许有多少0
		*/

		for (int row = 0; row < h; row++){
			for (int col = 0; col < w; col++) {

				if (image[row * w + col] == 255) { // 如果是255(ROI)

					int zeroCount = 0;
					for (int itemp = -1; itemp < 2; itemp++) {  // 遍历邻域 从-1开始

						for (int jtemp = -1; jtemp < 2;jtemp++) {
							if (image[(row + itemp) * w + (col + jtemp)] == 0)
								zeroCount++;

						}
					}

					if (zeroCount > eroThresh)
						outImg[row * w + col] = 0; // 多于允许阈值 归为背景类

				}


			}
		}
	}


}

void CMFCApplication1View::dilation(BYTE* image, int w, int h, BYTE* outImg)
{
	/*
	膨胀做法和腐蚀完全一样 只需要遍历是背景类的点 如果邻域包含多于阈值的前景(ROI)点
	则该点归为前景
	*/
	const int repeatTimes = 5; // 重复次数
	memcpy(outImg, image, sizeof(BYTE) * w * h); // 先把image赋给outImg

	const int dilaThresh = 2; // 邻域最多允许有多少0

	for (int repeat = 0; repeat < repeatTimes; repeat++) {

		for (int row = 0; row < h; row++) {
			for (int col = 0; col < w; col++) {

				if (image[row * w + col] == 0) { // 如果是背景

					int roiCount = 0;
					for (int itemp = -1; itemp < 2; itemp++) {  // 遍历邻域 从-1开始

						for (int jtemp = -1; jtemp < 2;jtemp++) {
							if (image[(row + itemp) * w + (col + jtemp)] == 255)
								roiCount++;

						}
					}

					if (roiCount > dilaThresh)
						outImg[row * w + col] = 255; // 多于允许阈值 归为背景类

				}


			}
		}
	}

}


/////////////// 功能函数 ///////////////////////////////////////////////////////
void CMFCApplication1View::tranByte(int* temp, BYTE* array, int w, int h)
{
	int dim;
	dim = w * h;

	double stm, stvar;
	stm = 120;
	stvar = 60;
	int i;

	double mean = 0;
	for (i = 0; i < dim; i++)
		mean += temp[i];
	mean /= dim;//mean value

	double varia = 0;
	double diff;
	for (i = 0; i < dim; i++)
	{
		diff = temp[i] - mean;
		varia += diff * diff;
	}
	varia /= dim;//varianve

	if (varia < 0.0001)
		return;
	double ratio;
	ratio = stvar / sqrt(varia);

	BYTE value;
	for (i = 0; i < dim; i++)
	{
		value = (BYTE)((temp[i] - mean) * ratio + stm);

		if (value >= 200)
			array[i] = 200;
		else if (value < 80)
			array[i] = 80;
		else
			array[i] = value;
	}
}


void CMFCApplication1View::OnBmp()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog MyFDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);

	MyFDlg.m_ofn.lpstrFilter = "(*.bmp)\0*.bmp\0";
	MyFDlg.m_ofn.lpstrInitialDir = "D:\\";

	bmpflag = 0;

	if (MyFDlg.DoModal() == IDOK)
	{
		CString SFName;

		SFName = MyFDlg.GetPathName(); //full name with path

		char* fnstr;
		fnstr = SFName.GetBuffer(4); //read the name from string 

		FILE* fpLst;
		int n;

		int len;
		len = strlen(fnstr);

		if (!strcmp(fnstr + len - 3, "bmp"))
		{
			bmpflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name


		}
		findex = 0;
		readImg(findex);
	}
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 打印

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG



void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_NEXT)
	{
		if (findex < fnum)
		{
			findex++;
			readImg(findex);
		}
	}
	if (nChar == VK_PRIOR)
	{
		if (findex > 0)
		{
			findex--;
			readImg(findex);
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

