
// LLView.h : interface of the CLLView class
//

#pragma once
#include <memory>
#include "TabDebugControl.h"
#include "AppDef.h"
#include <array>
#include <string>

class MonitorDebug;
class CLLDoc;

class CLLView : public CView
{
protected: // create from serialization only
	CLLView() noexcept;
	DECLARE_DYNCREATE(CLLView)

// Attributes
public:
	CLLDoc* GetDocument() const;
	void CloseMonitorDebug();
// Operations
public:
	std::array<CListBox, MAX_TAB> m_arrayTab;
	TabDebugControl m_tabManger;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	void PopulateDebugBufferData(const std::string& mess, UINT8 id);
// Implementation
public:
	virtual ~CLLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	MonitorDebug* m_pMonitor;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LLView.cpp
inline CLLDoc* CLLView::GetDocument() const
   { return reinterpret_cast<CLLDoc*>(m_pDocument); }
#endif

