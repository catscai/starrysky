// ProcessList.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "ProcessList.h"


// CProcessList
IMPLEMENT_DYNAMIC(CProcessList, CListCtrl)

CProcessList::CProcessList()
{
	//m_menu.LoadMenu(IDR_DOWN_MENU);
}

CProcessList::~CProcessList()
{
	//m_menu.DestroyMenu();
}


BEGIN_MESSAGE_MAP(CProcessList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CProcessList::OnCustomDraw)
END_MESSAGE_MAP()



// CProcessList 消息处理程序


void CProcessList::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	 //draw each item.set txt color,bkcolor....
	 NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
 
	 // Take the default processing unless we set this to something else below.
	 *pResult = CDRF_DODEFAULT;
 
	 // First thing - check the draw stage. If it’s the control’s prepaint
	 // stage, then tell Windows we want messages for every item.
 
	 if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	 {
	  *pResult = CDRF_NOTIFYITEMDRAW;
	 }
	 else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	 {
	  // This is the notification message for an item.  We’ll request
	  // notifications before each subitem’s prepaint stage.
  
	  *pResult = CDRF_NOTIFYSUBITEMDRAW;
	 }
	 else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	 {
	  // This is the prepaint stage for a subitem. Here’s where we set the
	  // item’s text and background colors. Our return value will tell
	  // Windows to draw the subitem itself, but it will use the new colors
	  // we set here.
  
	  int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
	  int nSubItem = pLVCD->iSubItem;
  
	  if(nSubItem != 1)//这里我只重绘第二列
	   return;

	  COLORREF crText  = ::GetSysColor(COLOR_WINDOWFRAME);
	  COLORREF crBkgnd = ::GetSysColor(COLOR_WINDOW);
  
	  CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
	  CRect rect;
	  GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	  if (GetItemState(nItem, LVIS_SELECTED))
	   DrawText(nItem, nSubItem, pDC, ::GetSysColor(COLOR_HIGHLIGHT), 
	   ::GetSysColor(COLOR_HIGHLIGHT), rect);
	  else
	   DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect);

	  *pResult = CDRF_SKIPDEFAULT; // We’ve painted everything.
	 }
}
void CProcessList::DrawText(int nItem, 
        int nSubItem, 
        CDC *pDC, 
        COLORREF crText, 
        COLORREF crBkgnd, 
        CRect &rect)
{
	 ASSERT(pDC);
	 pDC->FillSolidRect(&rect, crBkgnd);
 
	 int nProcess = GetItemData(nItem);
	 CRect procRect = rect;
	 pDC->Rectangle(procRect);

	 procRect.left += 1;
	 procRect.bottom -= 1;
	 procRect.top += 1;
	 procRect.right = procRect.left + rect.Width() * nProcess / 100;
	 CBrush brush(RGB(99,247,106));
	 pDC->FillRect(&procRect, &brush);
 
	 CString str;
	 str.Format(_T("%d%%"), nProcess);
 
	 if (!str.IsEmpty())
	 {
	  UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_CENTER;
  
	  pDC->SetBkMode(TRANSPARENT);
	  pDC->SetTextColor(crText);
	  pDC->SetBkColor(crBkgnd);
	  pDC->DrawText(str, &rect, nFormat);
	 }
}




