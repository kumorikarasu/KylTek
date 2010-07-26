#include "main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIInputbox.h"

CGUIInputBox::CGUIInputBox(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, int flags, UINT _event, void* _dest, LPTSTR _text)
	: CGUIBase(params, colors, sprites){

	m_pDest=_dest;
	m_Event=_event;
	m_Type=GUITYPE_INPUTBOX;
	m_Scrollx=0;
	SetText(_text);
	m_Caretx=m_MaxCaretx;
	SyncSelStart();
	m_bCaretOn=true;
	m_CaretBlink=40;
	m_flags=0;
	setFlags(flags);
};

CGUIInputBox::~CGUIInputBox(){};


bool CGUIInputBox::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(!m_bEnabled)
		return false;

	switch(uMsg){
		// Keyboard messages
        case WM_KEYDOWN:
		case WM_CHAR:
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
bool CGUIInputBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
if(GetFocus())
	switch(uMsg){
		case WM_KEYDOWN:
			switch( wParam ){
				case VK_UP:
					break;
				case VK_DOWN:
					break;
				case VK_LEFT:
					if(m_Caretx!=0)
						PlaceCaret(m_Caretx-1);

					if( GetKeyState( VK_SHIFT ) >= 0 )
						SyncSelStart();
					break;
				case VK_RIGHT:
					if(m_Caretx!=m_MaxCaretx)
						PlaceCaret(m_Caretx+1);

					if( GetKeyState( VK_SHIFT ) >= 0 )
						SyncSelStart();
					break;
			default:
				break;
			}
			break;
        case WM_CHAR:
			switch( wParam ){
				case VK_BACK:
					if(m_Caretx==m_SelStartx)
						DeleteTextAtCaret();
					else
						DeleteSelectionText();
					break;
				case 24: // Ctrl-X Cut
                case VK_CANCEL: // Ctrl-C Copy
                case 22: // Ctrl-V Paste
                case 26:  // Ctrl Z
                case 2:   // Ctrl B
                case 14:  // Ctrl N
                case 19:  // Ctrl S
                case 4:   // Ctrl D
                case 6:   // Ctrl F
                case 7:   // Ctrl G
                case 10:  // Ctrl J
                case 11:  // Ctrl K
                case 12:  // Ctrl L
                case 17:  // Ctrl Q
                case 23:  // Ctrl W
                case 5:   // Ctrl E
                case 18:  // Ctrl R
                case 20:  // Ctrl T
                case 25:  // Ctrl Y
                case 21:  // Ctrl U
                case 9:   // Ctrl I
                case 15:  // Ctrl O
                case 16:  // Ctrl P
                case 27:  // Ctrl [
                case 29:  // Ctrl ]
                case 28:  // Ctrl \ 
					break;
				default:
					TCHAR c=(char)wParam;
					m_flags;
					
					bool aids1=CheckFlag(ANYCHAR);
					bool aids2=CheckFlag(ALPHANUMERIC);
					bool aids3=CheckFlag(ALPHABET);
					bool aids4=CheckFlag(NUMERIC);
					if(CheckFlag(ANYCHAR) || 
							((CheckFlag(ALPHANUMERIC) || CheckFlag(ALPHABET)) && (c==' ' || (c>='a' && c<='z') || (c>='A' && c<='Z'))) ||
							((CheckFlag(ALPHANUMERIC) || CheckFlag(NUMERIC)) && (c>='0' && c<='9'))){
						AddChar(c);
						SyncSelStart();
					}
			}
			return true;
		default:
			return false;
	}

	return false;
}
bool CGUIInputBox::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	GetCursorPos(&m_MousePos);
	switch(uMsg){
		case WM_MOUSEMOVE:
			if(!m_bMouseOver && PtInBB(m_MousePos) && GetVisible()){
				OnMouseEnter();
			}if(m_bMouseOver && !PtInBB(m_MousePos)){
				OnMouseLeave();
			}
			if(m_bButtonDown)
				PlaceCaret(m_MousePos);
			break;
		case WM_LBUTTONDOWN:
			if(m_bMouseOver){
				if(!m_bButtonDown){
					if(GetFocus() || RequestFocus())
						m_bButtonDown=true;

				}
				PlaceCaret(m_MousePos);
				if( GetKeyState( VK_SHIFT ) >= 0 )
					SyncSelStart();
				return true;
			}
			break;
		case WM_LBUTTONUP:
			m_bButtonDown=false;
			break;
		case WM_MOUSEWHEEL:
			break; 
	}
	if(m_bMouseOver)
		return true;
	return false;
}
void CGUIInputBox::SetText(std::wstring _text){ 
	m_Text=_text;
	m_MaxCaretx=m_Text.length();
	PlaceCaret(m_MaxCaretx);
	SyncSelStart();
}
void CGUIInputBox::AddChar(TCHAR _char){
	if(m_Caretx!=m_SelStartx)
		DeleteSelectionText();
	
	m_Text=m_Text.substr(0, m_Caretx)+_char+m_Text.substr(m_Caretx);
	m_MaxCaretx++;
	PlaceCaret(m_Caretx+1);
	if(m_Caretx-m_Scrollx>=((int)(m_width/8))){
		m_Scrollx++;
	}
}
void CGUIInputBox::DeleteTextAtCaret(){
	if(m_Caretx!=0){
		m_Text=m_Text.substr(0, m_Caretx-1)+m_Text.substr(m_Caretx);
		PlaceCaret(m_Caretx-1);
	}
	SyncSelStart();
}
void CGUIInputBox::DeleteSelectionText(){
	if(m_Caretx>m_SelStartx){
		m_Text=m_Text.substr(0, m_SelStartx)+m_Text.substr(m_Caretx, m_Text.length());
		PlaceCaret(m_SelStartx);
	}else{
		m_Text=m_Text.substr(0, m_Caretx)+m_Text.substr(m_SelStartx, m_Text.length());
	}
	SyncSelStart();
};
void CGUIInputBox::PlaceCaret(int _x){
	m_MaxCaretx=(UINT)m_Text.length();
	m_Caretx=min((UINT)max(0, _x), m_MaxCaretx);
	
	if(m_Caretx>=m_Scrollx+((int)(m_width/8))){
		m_Scrollx=m_Caretx-((int)(m_width/8))+1;
	}else if(m_Caretx<m_Scrollx){
		m_Scrollx=m_Caretx;
	}
	m_CaretBlink=30;
	m_bCaretOn=true;
}
void CGUIInputBox::PlaceCaret(POINT _cursor){
	float dx=m_pos.x;
	float dy=m_pos.y;
	CGUIBase* p=m_pParent;
	while(p!=NULL){
		dx+=p->GetPos().x;
		dy+=p->GetPos().y;
		p=p->GetParent();
	}
	PlaceCaret((int)((float)(_cursor.x-dx-m_border-2)/(float)8+0.5f)+m_Scrollx);
}
void CGUIInputBox::SyncSelStart(){
	m_SelStartx=m_Caretx;
}

