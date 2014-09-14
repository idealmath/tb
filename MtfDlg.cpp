// MtfDlg.cpp : //Mtf文本编辑对话框实现文件
//

#include "stdafx.h"
#include "Figure.h"
#include "MtfDlg.h"
#include "MyImage.h"
#include ".\colormenu.h"
#include "ToolCtrPopup.h"
extern byte m_ColorT[4];    //文本颜色

// CMtfDlg  Mtf文本编辑对话框

IMPLEMENT_DYNAMIC(CMtfDlg, CDialog)

CMtfDlg::CMtfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMtfDlg::IDD, pParent)
{
	m_mtfstr=L"<e></e>";//mtf格式串
	m_bmWidth=m_bmHeight=0; //显示位图的高
	m_p32Bits=NULL;//32位位图数据指针
	m_clrBkgnd=0;	
	m_clrText=0xFFFF;
	m_nInitType=0;//打开对话框的类型，0为初始添加文本，1为编辑修改
}

CMtfDlg::~CMtfDlg()
{
	if(m_p32Bits)
	{
		delete []m_p32Bits;
		m_p32Bits=NULL;
	}
}
// CMtfDlg 消息处理程序

void CMtfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM, m_FormulaCtrl);

}

BEGIN_MESSAGE_MAP(CMtfDlg, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(MYMSG_DLGSISZ,OnMySize)		
	ON_MESSAGE(WM_KICKIDLE ,OnKickIdle)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW,   0,   0xFFFF,   OnToolTipText) 
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA,   0,   0xFFFF,   OnToolTipText)
	ON_MESSAGE(MYMSG_FONTNAMECHANGE,OnFontName)
	ON_MESSAGE(MYMSG_FONTSIZECHANGE,OnFontSize)
	ON_MESSAGE(MYMSG_FORMULAIDCHANGE,OnFormulaIdChange)
	ON_MESSAGE(MYMSG_RETURN,OnBarReturn)
	ON_COMMAND(ID_MTF_COLOR, OnToolbarColor)
	ON_COMMAND(ID_RICH_SYMBOL, OnPopSymbolWnd)
	ON_MESSAGE(MYMSG_TOOLBARBTN, OnSymbolBtn)
END_MESSAGE_MAP()
// CMtfDlg 消息处理程序
 LRESULT CMtfDlg::OnKickIdle(WPARAM w,LPARAM l)
 {
  //    //调用CWnd::UpdateDialogControls更新用户界面
  //  UpdateDialogControls(this, TRUE);	 
	 m_dlgMtfBar.OnUpdateCmdUI((CFrameWnd *)this,TRUE);
	 m_dlgToolBar.OnUpdateCmdUI((CFrameWnd *)this,TRUE);	
	if(m_BinTree.m_nEditType)//选中的节点
	{
		CNode* p=m_BinTree.m_pCurEditNode;//当前待编辑节点
		CDC *dc =GetDC(); 		
		if(dc)
		{
			m_dlgMtfBar.SyncToView(p->m_lf.lfFaceName,abs(MulDiv(p->m_lf.lfHeight,1440,dc->GetDeviceCaps(LOGPIXELSY))));
				
		}
		ReleaseDC(dc);
	}
	 return 0;
 }
BOOL CMtfDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();	
	//int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	//int nFullHeight=GetSystemMetrics(SM_CYSCREEN);		
	//MoveWindow(0,0,nFullWidth,nFullHeight);//全屏，为了工具栏下的一条线长度,现改用了RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	m_ImageList.Create(20,20,ILC_COLOR4|ILC_MASK,128,0);
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAPSYMBOL1);
	m_ImageList.Add(&bm,0xFFFFFF);
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAPSYMBOL2);
	m_ImageList.Add(&bm,0xFFFFFF);
	bm.DeleteObject();

	m_FormulaCtrl.SetBinTree(&m_BinTree);
	if (!m_dlgMtfBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		/*| CBRS_GRIPPER */| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_dlgMtfBar.LoadToolBar(IDR_TOOLBARMTFDLG,1))
	{ 
		TRACE0("未能创建工具栏\n");
		return -1;    
	}
	m_dlgMtfBar.SetWindowText(_T("格式工具栏"));
	m_dlgMtfBar.EnableToolTips(TRUE);	
	if (!m_dlgToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_dlgToolBar.LoadToolBar(IDR_TOOLBARMTFBTN))
	{
		TRACE0("未能创建工具栏\n");
		return -1;    
	}
	m_dlgToolBar.SetWindowText(_T("模板工具栏"));
	m_dlgToolBar.EnableToolTips(TRUE);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);	
	m_FormulaCtrl.SetFocus();

	m_BinTree.m_DefaultColor=m_clrText;
	m_BinTree.m_clrBkgnd=m_clrBkgnd;
	
	if(m_nInitType<2)//不是编辑标签
	{
		lstrcpy(m_BinTree.m_DefaultLF.lfFaceName, L"宋体");//L"宋体" L"微软雅黑"
		m_BinTree.m_DefaultLF.lfCharSet=GB2312_CHARSET;
		m_BinTree.m_DefaultLF.lfHeight=-35;
	}
	m_BinTree.BuildTree(m_mtfstr);
	m_BinTree.m_pCurEditNode=m_BinTree.GetRootNode();
	if(m_nInitType==0)//初始添加文本
	{		
		m_FormulaCtrl.CreateEdit(VK_ESCAPE,0);
	}
	else m_BinTree.m_nEditType=4;
	m_FormulaCtrl.SetModifyPoint();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE

}
BOOL CMtfDlg::PreTranslateMessage(MSG* pMsg)
{	
	return CDialog::PreTranslateMessage(pMsg); //用此 子窗口keydown正常用
} 
BOOL CMtfDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	// allow top level routing frame to handle the message
	/*if (GetRoutingFrame() != NULL)
		return FALSE;*/
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//
	CString strTipText;
	UINT_PTR nID = (UINT_PTR)pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	if (nID != 0) // will be zero on a separator
	{
		strTipText.LoadString((UINT)nID);		
		// this is the command id, not the button index		
		strTipText=strTipText.Mid(strTipText.Find('\n')+1);
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText,
		(sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0])));
	else
	{
		int n = MultiByteToWideChar(CP_ACP, 0, strTipText, -1, pTTTW->szText,
			sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0]));
		if (n > 0)
			pTTTW->szText[n-1] = 0;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		int n = WideCharToMultiByte(CP_ACP, 0, strTipText, -1,
			pTTTA->szText, sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0]),
			NULL, NULL);
		if (n > 0)
			pTTTA->szText[n-1] = 0;
	}
	else
		lstrcpyn(pTTTW->szText, strTipText,
		(sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0])));
