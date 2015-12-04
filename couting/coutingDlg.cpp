
// coutingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "couting.h"
#include "coutingDlg.h"
#include "afxdialogex.h"
#include "opencv2\opencv.hpp"
#include "CvvImage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;


CDC *pdc;
HDC hdc;
CWnd *pwnd;
CRect rect;
int FrameMAX;
CvCapture* pCapture = NULL;
IplImage* img=0;
CvPoint* PointArr=NULL;
int size;
static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;
const char* cascade_name ="cascades.xml"; 
int current=0;
IplImage* mask;
int PedesNum=0;
CvFont font;






// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcoutingDlg 对话框




CcoutingDlg::CcoutingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcoutingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcoutingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcoutingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CcoutingDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_BUTTON2, &CcoutingDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_BUTTON3, &CcoutingDlg::OnBnClickedDetect)
END_MESSAGE_MAP()


// CcoutingDlg 消息处理程序

BOOL CcoutingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcoutingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcoutingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcoutingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcoutingDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
		//KillTimer(1);
		cvInitFont( &font, CV_FONT_VECTOR0,1, 1, 0, 1, 8);
		CFileDialog openavi(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("视频文件(*.avi)|*.avi|视频文件(*.mp4)|*.mp4|所有文件(*.*)|*.*||"));
		openavi.DoModal();
		CString avipath=openavi.GetPathName();
		if(avipath.IsEmpty())
		{
			MessageBox(_T("未选中视频文件"),_T("提示"));
			return;
		}
		char filename[300];
		int len=WideCharToMultiByte(CP_ACP,0,avipath,avipath.GetLength(),NULL,0,NULL,NULL);
		WideCharToMultiByte(CP_ACP,0,avipath,avipath.GetLength(),filename,len,NULL,NULL);
		filename[len] = '\0';
		CString s(filename);
		if(pCapture = cvCaptureFromFile(filename))
		{
			pwnd=GetDlgItem(IDC_STATIC);  
			pdc =pwnd->GetDC();
			hdc=pdc->GetSafeHdc();         
			GetDlgItem(IDC_STATIC)->GetClientRect(rect);   
			FrameMAX=(int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);
			SetTimer(1,25,NULL);
		}else
		{
			MessageBox(_T("视频文件无法正常打开"),_T("提示"));
		}

}
void CcoutingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1==nIDEvent)
	{  
		img=cvQueryFrame(pCapture); //从摄像头或者文件中抓取并返回一帧
		current++;
		if(img!=NULL)
		{
			IplImage *interimg=cvCloneImage(img);
			if(PointArr!=NULL)
			{
			//cvFillConvexPoly(img,PointArr,size,cvScalarAll(255),CV_AA,0);
			for( int j = 0; j < size; j++ )
				{ 
					cvLine( interimg, PointArr[j],  PointArr[(j+1)%size], cvScalarAll( 255 ), 2, 8 );
				}
			}
			char str[11];
			sprintf(str,"%d",(PedesNum+1));
			cvPutText(interimg, str,cvPoint(interimg->width-50,50), &font,cvScalar(0,0,255));
			CvvImage m_CvvImage;
			m_CvvImage.CopyOf(interimg,1); //复制该帧图像    
			m_CvvImage.DrawToHDC(hdc, &rect); //显示到设备的矩形框内
			cvReleaseImage(&interimg);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CcoutingDlg::OnBnClickedSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);

	if(!img){
		MessageBox(_T("请先打开视频源"),_T("提示"));
		return ;
	}
	MouseArgs* m_arg =new MouseArgs();
	m_arg->img = cvCloneImage(img);
	cvNamedWindow("Draw ROI",CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("Draw ROI",MouseDraw,(void*)m_arg); 

	while(1)
	{
		cvShowImage("Draw ROI",m_arg->img);
		char c=cvWaitKey(100);
		if(c==13|c==32)
			{
				SetTimer(1,25,NULL);
				break;
			}
	}

	if(m_arg->points < 1)
		return ;
	cout<<m_arg->points <<endl;
	mask = cvCreateImage( cvGetSize(img), 8, 1 );
	cvZero(mask);

	PointArr =new CvPoint[m_arg->points];
	cvCvtSeqToArray(m_arg->seq, PointArr);
	size=m_arg->points;
	cvFillConvexPoly(mask,PointArr,m_arg->points,cvScalarAll(255),CV_AA,0);
	//cvNamedWindow("Mask",CV_WINDOW_AUTOSIZE);
	//cvShowImage("Mask",mask);
	//int d=0;
	//double c=cvPointPolygonTest(m_arg->seq,cvPoint2D32f(50,50),d);
	//cout<<c<<endl;

	// 获得区域
	//IplImage* roi = cvCreateImage( cvGetSize(img), 8, 3 );
	//cvCopy(img,roi,mask);
	//cvNamedWindow("ROI",CV_WINDOW_AUTOSIZE);
	//cvShowImage("ROI",roi);

	//
	
	//cvWaitKey(0);
	cvDestroyWindow("Draw ROI");
	//cvDestroyWindow("Mask");
	//cvDestroyWindow("ROI");

	//
//	m_arg->Destroy ();
	//delete m_arg;
	//cvReleaseImage(&mask);
	//cvReleaseImage(&roi);
	//
	getchar();
	return ;
}


void CcoutingDlg::OnBnClickedDetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if(img==NULL)
	{
		MessageBox(_T("请先打开视频源"),_T("提示"));
		return ;
	}
	pThread=AfxBeginThread(ThreadFunc,NULL);
	
}

