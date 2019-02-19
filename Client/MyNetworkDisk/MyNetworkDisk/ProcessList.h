#pragma once


// CProcessList
typedef struct _ProcessLCI_Data_
{
            int  nProportion;
           int nStep;
} PLCIDATA, *LPPLCIDATA;
class CProcessList : public CListCtrl
{
	DECLARE_DYNAMIC(CProcessList)

public:
	CProcessList();
	virtual ~CProcessList();

protected:
	DECLARE_MESSAGE_MAP()
private:
public:
	void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
	void CProcessList::DrawText(int nItem, 
        int nSubItem, 
        CDC *pDC, 
        COLORREF crText, 
        COLORREF crBkgnd, 
        CRect &rect);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


