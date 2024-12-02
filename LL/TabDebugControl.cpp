#include "pch.h"
#include "TabDebugControl.h"


BEGIN_MESSAGE_MAP(TabDebugControl, CMFCTabCtrl)
END_MESSAGE_MAP()

TabDebugControl::TabDebugControl()
	: m_nCurrentTabSelected(0)
{
}

TabDebugControl::~TabDebugControl()
{
}

BOOL TabDebugControl::SetActiveTab(int iTab)
{
	m_nCurrentTabSelected = iTab;

	return CMFCTabCtrl::SetActiveTab(iTab);
}
