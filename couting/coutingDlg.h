
// coutingDlg.h : 头文件
//
#include "mouseROI.h"
#pragma once


// CcoutingDlg 对话框
class CcoutingDlg : public CDialogEx
{
// 构造
public:
	CcoutingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COUTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedDetect();
protected:
	CWinThread* pThread;
};
UINT ThreadFunc(LPVOID lpParam);
void detect_and_draw(IplImage* img );
void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
