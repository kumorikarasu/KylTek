#include "main.h"
//#include "CDeviceHandler.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIButton.h"

CGUIButton::CGUIButton(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, LPTSTR _text, bool _canHoldDown)
	: CGUIBase(params, colors, sprites){
	m_Event=_event;
	m_pDest=_dest;
	m_Text=_text;
	m_bCanHoldDown=_canHoldDown;
	m_holdTimer=20;
	m_Type=GUITYPE_BUTTON;
};
CGUIButton::~CGUIButton(){};

bool CGUIButton::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(!m_bEnabled)
		return false;
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
bool CGUIButton::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	return false;
}
bool CGUIButton::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {

	switch(uMsg){
		case WM_MOUSEMOVE:
			POINT MousePos;
			GetCursorPos(&MousePos);
			if(!m_bMouseOver && PtInBB(MousePos) && GetVisible()){
				OnMouseEnter();
			}if(m_bMouseOver && !PtInBB(MousePos)){
				OnMouseLeave();
			}
			//if(Parent && ((CGUIWindow*)Parent)->GetFocusElement()==this)
				//return true;
			
			break;
		case WM_LBUTTONDOWN:
			if(m_bMouseOver){
				if(GetFocus() || RequestFocus()){
					m_bButtonDown=true;
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
	if(m_bMouseOver || m_bButtonDown)
		return true;
	return false;
}

bool CGUIButton::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIButton::CanHaveFocus(){ return true; }
void CGUIButton::OnFocusIn() { m_bHasFocus = true; }
void CGUIButton::OnFocusOut() { m_bHasFocus = false; }
void CGUIButton::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUIButton::OnMouseLeave() { m_bMouseOver = false; }
void CGUIButton::OnHotkey() {
	if(OnGUIEvent)
		OnGUIEvent(m_Event, this, m_pDest);
}

void CGUIButton::Step(){
	if(m_bButtonDown && m_bMouseOver){
		m_holdTimer--;
		if(m_holdTimer==0){
			OnHotkey();
			m_holdTimer=2;
		}	
	}else
		m_holdTimer=20;
}
void CGUIButton::Draw(CGraphicsHandler *g){
	if(GetVisible()){
		UpdateBB();
		if(m_pSprites){
			if(m_bButtonDown && m_bMouseOver && m_pSprites->Button->MouseDown && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, m_pos);
			else if((m_bMouseOver || m_bButtonDown) && m_pSprites->Button->MouseOver && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, m_pos);
			else if(m_pSprites->Button->Default)
				g->DrawSpriteTL(m_pSprites->Button->Default, 0, m_pos);
		}else if(m_pColors)
			g->DrawRectTL(m_pos, m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);

		if(m_Text!=L"" && m_pColors)
			g->DrawTextTL(m_Text, Vector2(m_pos.x+5, m_pos.y+2), m_pColors->Text);

		//if(GetFocus())
			//DrawTextExact(g_pFontList->Arial20B, "Has Focus->", dx-128, dy+2, TextColor);
	
		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}