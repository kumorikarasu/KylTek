#pragma once
#ifndef CGUIBUTTON_H_
#define CGUIBUTTON_H_

class CGUIButton : public CGUIBase{
private:
	std::wstring m_Text;
	UINT m_Event;
	void* m_pDest;
	bool m_bCanHoldDown;
	UINT m_holdTimer;
public:
	CGUIButton(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, LPTSTR _text, bool _canHoldDown);
	~CGUIButton();

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

	void SetText( LPTSTR _text ) { m_Text=_text; }
	std::wstring GetText() { return m_Text; }
};

#endif