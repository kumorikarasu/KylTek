#ifndef CGUIVERTICALSCROLLBAR_H_
#define CGUIVERTICALSCROLLBAR_H_

#include "CGUIButton.h"
#include "Sprite.h"

class CGUIVerticalScrollBar : public CGUIBase{
private:
	CGUIButton *m_pUp, *m_pDown;
	float m_minBarHeight, m_maxBarHeight, m_barHeight, m_minValue, m_maxValue, m_value;
	Vector2 m_barPos;
	POINT m_mouseStart;
	RECT m_barBB;
	bool m_bMouseOverBar;
public:
	CGUIVerticalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* up, GUI_BUTTON_SPRITES* down);
	CGUIVerticalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, VSBspr* VSBsprlist);
	~CGUIVerticalScrollBar();

	void Init(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* up, GUI_BUTTON_SPRITES* down);

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

	virtual void UpdateBB();
	virtual void Resize(int width, int height);
	bool PtInBarBB(POINT _pt){ if(PtInRect(&m_barBB, _pt))return true; return false; }
	bool PtInBarBB(Vector2 _pt){ 
		POINT pt={(long)_pt.x, (long)_pt.y};
		return PtInBarBB(pt); 
	}

	void SetParent(CGUIBase* _parent);

	void Scroll(int value);
	void ResizeBar(int height);

	void SetMinValue(float value);
	float GetMinValue();
	void SetValue(float value);
	float GetValue();
	void SetMaxValue(float value);
	float GetMaxValue();
	float GetMinBarHeight(){return m_minBarHeight;}
	float GetBarHeight(){return m_barHeight;}
	float GetMaxBarHeight(){return m_maxBarHeight;}

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);
};


#endif