#endif
	*pResult = 0;
	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);
	return TRUE; // message was handled
}
LRESULT CMtfDlg::OnMySize(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetWindowRect(rect);
	int w=rect.Width(),h=rect.Height();
	int w2=(int)wParam+80,h2=(int)lParam+150;		
	bool bb=false;
	if(w2>w||w-w2>100)
	{
		w=w2+50;
		if(w<600)w=600;
		bb=true;
	}
	if(h2>h ||h-h2>100)
	{
		h=h2+50;
		if(h<350)h=350;
		bb=true;
	}	
	if(bb)
	{
		int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
		int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
		if(w>nFullWidth)w=nFullWidth;
		if(h>nFullHeight)h=nFullHeight;
		//MoveWindow((nFullWidth-w)/2,(nFullHeight-h)/2,w,h);//居中
		MoveWindow(rect.left,rect.top,w,h);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	}
	return 0;
}
void CMtfDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (IsWindow(m_FormulaCtrl.GetSafeHwnd()))
	{
		CRect rect;
		m_FormulaCtrl.GetWindowRect(rect);
		ScreenToClient(rect);
		int ll=rect.left,tt=rect.top;	
		m_FormulaCtrl.MoveWindow(ll,tt,cx-2*ll,cy-tt-35);
		GetDlgItem(IDOK)->MoveWindow(cx/2+80,cy-30,80,24);
		GetDlgItem(IDCANCEL)->MoveWindow(cx/2-160,cy-30,80,24);				
	}
}
void CMtfDlg::OnOK()
{
	m_mtfstr=m_BinTree.GetMtfString();//mtf格式串	
	//m_p32Bits=m_BinTree.GetDIB(m_bmWidth,m_bmHeight,m_bAlpha);	
	CDialog::OnOK();
}

BOOL CMtfDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(IsWindow(m_FormulaCtrl.GetSafeHwnd()) && m_FormulaCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	else
		return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CMtfDlg::OnFontName(WPARAM wParam, LPARAM lParam)
{	
	return m_FormulaCtrl.SendMessageW(MYMSG_FONTNAMECHANGE,wParam,lParam);
	 

}
LRESULT  CMtfDlg::OnFontSize(WPARAM wParam, LPARAM lParam)
{	
	return m_FormulaCtrl.SendMessage(MYMSG_FONTSIZECHANGE,wParam,lParam);
	
}

LRESULT  CMtfDlg::OnFormulaIdChange(WPARAM wParam, LPARAM lParam)
{	
	return m_FormulaCtrl.SendMessage(MYMSG_FORMULAIDCHANGE,wParam,lParam);
	
}
//
LRESULT  CMtfDlg::OnBarReturn(WPARAM wParam, LPARAM lParam)
{
	m_FormulaCtrl.SetFocus();
	return 0;
}
void CMtfDlg::OnToolbarColor()
{
	CColorMenu colorMenu(1);
	CRect rc;
	int index =m_dlgMtfBar.CommandToIndex(ID_MTF_COLOR);
	m_dlgMtfBar.GetItemRect(index, &rc);
	m_dlgMtfBar.ClientToScreen(rc);
	colorMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);

}

void CMtfDlg::OnPopSymbolWnd()
{
	CRect rect;
	int index=m_dlgMtfBar.CommandToIndex(ID_RICH_SYMBOL);
	//m_dlgToolBar.SetButtonInfo(index,ID_RICH_SYMBOL, TBBS_SEPARATOR,36);
	m_dlgMtfBar.GetItemRect(index,&rect);	
	m_dlgMtfBar.ClientToScreen(rect);
	CPoint point(rect.left,rect.bottom);	
	CToolBarPopupWnd * m_wndBtn=new CToolBarPopupWnd;
	m_wndBtn->Create(point,this,&m_ImageList,8,11);

}
LRESULT CMtfDlg::OnSymbolBtn(WPARAM wParam, LPARAM lParam)
{	
	static CString symbol=L"αβγδεζηθλμξπρφψω±＋－×÷√∵∴＝≠≤≥≌∽∥⊥∈∪∩∧∨∫∞…§№℃°⌒⊙∠Δ→←↑↓⒈⒉⒊⒋≡≈①②③④⑤⑥⊕≮≯∮□○∑¬▵▱∅⊖⊗⊘≞≜⇄⇌⇍⇎⇏⇐⇔⇒⊆⊇∀∃∁∡∉∄⊂⊃⊄⊅⊈⊉⊊⊋∓∤∦∂∬∭∯∰✍☜☝☞☟❄✉♪➶➵➢➣✓✗✰☭";	
	m_FormulaCtrl.PostMessage(WM_CHAR,symbol[(int)lParam],1);	
	return 0;
}