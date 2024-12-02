
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "LL.h"
#include "LLView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	ModifyStyle(WS_MAXIMIZEBOX, 0);
	ModifyStyle(WS_MINIMIZEBOX, 0);
	ModifyStyle(WS_THICKFRAME, 0);

	return 0;
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	SetWindowText("Live Logger");
}

void CMainFrame::OnClose()
{
	// calling close monitor
	// because only one view, so can access pView directly
	CFrameWnd* pMainFrame = static_cast<CFrameWnd*>(AfxGetMainWnd());
	if (pMainFrame)
	{
		CLLView* pView = static_cast<CLLView*>(pMainFrame->GetActiveView());
		if (pView)
		{
			pView->CloseMonitorDebug();
		}
	}

	CFrameWnd::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.hMenu = NULL;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

