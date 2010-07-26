#pragma once
#ifndef CGUICHECKBOX_H_
#define CGUICHECKBOX_H_

class CGUICheckBox : public CGUIBase{
private:
	bool m_bChecked;
	UINT m_Event;
	void* m_pDest;
public:
	CGUICheckBox(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, bool checked);
	~CGUICheckBox();

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

	void SetChecked( bool _checked ) { m_bChecked=_checked; }
	bool GetChecked() { return m_bChecked; }
};

#endif