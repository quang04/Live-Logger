
// LLView.cpp : implementation of the CLLView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LL.h"
#endif

#include "LLDoc.h"
#include "LLView.h"
#include "Resource.h"
#include "MonitorDebug.h"

#include <format>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLLView

IMPLEMENT_DYNCREATE(CLLView, CView)

BEGIN_MESSAGE_MAP(CLLView, CView)
END_MESSAGE_MAP()

// CLLView construction/destruction

CLLView::CLLView() noexcept
	: m_pMonitor(nullptr)
{
	// TODO: add construction code here

}

void CLLView::CloseMonitorDebug()
{
	if (m_pMonitor) delete m_pMonitor;
	m_pMonitor = nullptr;
}

CLLView::~CLLView()
{
}

BOOL CLLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CLLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// Create tab element
	CRect clRect;
	GetWindowRect(&clRect);
	ScreenToClient(&clRect);

	// tab manager
	m_tabManger.Create(CMFCTabCtrl::STYLE_3D,
		clRect,
		this,
		IDC_DEBUG_TAB_CONTROL,
		CMFCTabCtrl::LOCATION_TOP);


	// add each tab component
	int tabCount = 0;
	for (auto& tab : m_arrayTab)
	{
		std::string tabName = std::format("Tab {}", tabCount + 1);

		auto isSuccess = tab.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL,
			CRect(0, 0, 0, 0),
			&m_tabManger,
			IDC_LISTBOX_TAB_BASE + tabCount);

		tab.SetFont(&afxGlobalData.fontRegular);

		//no need set image for tab so set -1
		m_tabManger.AddTab(&tab, tabName.c_str(), -1, FALSE);

		tabCount++;
	}


	m_pMonitor = new MonitorDebug();

	// now only have one subscriber so fix id is 0
	m_pMonitor->AddCallback(std::bind(&CLLView::PopulateDebugBufferData, this, std::placeholders::_1, std::placeholders::_2), 0);

	m_pMonitor->Start();

}

void CLLView::PopulateDebugBufferData(const std::string& mess, UINT8 id)
{
	if (GetSafeHwnd() == nullptr) return;

	if (id >= MAX_TAB) return;

	auto& tab = m_arrayTab[id];

	auto hwnd = tab.GetSafeHwnd();
	if (!hwnd) return;

	if (tab.GetCount() > MAX_LINE)
		tab.ResetContent();

	auto index = tab.AddString(mess.c_str());

	// scroll to an end
	tab.SetTopIndex(index);
}

// CLLView drawing

void CLLView::OnDraw(CDC* /*pDC*/)
{
	CLLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CLLView diagnostics

#ifdef _DEBUG
void CLLView::AssertValid() const
{
	CView::AssertValid();
}

void CLLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLLDoc* CLLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLLDoc)));
	return (CLLDoc*)m_pDocument;
}
#endif //_DEBUG


// CLLView message handlers
