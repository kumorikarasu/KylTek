#include "Main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIVerticalScrollBar.h"

CGUIVerticalScrollBar::CGUIVerticalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* up, GUI_BUTTON_SPRITES* down)
	: CGUIBase(params, colors, bar){
	m_Type=GUITYPE_VSB;
	m_barBB=m_BoundingBox;
	Init(params, colors, bar, up, down);
};
CGUIVerticalScrollBar::CGUIVerticalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, VSBspr* VSBsprlist)
	: CGUIBase(params, colors, NULL){
	m_Type=GUITYPE_VSB;

	GUI_BUTTON_SPRITES *up = new GUI_BUTTON_SPRITES(VSBsprlist->up, VSBsprlist->up_over, VSBsprlist->up_down);
	GUI_BUTTON_SPRITES *down = new GUI_BUTTON_SPRITES(VSBsprlist->down, VSBsprlist->down_over, VSBsprlist->down_down);
	GUI_SPRITES *bar = new GUI_SPRITES(VSBsprlist->bar, VSBsprlist->bar_over, VSBsprlist->bar_down, NULL, NULL);

	Init(params, colors, bar, up, down);
}

CGUIVerticalScrollBar::~CGUIVerticalScrollBar(){};

void CGUIVerticalScrollBar::Init(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* up, GUI_BUTTON_SPRITES* down){
	GUI_PARAMS Params;
	Params.pos=m_pos;
	Params.width=m_width;
	Params.height=m_width;
	Params.GUIEvent=OnGUIEvent;
	Params.enabled=m_bEnabled;
	Params.visible=m_bVisible;
	
	GUI_SPRITES Spr(up, NULL, NULL);

	m_pUp = new CGUIButton(&Params, NULL, &Spr, EVENT_SCROLLUP, this, "", true);

	Params.pos=m_pos+Vector2(0, m_height-m_width);
	Spr.Button=down;

	m_pDown = new CGUIButton(&Params, NULL, &Spr, EVENT_SCROLLDOWN, this, "", true);

	m_minValue=100;
	m_minBarHeight=20;
	m_barPos=Vector2(0, m_width);
	m_maxBarHeight=m_height-(m_width*2);
	m_value=0;
	m_maxValue=100;
	ResizeBar(m_maxBarHeight-(m_maxValue-m_minValue));
	if(!m_pSprites)
		m_pSprites=new GUI_SPRITES();
	m_pSprites->Button=bar->Button;
}

void CGUIVerticalScrollBar::SetParent(CGUIBase* _parent){
	m_pParent=_parent;
	m_pUp->SetParent(m_pParent);
	m_pDown->SetParent(m_pParent);
}

bool CGUIVerticalScrollBar::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){

	if(!m_bEnabled)
		return false;

	if(!m_bButtonDown && (m_pUp->WndProc(hWnd, uMsg, wParam, lParam) || m_pDown->WndProc(hWnd, uMsg, wParam, lParam))){
		OnMouseLeave();
		return true;
	}

	switch(uMsg){
		// Keyboard messages
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
			return HandleKeyboard( uMsg, wParam, lParam );
		//Mouse Messages
				case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
			return HandleMouse( uMsg, wParam, lParam );
		default:
			return false;
	}
}
bool CGUIVerticalScrollBar::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	return false;
}
bool CGUIVerticalScrollBar::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	POINT MousePos;
	GetCursorPos(&MousePos);
	switch(uMsg){
		case WM_MOUSEMOVE:
			if(!m_bMouseOverBar && PtInBarBB(MousePos) && GetVisible()){
				m_bMouseOverBar=true;
			}if(m_bMouseOverBar && !PtInBB(MousePos)){
				m_bMouseOverBar=false;
			}
			if(!m_bMouseOver && PtInBB(MousePos) && GetVisible()){
				OnMouseEnter();
			}if(m_bMouseOver && !PtInBB(MousePos)){
				OnMouseLeave();
			}
			if(m_bButtonDown && m_bMouseOver){
				Scroll((MousePos.y-m_mouseStart.y)*(m_maxValue/(m_maxBarHeight-m_barHeight)));
				m_mouseStart=MousePos;
			}
			//if(Parent && ((CGUIWindow*)Parent)->GetFocusElement()==this)
				//return true;

			break;
		case WM_LBUTTONDOWN:
			if(m_bMouseOverBar){
				if(GetFocus() || RequestFocus()){
					m_bButtonDown=true;
					m_mouseStart=MousePos;
				}
				return true;
			}
			break;
		case WM_LBUTTONUP:
			if(m_bButtonDown && m_bMouseOver){
				OnHotkey();
				m_bButtonDown=false; 
				return true;
			}
			m_bButtonDown=false; 
			break;
	}
	if(m_bMouseOver)
		return true;
	return false;
}

