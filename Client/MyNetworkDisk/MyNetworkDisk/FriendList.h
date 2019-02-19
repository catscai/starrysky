#pragma once


// CFriendList

class CFriendList : public CListBox
{
	DECLARE_DYNAMIC(CFriendList)

public:
	CFriendList();
	virtual ~CFriendList();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CMenu*			m_menu;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void	SetMyMenu(CMenu* u);
};


