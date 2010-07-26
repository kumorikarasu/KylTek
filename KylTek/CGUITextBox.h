#pragma once
#ifndef CGUITEXTBOX_H_
#define CGUITEXTBOX_H_

#include "CGUIVerticalScrollBar.h"
#include "CGUIHorizontalScrollBar.h"

class CGUITextBox : public CGUIBase{  
private:
	std::wstring m_TextDraw;
	std::vector<std::wstring> m_Text;
	bool m_bEditable, m_bSelectable, m_bCaretOn;
	UINT m_Caretx, m_Carety, m_SelStartx, m_SelStarty, m_CaretBlink, m_MaxCaretx, m_MaxCarety, m_Scrollx, m_Scrolly;
	POINT m_MousePos;

	CGUIVerticalScrollBar* m_pVSB;
	CGUIHorizontalScrollBar* m_pHSB;
public:
	CGUITextBox(GUI_PARAMS* params, GUI_COLORS* _colors, GUI_SPRITES* _sprites, bool _editable, bool _selectable, LPTSTR _text);
	~CGUITextBox();

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

	void SetText(std::wstring _text);
	void AddLine(std::wstring _text);
	void AddText(std::wstring _text);
	void AddChar(TCHAR _text);
	void SetTextDraw();
	std::wstring Getline(int _line){ return m_Text[_line]; }
	void PlaceCaret(int _x, int _y);
	void PlaceCaret(POINT _cursor);
	void SyncSelStart();
	void DeleteSelectionText();
	void DeleteTextAtCaret();

	void EnableVSB(VSBspr* VSBsprlist);
	void EnableHSB(HSBspr* HSBsprlist);

	CGUIVerticalScrollBar* GetVSB(){ return m_pVSB; }
	CGUIHorizontalScrollBar* GetHSB() { return m_pHSB; }

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);
};

#endif