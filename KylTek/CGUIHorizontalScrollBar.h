#ifndef CGUIHORIZONTALSCROLLBAR_H_
#define CGUIHORIZONTALSCROLLBAR_H_

#include "CGUIButton.h"
#include "Sprite.h"

class CGUIHorizontalScrollBar : public CGUIBase{
private:
	CGUIButton *m_pLeft, *m_pRight;
	float m_minBarWidth, m_maxBarWidth, m_barWidth, m_minValue, m_maxValue, m_value;
	Vector2 m_barPos;
	POINT m_mouseStart;
	RECT m_barBB;
public:
	CGUIHorizontalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* up, GUI_BUTTON_SPRITES* down);
	CGUIHorizontalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, HSBspr* HSBsprlist);
	~CGUIHorizontalScrollBar();

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
	void ResizeBar(int width);

	void SetMinValue(float value);
	float GetMinValue();
	void SetValue(float value);
	float GetValue();
	void SetMaxValue(float value);
	float GetMaxValue();
	float GetMinBarWidth(){return m_minBarWidth;}
	float GetBarWidth(){return m_barWidth;}
	float GetMaxBarWidth(){return m_maxBarWidth;}

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);
};


#endif