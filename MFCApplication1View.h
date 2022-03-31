
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once


class CMFCApplication1View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 特性
public:
	CMFCApplication1Doc* GetDocument() const;

// 操作
public:

	BYTE * image;
	int width, height;

	int findex, fnum;
	int bmpflag, rawflag, flag;
	char* fnames, directory[120];

	void readImg(int);

	BYTE* outImg;
	BYTE* rgbimg;

	
	// 计算图像直方图
	int histFlag;
	int hist[256]; //存储图像直方图，256灰度级
	void histCompute(BYTE*, int, int);//计算图像直方图函数


	//直方图均衡
	int eqlFlag;
	BYTE* eqlImg;  //存储直方图均衡后的图像
	void hisEqualiz(BYTE*, int, int, BYTE*);//直方图均衡函数
	
	//均值滤波
	int MeanFilterFlag;
	BYTE* meanFilterImg; // 存储均值滤波后的图像
	void meanFilter(BYTE*, int, int, BYTE*); // 均值滤波器
	int convolution(int* operatr, int size, BYTE* block); //卷积函数
	
	
	//高斯滤波
 	int gaussianFlag;
	BYTE* gaussianImg; // 存储高斯滤波后的图像
	void gaussian(BYTE*, int, int, BYTE*);// 高斯滤波器

	//中值滤波
	int midFitlerFlag;
	BYTE* midValueImg;
	void midFindFiltering(BYTE*, int, int, BYTE*);
	int MidValueFind(int,BYTE*);

	//边缘提取
	int sobelFlag;
	int* sob_x, * sob_y;
	BYTE* outSobelx, * outSobely;
	void sobel(BYTE*, int, int, int*, int*);
	void tranByte(int*, BYTE*, int, int);
	int convolution_without_norm(int* operatr, int size, BYTE* block); // 不含归一化的卷积 用于梯度算子

	//形态学处理
	int erosionFlag;
	BYTE* erosionImg;
	void erosion(BYTE*, int, int, BYTE*);

	int dilationFlag;
	BYTE* dilationImg;
	void dilation(BYTE*, int, int, BYTE*);

	int openFlag;
	BYTE* openImg;

	int closeFlag;
	BYTE* closeImg;




// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void HistCompute();
	afx_msg void HistEqualization();
	afx_msg void SmoothFiltering();
	afx_msg void GaussianFiltering();
	afx_msg void Sobel();
	afx_msg void Erosion();
	afx_msg void Dilation();
	afx_msg void Open();
	afx_msg void Close();
	afx_msg void OnBmp();
	afx_msg void MidValueFilter();
};

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

