#pragma once
#ifndef CGUITABGROUP_H_
#define CGUITABGROUP_H_

#include <list>

class CGUITabGroup : public CGUIBase{
private:
	struct Tab{
		CGUIWindow* Window;
		CGUIButton* Button;
		std::wstring Title;
		UINT ListID;
	};
	std::list<Tab*>* m_pTabList;
	std::list<Tab*>::iterator m_TabIt;
	Tab* m_pActiveTab;
	UINT m_numOfTabs;
	UINT m_nextTabStartx;
public:
	
	CGUITabGroup(GUI_PARAMS* params, GUI_COLORS* _colors, GUI_SPRITES* _sprites);
	~CGUITabGroup();

	virtual bool WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool RequestFocus();
	virtual bool CanHaveFocus();
	virtual void OnFocusIn();
	virtual void OnFocusOut();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnHotkey();

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);

	void AddTab(std::wstring title, Vector2 buttonDimensions, GUI_COLORS* buttoncolors, GUI_SPRITES* buttonsprites, GUI_COLORS* windowcolors, GUI_SPRITES* windowsprites);
	void ActivateTab(UINT ListID);
	void ActivateTab(std::wstring title);
	void AddToTabWindow(UINT ListID, CGUIBase* _GUI);
	void AddToTabWindow(std::wstring title, CGUIBase* _GUI);
};

#endif