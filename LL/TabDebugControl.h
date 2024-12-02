#pragma once


class TabDebugControl : public CMFCTabCtrl
{
public:
	TabDebugControl();
	virtual ~TabDebugControl();
public:
	int m_nCurrentTabSelected;
protected:
	virtual  BOOL SetActiveTab(int iTab) override;

	DECLARE_MESSAGE_MAP()
};