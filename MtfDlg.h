#pragma once

#include "mtfctrl.h"
#include "FormatToolBar.h"
//#include "afxpriv.h"//
#define WM_KICKIDLE 0x036A
#define WM_IDLEUPDATECMDUI     0x0363


// CMtfDlg Mtf文本编辑对话框对话框

class CMtfDlg : public CDialog
{
	DECLARE_DYNAMIC(CMtfDlg)

public:
	CMtfDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMtfDlg();
	CBinTree m_BinTree;	
	CToolBar  m_dlgToolBar;
	CFormatToolBar m_dlgMtfBar;
	CFormulaCtrl m_FormulaCtrl;	
	//
	CString m_mtfstr;//mtf格式串
	LONG m_bmWidth; //显示位图的宽
	LONG m_bmHeight; //显示位图的高
	GLubyte* m_p32Bits;//32位位图数据指针
	COLORREF m_clrBkgnd;//文本背景色，被透明
	COLORREF m_clrText;//文本色
	CImageList m_ImageList;	
	//byte m_bAlpha;//文本背景透明度

	int m_nInitType;//打开对话框的类型，0为初始添加文本，1为编辑修改,2为修改度量值标签

/// 对话框数据
	enum { IDD = IDD_MTF_DLG };
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