bool CGUIInputBox::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIInputBox::CanHaveFocus(){ return true; }
void CGUIInputBox::OnFocusIn() { m_bHasFocus = true; }
void CGUIInputBox::OnFocusOut() { m_bHasFocus = false; }
void CGUIInputBox::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUIInputBox::OnMouseLeave() { m_bMouseOver = false; }
void CGUIInputBox::OnHotkey() {
	if(!(m_MaxCaretx==0)){
		OnGUIEvent(m_Event, this, m_pDest);
	}
}
void CGUIInputBox::Step(){
	if(m_CaretBlink>0)
		m_CaretBlink--;
	else{
		m_CaretBlink=30;
		m_bCaretOn=!m_bCaretOn;
	}
	if(m_bButtonDown)
		PlaceCaret(m_MousePos);
}
void CGUIInputBox::Draw(CGraphicsHandler *g){
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

		if(m_Caretx!=m_SelStartx && GetFocus()){
			g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_SelStartx-m_Scrollx)*8, m_pos.y+m_border+2), (m_Caretx-m_SelStartx)*8, 13, ColorARGB(255, 0,0,255));	
		}
		
		if(m_Text!=L""){
			//if(m_Carety==m_SelStarty)
				g->DrawTextTL(m_Text, Vector2(m_pos.x+m_border+2, m_pos.y+m_border+3), m_pColors->Text);
			//else{
				//g->DrawTextTL(m_TextDraw, Vector2(m_pos.x+m_border+2, m_pos.y+m_border+3), ColorARGB(255, 255, 0,0));
			//}
		}
		if(m_bCaretOn && GetFocus())
			g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_Caretx-m_Scrollx)*8, m_pos.y+m_border+2), 1, 13, ColorARGB(255, 0,0,0));

		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}