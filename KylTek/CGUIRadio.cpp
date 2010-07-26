#include "main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIRadio.h"

CGUIRadioGroup::CGUIRadioGroup(){
	m_pActiveButton=NULL;
	m_numOfButtons=0;	
};
CGUIRadioGroup::~CGUIRadioGroup(){};

void CGUIRadioGroup::AddButton(CGUIRadioButton *button, bool active){
	if(!button)
		return;

	button->SetGroup(this);
	button->SetId(m_numOfButtons);
	

	m_pButtonList.push_back(button);
	m_numOfButtons++;

	if(active)
		SetActive(button->GetId());
	
}
void CGUIRadioGroup::AddButton(GUI_PARAMS *params, GUI_COLORS *colors, GUI_SPRITES *sprites, UINT _event, void *_dest, bool active){
	AddButton(new CGUIRadioButton(params, colors, sprites, _event, _dest, NULL), active);
}

CGUIRadioButton::CGUIRadioButton(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, CGUIRadioGroup* group)
: CGUIBase(params, colors, sprites){
	m_Type=GUITYPE_RADIO;
	m_Event=_event;
	m_pDest=_dest;
	m_pGroup=group;
	m_bActive=false;
	if(m_pGroup)
		m_pGroup->AddButton(this, false);
}
CGUIRadioButton::~CGUIRadioButton(){
	
}

bool CGUIRadioButton::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
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
bool CGUIRadioButton::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	return false;
}
bool CGUIRadioButton::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {

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
				if(!m_bButtonDown)
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
	}
	if(m_bMouseOver)
		return true;
	return false;
}

bool CGUIRadioButton::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIRadioButton::CanHaveFocus(){ return true; }
void CGUIRadioButton::OnFocusIn() { m_bHasFocus = true; }
void CGUIRadioButton::OnFocusOut() { m_bHasFocus = false; }
void CGUIRadioButton::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUIRadioButton::OnMouseLeave() { m_bMouseOver = false; }
void CGUIRadioButton::OnHotkey() {
	if(m_pGroup)
		m_pGroup->SetActive(m_Id);
	if(OnGUIEvent)
		OnGUIEvent(m_Event, this, m_pDest);
}

void CGUIRadioButton::Step(){}
void CGUIRadioButton::Draw(CGraphicsHandler *g){
	if(GetVisible()){
		UpdateBB();
		if(m_pSprites){
			if(m_bButtonDown && m_bMouseOver && m_pSprites->Button->MouseDown && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, m_pos);
			else if((m_bMouseOver || m_bButtonDown) && m_pSprites->Button->MouseOver && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, m_pos);
			else if(m_pSprites->Button->Default)
				g->DrawSpriteTL(m_pSprites->Button->Default, 0, m_pos);
			if(GetActive())
				g->DrawSpriteTL(m_pSprites->Activated, 0, m_pos);
		}else if(m_pColors){
			g->DrawOvalTL(m_pos, m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);
			if(GetActive())
				g->DrawOvalTL(m_pos+Vector2((float)m_border*2, (float)m_border*2), m_width-m_border*4, m_height-m_border*4, m_pColors->Text);
		}
		

		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}