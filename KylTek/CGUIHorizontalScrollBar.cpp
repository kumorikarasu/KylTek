#include "Main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIHorizontalScrollBar.h"

CGUIHorizontalScrollBar::CGUIHorizontalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* left, GUI_BUTTON_SPRITES* right)
	: CGUIBase(params, colors, bar){
	m_Type=GUITYPE_HSB;
	m_barBB=m_BoundingBox;
	Init(params, colors, bar, left, right);
};
CGUIHorizontalScrollBar::CGUIHorizontalScrollBar(GUI_PARAMS* params, GUI_COLORS* colors, HSBspr* HSBsprlist)
	: CGUIBase(params, colors, NULL){
	m_Type=GUITYPE_HSB;

	GUI_BUTTON_SPRITES *left = new GUI_BUTTON_SPRITES(HSBsprlist->left, HSBsprlist->left_over, HSBsprlist->left_down);
	GUI_BUTTON_SPRITES *right = new GUI_BUTTON_SPRITES(HSBsprlist->right, HSBsprlist->right_over, HSBsprlist->right_down);
	GUI_SPRITES *bar = new GUI_SPRITES(HSBsprlist->bar, HSBsprlist->bar_over, HSBsprlist->bar_down, NULL, NULL);

	Init(params, colors, bar, left, right);
}

CGUIHorizontalScrollBar::~CGUIHorizontalScrollBar(){};

void CGUIHorizontalScrollBar::Init(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* bar, GUI_BUTTON_SPRITES* left, GUI_BUTTON_SPRITES* right){
	GUI_PARAMS Params;
	Params.pos=m_pos;
	Params.width=m_height;
	Params.height=m_height;
	Params.GUIEvent=OnGUIEvent;
	Params.enabled=m_bEnabled;
	Params.visible=m_bVisible;
	
	GUI_SPRITES Spr(left, NULL, NULL);

	m_pLeft = new CGUIButton(&Params, NULL, &Spr, EVENT_SCROLLLEFT, this, L"", true);

	Params.pos=m_pos+Vector2(m_width-m_height, 0);
	Spr.Button=right;

	m_pRight = new CGUIButton(&Params, NULL, &Spr, EVENT_SCROLLRIGHT, this, L"", true);

	m_minValue=0;
	m_minBarWidth=20;
	m_barPos=Vector2(m_height, 0);
	m_maxBarWidth=m_width-(m_height*2);
	m_value=0;
	m_maxValue=100;
	ResizeBar(m_maxBarWidth-(m_maxValue-m_minValue));
	if(!m_pSprites)
		m_pSprites=new GUI_SPRITES();
	m_pSprites->Button=bar->Button;
}

void CGUIHorizontalScrollBar::SetParent(CGUIBase* _parent){
	m_pParent=_parent;
	m_pLeft->SetParent(m_pParent);
	m_pRight->SetParent(m_pParent);
}

