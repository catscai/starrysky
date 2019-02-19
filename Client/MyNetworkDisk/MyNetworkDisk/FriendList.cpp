// FriendList.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "FriendList.h"


// CFriendList

IMPLEMENT_DYNAMIC(CFriendList, CListBox)

CFriendList::CFriendList()
{
	m_menu = NULL;
}

CFriendList::~CFriendList()
{
}


BEGIN_MESSAGE_MAP(CFriendList, CListBox)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CFriendList 消息处理程序




void CFriendList::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT CursorPos;
	::GetCursorPos(&CursorPos);
	m_menu->GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, CursorPos.x, CursorPos.y, this->GetParent());  
	CListBox::OnRButtonUp(nFlags, point);
}

void	CFriendList::SetMyMenu(CMenu* u)
{
	this->m_menu = u;
}