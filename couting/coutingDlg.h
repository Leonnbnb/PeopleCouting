
// coutingDlg.h : ͷ�ļ�
//
#include "mouseROI.h"
#pragma once


// CcoutingDlg �Ի���
class CcoutingDlg : public CDialogEx
{
// ����
public:
	CcoutingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COUTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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