bool CGUIHorizontalScrollBar::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){

	if(!m_bEnabled)
		return false;

	if(!m_bButtonDown && (m_pLeft->WndProc(hWnd, uMsg, wParam, lParam) || m_pRight->WndProc(hWnd, uMsg, wParam, lParam))){
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
bool CGUIHorizontalScrollBar::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	return false;
}
bool CGUIHorizontalScrollBar::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	POINT MousePos;
	GetCursorPos(&MousePos);
	switch(uMsg){
		case WM_MOUSEMOVE:
			
			if(!m_bMouseOver && PtInBB(MousePos) && GetVisible()){
				OnMouseEnter();
			}if(m_bMouseOver && !PtInBB(MousePos)){
				OnMouseLeave();
			}
			if(m_bButtonDown && m_bMouseOver){
				Scroll((MousePos.x-m_mouseStart.x)*(m_maxValue/(m_maxBarWidth-m_barWidth)));
				m_mouseStart=MousePos;
			}
			//if(Parent && ((CGUIWindow*)Parent)->GetFocusElement()==this)
				//return true;

			break;
		case WM_LBUTTONDOWN:
			if(m_bMouseOver){
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

bool CGUIHorizontalScrollBar::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIHorizontalScrollBar::CanHaveFocus(){ return true; }
void CGUIHorizontalScrollBar::OnFocusIn() { m_bHasFocus = true; }
void CGUIHorizontalScrollBar::OnFocusOut() { m_bHasFocus = false; }
void CGUIHorizontalScrollBar::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUIHorizontalScrollBar::OnMouseLeave() { m_bMouseOver = false; }
void CGUIHorizontalScrollBar::OnHotkey(){}

void CGUIHorizontalScrollBar::UpdateBB(){
	Vector2 m_realpos=m_pos;
	CGUIBase* p=m_pParent;
	while(p){
		m_realpos+=p->GetPos();
		p=p->GetParent();
	}
	m_barBB.left=(long)m_realpos.x+m_barPos.x;
	m_barBB.right=(long)m_realpos.x+m_barPos.x+m_barWidth;
	m_barBB.top=(long)m_realpos.y;
	m_barBB.bottom=(long)m_realpos.y+m_height;
	m_BoundingBox.left=(long)m_realpos.x;
	m_BoundingBox.right=(long)m_realpos.x+m_width;
	m_BoundingBox.top=(long)m_realpos.y;
	m_BoundingBox.bottom=(long)m_realpos.y+m_height;
}

void CGUIHorizontalScrollBar::Resize(int width, int height){
	m_width=width;
	m_height=height;
	m_pRight->SetPos(m_pos+Vector2(m_width-m_height, 0));
	m_maxBarWidth=m_width-(m_height*2);
	ResizeBar(m_barWidth);
	UpdateBB();
}

void CGUIHorizontalScrollBar::Scroll(int value){
	SetValue(m_value+value);
}

void CGUIHorizontalScrollBar::ResizeBar(int width){
	m_barWidth=min(max(width,m_minBarWidth), m_maxBarWidth);
	
}
void CGUIHorizontalScrollBar::SetMinValue(float value){
	m_minValue=value;
	ResizeBar(m_maxBarWidth-(m_maxValue-m_minValue));
}
float CGUIHorizontalScrollBar::GetMinValue(){
	return m_minValue;
}
void CGUIHorizontalScrollBar::SetValue(float value){
	m_value=min(max(value,m_minValue), m_maxValue);
	m_barPos=Vector2(m_height+(m_maxBarWidth-m_barWidth)*((m_value-m_minValue)/(m_maxValue-m_minValue)), 0);
}
float CGUIHorizontalScrollBar::GetValue(){
	return m_value;
}
void CGUIHorizontalScrollBar::SetMaxValue(float value){
	m_maxValue=value;
	ResizeBar(m_maxBarWidth-(m_maxValue-m_minValue));
}
float CGUIHorizontalScrollBar::GetMaxValue(){
	return m_maxValue;
}

void CGUIHorizontalScrollBar::Step(){
	m_pLeft->Step();
	m_pRight->Step();
	if(m_bEnabled && m_maxValue==m_minValue){
		m_pLeft->SetEnabled(false);
		m_pRight->SetEnabled(false);
		SetEnabled(false);
	}else if(!m_bEnabled && m_maxValue!=m_minValue){
		m_pLeft->SetEnabled(true);
		m_pRight->SetEnabled(true);
		SetEnabled(true);
	}
}
void CGUIHorizontalScrollBar::Draw(CGraphicsHandler *g){
	if(GetVisible()){
		UpdateBB();
		if(m_pColors)
			g->DrawRectTL(m_pos, m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);
		if(m_pSprites){
			if(m_bButtonDown && m_bMouseOver && m_pSprites->Button->MouseDown && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, m_pos+m_barPos, Vector2(m_barWidth/m_pSprites->Button->MouseOver->SpriteWidth,1),0, ColorARGB(255,255,255,255));
			else if((m_bMouseOver || m_bButtonDown) && m_pSprites->Button->MouseOver && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, m_pos+m_barPos, Vector2(m_barWidth/m_pSprites->Button->MouseOver->SpriteWidth,1),0, ColorARGB(255,255,255,255));
			else if(m_pSprites->Button->Default)
				g->DrawSpriteTL(m_pSprites->Button->Default, 0, m_pos+m_barPos, Vector2(m_barWidth/m_pSprites->Button->MouseOver->SpriteWidth,1),0, ColorARGB(255,255,255,255));
		}

		m_pLeft->Draw(g);
		m_pRight->Draw(g);
	
		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}