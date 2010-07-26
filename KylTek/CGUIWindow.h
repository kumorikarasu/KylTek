#pragma once
#ifndef CGUIWINDOW_H_
#define CGUIWINDOW_H_

#include <list>

class CGUIWindow : public CGUIBase{
private:
	std::list<CGUIBase*>* m_pGUIList;
	std::list<CGUIBase*>::iterator m_GUIit;
	std::list<CGUIBase*>::reverse_iterator m_ReverseGUIit;
	CGUIBase *m_pElement;
	CGUIBase* m_pFocusElement;
	std::wstring m_Title;
	bool m_bMovable;
	POINT m_mouseStart;
	UINT m_numOfElements;
	IDirect3DSurface9* m_pDrawSurface;
public:
	
	CGUIWindow(GUI_PARAMS* params, GUI_COLORS* _colors, GUI_SPRITES* _sprites, bool _movable, LPTSTR _title);
	~CGUIWindow();

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

	bool FocusNext();
	bool SetFocusElement(CGUIBase* _element);
	CGUIBase* GetFocusElement(){ return m_pFocusElement; }

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);

	void Add(CGUIBase* _GUI);
};


#endif