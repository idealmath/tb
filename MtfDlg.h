#pragma once

#include "mtfctrl.h"
#include "FormatToolBar.h"
//#include "afxpriv.h"//
#define WM_KICKIDLE 0x036A
#define WM_IDLEUPDATECMDUI     0x0363


// CMtfDlg Mtf�ı��༭�Ի���Ի���

class CMtfDlg : public CDialog
{
	DECLARE_DYNAMIC(CMtfDlg)

public:
	CMtfDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMtfDlg();
	CBinTree m_BinTree;	
	CToolBar  m_dlgToolBar;
	CFormatToolBar m_dlgMtfBar;
	CFormulaCtrl m_FormulaCtrl;	
	//
	CString m_mtfstr;//mtf��ʽ��
	LONG m_bmWidth; //��ʾλͼ�Ŀ�
	LONG m_bmHeight; //��ʾλͼ�ĸ�
	GLubyte* m_p32Bits;//32λλͼ����ָ��
	COLORREF m_clrBkgnd;//�ı�����ɫ����͸��
	COLORREF m_clrText;//�ı�ɫ
	CImageList m_ImageList;	
	//byte m_bAlpha;//�ı�����͸����

	int m_nInitType;//�򿪶Ի�������ͣ�0Ϊ��ʼ����ı���1Ϊ�༭�޸�,2Ϊ�޸Ķ���ֵ��ǩ

/// �Ի�������
	enum { IDD = IDD_MTF_DLG };
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
public:			
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg LRESULT OnKickIdle(WPARAM w,LPARAM l);	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg   BOOL OnToolTipText(UINT   nID,   NMHDR*   pNMHDR,   LRESULT*   pResult);	
	afx_msg LRESULT OnMySize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontName(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFontSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFormulaIdChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBarReturn(WPARAM wParam, LPARAM lParam);
	afx_msg void OnToolbarColor();
	afx_msg void OnPopSymbolWnd();
	afx_msg LRESULT OnSymbolBtn(WPARAM wParam, LPARAM lParam);		
protected:
	virtual void OnOK();
};
