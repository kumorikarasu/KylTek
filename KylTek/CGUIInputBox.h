#pragma once
#ifndef CGUIINPUTBOX_H_
#define CGUIINPUTBOX_H_

enum eFlags{
	EmptyState,
	ANYCHAR = 1 << 0,
	ALPHANUMERIC = 1 << 1,
	ALPHABET = 1 << 2, 
	NUMERIC = 1 << 3,
	PASSWORD = 1 << 4	
};

class CGUIInputBox : public CGUIBase{  
private:
	std::wstring m_Text;
	bool m_bCaretOn;
	UINT m_Caretx, m_SelStartx, m_CaretBlink, m_MaxCaretx, m_Scrollx;
	UINT m_Event;
	void* m_pDest;
	POINT m_MousePos;

	int m_flags;
public:
	CGUIInputBox(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, int flags, UINT _event, void* _dest, LPTSTR _text);
	~CGUIInputBox();

	virtual bool WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void setFlags(int flags)	{ m_flags |= flags; }
	void unsetFlags(int flags)	{ m_flags &= ~flags; }
	bool CheckFlag(int flags)	{ if(m_flags & flags) return true; return false; }

	virtual bool RequestFocus();
	virtual bool CanHaveFocus();
	virtual void OnFocusIn();
	virtual void OnFocusOut();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnHotkey();

	void SetText(std::wstring text);
	std::wstring GetText(){ return m_Text; }
	void AddChar(TCHAR _text);
	void PlaceCaret(int x);
	void PlaceCaret(POINT cursor);
	void SyncSelStart();
	void DeleteSelectionText();
	void DeleteTextAtCaret();

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);
};

#endif