bool CGUIVerticalScrollBar::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIVerticalScrollBar::CanHaveFocus(){ return true; }
void CGUIVerticalScrollBar::OnFocusIn() { m_bHasFocus = true; }
void CGUIVerticalScrollBar::OnFocusOut() { m_bHasFocus = false; }
void CGUIVerticalScrollBar::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUIVerticalScrollBar::OnMouseLeave() { m_bMouseOver = false; }
void CGUIVerticalScrollBar::OnHotkey(){}

void CGUIVerticalScrollBar::UpdateBB(){
	Vector2 m_realpos=m_pos;
	CGUIBase* p=m_pParent;
	while(p){
		m_realpos+=p->GetPos();
		p=p->GetParent();
	}
	m_barBB.left=(long)m_realpos.x;
	m_barBB.right=(long)m_realpos.x+m_width;
	m_barBB.top=(long)m_realpos.y+m_barPos.y;
	m_barBB.bottom=(long)m_realpos.y+m_barPos.y+m_barHeight;
	m_BoundingBox.left=(long)m_realpos.x;
	m_BoundingBox.right=(long)m_realpos.x+m_width;
	m_BoundingBox.top=(long)m_realpos.y+m_width;
	m_BoundingBox.bottom=(long)m_realpos.y+m_height-(m_width*2);
}

void CGUIVerticalScrollBar::Resize(int width, int height){
	m_width=width;
	m_height=height;
	m_pDown->SetPos(m_pos+Vector2(0, m_height-m_width));
	m_maxBarHeight=m_height-(m_width*2);
	ResizeBar(m_barHeight);
	UpdateBB();
}

void CGUIVerticalScrollBar::Scroll(int value){
	SetValue(m_value+value);
}

void CGUIVerticalScrollBar::ResizeBar(int height){
	m_barHeight=min(max(height,m_minBarHeight), m_maxBarHeight);
	
}
void CGUIVerticalScrollBar::SetMinValue(float value){
	m_minValue=value;
	ResizeBar(m_maxBarHeight-(m_maxValue-m_minValue));
}
float CGUIVerticalScrollBar::GetMinValue(){
	return m_minValue;
}
void CGUIVerticalScrollBar::SetValue(float value){
	m_value=min(max(value,m_minValue), m_maxValue);
	m_barPos=Vector2(0, m_width+(m_maxBarHeight-m_barHeight)*((m_value-m_minValue)/(m_maxValue-m_minValue)));
}
float CGUIVerticalScrollBar::GetValue(){
	return m_value;
}
void CGUIVerticalScrollBar::SetMaxValue(float value){
	m_maxValue=value;
	ResizeBar(m_maxBarHeight-(m_maxValue-m_minValue));
}
float CGUIVerticalScrollBar::GetMaxValue(){
	return m_maxValue;
}

void CGUIVerticalScrollBar::Step(){
	m_pUp->Step();
	m_pDown->Step();
	if(m_bEnabled && m_maxValue==m_minValue){
		m_pUp->SetEnabled(false);
		m_pDown->SetEnabled(false);
		SetEnabled(false);
	}else if(!m_bEnabled && m_maxValue!=m_minValue){
		m_pUp->SetEnabled(true);
		m_pDown->SetEnabled(true);
		SetEnabled(true);
	}
}
void CGUIVerticalScrollBar::Draw(CGraphicsHandler *g){
	if(GetVisible()){
		UpdateBB();
		if(m_pColors)
			g->DrawRectTL(m_pos, m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);
		if(m_pSprites){
			if(m_bButtonDown && m_bMouseOverBar && m_pSprites->Button->MouseDown && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, m_pos+m_barPos, Vector2(1,m_barHeight/m_pSprites->Button->MouseDown->SpriteHeight),0, ColorARGB(255,255,255,255));
			else if((m_bMouseOverBar || m_bButtonDown) && m_pSprites->Button->MouseOver && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, m_pos+m_barPos, Vector2(1,m_barHeight/m_pSprites->Button->MouseOver->SpriteHeight),0, ColorARGB(255,255,255,255));
			else if(m_pSprites->Button->Default)
				g->DrawSpriteTL(m_pSprites->Button->Default, 0, m_pos+m_barPos, Vector2(1,m_barHeight/m_pSprites->Button->MouseOver->SpriteHeight),0, ColorARGB(255,255,255,255));
		}

		m_pUp->Draw(g);
		m_pDown->Draw(g);
	
		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}