UINT ThreadFunc(LPVOID lpParam){

	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); 
	if( !cascade ) 
	{ 
		 AfxMessageBox(_T("无法加载分类器"),MB_YESNO);
		return -1; 
	} 
	storage = cvCreateMemStorage(0); 
	cvNamedWindow( "result", 1 ); 
	::SetTimer(NULL,1,50,(void(CALLBACK*)(HWND,UINT,UINT,DWORD))TimerProc); 
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
		DispatchMessage(&msg);   //消息分发到回调函数

	return 0; 

}

void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	if(img!=NULL) 
	{ 
		//IplImage *img1=cvCloneImage(img);
		//PedesNum=0;
		IplImage* roi = cvCreateImage( cvGetSize(img), 8, 3 );
		cvCopy(img,roi,mask);
		detect_and_draw(roi); 
		cvWaitKey(0); 
		cvReleaseImage( &roi);
	}
	else if (current<FrameMAX)
	{
		AfxMessageBox(_T("无法加载图片"),MB_YESNO);
	}
	cvDestroyWindow("result"); 
	return ;
}

void detect_and_draw(IplImage* img ) 
{ 
	double scale=0.9; //0.43
	static CvScalar colors[] = { 
		{{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}}, 
		{{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}} 
	};//Just some pretty colors to draw with

	//Image Preparation 
	// 
	IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
	IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
	cvCvtColor(img,gray, CV_BGR2GRAY); 
	cvResize(gray, small_img, CV_INTER_LINEAR);

	cvEqualizeHist(small_img,small_img); //直方图均衡

	//Detect objects if any 
	// 
	cvClearMemStorage(storage); 
	double t = (double)cvGetTickCount(); 
	CvSeq* objects = cvHaarDetectObjects(small_img, 
		cascade, 
		storage, 
		1.1, 
		1, 
		0|CV_HAAR_SCALE_IMAGE/*CV_HAAR_DO_CANNY_PRUNING*/, 
		cvSize(10,10),cvSize(28,28));

	t = (double)cvGetTickCount() - t; 
	printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
	//cv::vector<cv::Rect> found;
	//Loop through found objects and draw boxes around them 
	int x=0;
	for(int i=0;i<(objects? objects->total:0);++i) 
	{ 
		CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 
		if((r->width*scale<=30)&&(r->height*scale)<=30)
		{
			x++;
			cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]); 
		}
	}
	PedesNum=x;
	cvShowImage( "result", img ); 
	cvReleaseImage(&gray); 
	cvReleaseImage(&small_img); 
}