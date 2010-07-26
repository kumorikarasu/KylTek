#include "main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUITextbox.h"

CGUITextBox::CGUITextBox(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, bool _editable, bool _selectable, LPTSTR _text)
	: CGUIBase(params, colors, sprites){

	m_pVSB=NULL;
	m_pHSB=NULL;

	m_bEditable=_editable;
	m_bSelectable=_selectable;
	m_Type=GUITYPE_TEXTBOX;
	m_Scrollx=0;
	m_Scrolly=0;
	SetText(_text);
	m_Caretx=m_MaxCaretx;
	m_Carety=m_MaxCarety;
	SyncSelStart();
	m_bCaretOn=true;
	m_CaretBlink=40;

	
};

CGUITextBox::~CGUITextBox(){};


bool CGUITextBox::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){

	if(!m_bEnabled)
		return false;

	if(!m_bButtonDown && ((m_pVSB && m_pVSB->WndProc(hWnd, uMsg, wParam, lParam)) || (m_pHSB && m_pHSB->WndProc(hWnd, uMsg, wParam, lParam))))
		return true;

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
bool CGUITextBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
if(GetFocus() && m_bSelectable)
	switch(uMsg){
		case WM_KEYDOWN:
			switch( wParam ){
				case VK_UP:
					PlaceCaret(m_Caretx, m_Carety-1);
					if( GetKeyState( VK_SHIFT ) >= 0 )
						SyncSelStart();
					break;
				case VK_DOWN:
					PlaceCaret(m_Caretx, m_Carety+1);
					if( GetKeyState( VK_SHIFT ) >= 0 )
						SyncSelStart();
					break;
				case VK_LEFT:
					if(m_Caretx==0 && m_Carety!=0)
						PlaceCaret(m_Text[m_Carety-1].length(), m_Carety-1);
					else
						PlaceCaret(m_Caretx-1, m_Carety);

					if( GetKeyState( VK_SHIFT ) >= 0 )
						SyncSelStart();
					break;
				case VK_RIGHT:
					if(m_Caretx==m_MaxCaretx && m_Carety!=m_MaxCarety)
						PlaceCaret(0, m_Carety+1);
					else
						PlaceCaret(m_Caretx+1, m_Carety);

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
					if(m_Caretx==m_SelStartx && m_Carety==m_SelStarty)
						DeleteTextAtCaret();
					else
						DeleteSelectionText();
					break;
				case VK_RETURN:
					if(m_bEditable){
						if(m_Caretx==m_SelStartx && m_Carety==m_SelStarty)
						AddChar('\n');
						SyncSelStart();
					}
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
					if(m_bEditable){
						AddChar((char)wParam);
						SyncSelStart();
					}
			}
			return true;
		default:
			return false;
	}

	return false;
}
bool CGUITextBox::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
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
			if(m_bMouseOver){
				if((int)wParam>0){//Scroll Up
					m_Scrolly=max((int)m_Scrolly-1, 0);
				}else{
					m_Scrolly=min((int)m_Scrolly+1, max((int)m_MaxCarety-((int)(m_height/13))+1, 0));
				}
				//m_Scrollx=min(max(m_Scrollx, 0), m_MaxCaretx+((int)(width/8)));
				SetTextDraw();
			}
			break; 
	}
	if(m_bMouseOver)
		return true;
	return false;
}

void CGUITextBox::AddText(std::wstring _text){
	UINT c=0;
	UINT index=0;
	/*std::wstring temp=m_Text;
	while(temp.find("\n")!=temp.npos){
		index+=(UINT)temp.find("\n");
		if((height/12-2)+Scroll==c || (Scroll==0 && c==m_Carety)){
			m_Text=m_Text.substr(0, index)+m_Text.substr(index+1);
			break;
		}
		c++;
	}*/
	//SetText(m_Text.substr(0,
}
void CGUITextBox::SetText(std::wstring _text){ 
	std::wstring t=_text;
	m_MaxCarety=-1;
	m_Scrolly=0;
	m_Text.clear();

	if(t.find(L"\n")==t.npos){
		if((TCHAR)m_Hotkey==(TCHAR)L"\n")
			OnHotkey();
		else
			AddLine(_text);
	}else{
		while(t.find(L"\n")!=t.npos){
			AddLine(t.substr(0, t.find(L"\n")-1));
			t=t.substr(t.find(L"\n")+1);
		}
	}
}
void CGUITextBox::SetTextDraw(){
	m_TextDraw=L"";	
	for(UINT a=m_Scrolly; a<m_Scrolly+(int)(m_height/13); a++){
		if(a>m_MaxCarety || a>=m_Text.size())
			break;
		m_TextDraw+=m_Text[a]+L"\n";
	}
}
void CGUITextBox::DeleteTextAtCaret(){
	if(m_Caretx!=0){
		m_Text[m_Carety]=m_Text[m_Carety].substr(0, m_Caretx-1)+m_Text[m_Carety].substr(m_Caretx);
		PlaceCaret(m_Caretx-1, m_Carety);
	}else if(m_Carety>0){
		int temp=(int)m_Text[m_Carety-1].length();
		m_Text[m_Carety-1]+=m_Text[m_Carety];
		for(UINT a=m_Carety;a<m_MaxCarety;a++)
			m_Text[a]=m_Text[a+1];
		m_MaxCarety--;
		m_Text.pop_back();
		PlaceCaret(temp, m_Carety-1);
	}
	SyncSelStart();
}
void CGUITextBox::DeleteSelectionText(){

	if(m_Carety==m_SelStarty){
		if(m_Caretx>m_SelStartx){
			m_Text[m_Carety]=m_Text[m_Carety].substr(0, m_SelStartx)+m_Text[m_Carety].substr(m_Caretx, m_Text[m_Carety].length());
			PlaceCaret(m_SelStartx, m_Carety);
		}else{
			m_Text[m_Carety]=m_Text[m_Carety].substr(0, m_Caretx)+m_Text[m_Carety].substr(m_SelStartx, m_Text[m_Carety].length());
		}
	}else if(m_Carety>m_SelStarty){
		int caretx=m_Text[m_SelStarty].substr(0, m_SelStartx).length();
		m_Text[m_SelStarty]=m_Text[m_SelStarty].substr(0,m_SelStartx)+m_Text[m_Carety].substr(m_Caretx);
		for(UINT a=m_SelStarty; a<m_Carety; a++){
			for(UINT b=m_SelStarty+1;b<m_MaxCarety;b++)
				m_Text[b]=m_Text[b+1];
			m_MaxCarety--;
			m_Text.pop_back();
		}
		PlaceCaret(caretx, m_SelStarty);
	}else{
		int caretx=m_Text[m_Carety].substr(0, m_Caretx).length();
		m_Text[m_Carety]=m_Text[m_Carety].substr(0,m_Caretx)+m_Text[m_SelStarty].substr(m_SelStartx);
		for(UINT a=m_Carety; a<m_SelStarty; a++){
			for(UINT b=m_Carety+1;b<m_MaxCarety;b++)
				m_Text[b]=m_Text[b+1];
			m_MaxCarety--;
			m_Text.pop_back();
		}
		PlaceCaret(caretx, m_Carety);
	}
	SyncSelStart();
	SetTextDraw();
};
void CGUITextBox::PlaceCaret(int _x, int _y){
	m_Carety=min((UINT)max(0, _y), m_MaxCarety);
	m_MaxCaretx=(UINT)m_Text[m_Carety].length();
	m_Caretx=min((UINT)max(0, _x), m_MaxCaretx);
	
	if(m_Caretx>=m_Scrollx+((int)(m_width/8))){
		m_Scrollx=m_Caretx-((int)(m_width/8))+1;
	}else if(m_Caretx<m_Scrollx){
		m_Scrollx=m_Caretx;
	}
	if(m_Carety>=m_Scrolly+((int)(m_height/13))){
		m_Scrolly=m_Carety-((int)(m_height/13))+1;
	}else if(m_Carety<m_Scrolly){
		m_Scrolly=m_Carety;
	}
	if(m_pVSB!=NULL)
		m_pVSB->SetValue(m_Scrolly);
	if(m_pHSB!=NULL)
		m_pHSB->SetValue(m_Scrollx);
	SetTextDraw();
	m_CaretBlink=30;
	m_bCaretOn=true;
}
void CGUITextBox::PlaceCaret(POINT _cursor){
	float dx=m_pos.x;
	float dy=m_pos.y;
	CGUIBase* p=m_pParent;
	while(p!=NULL){
		dx+=p->GetPos().x;
		dy+=p->GetPos().y;
		p=p->GetParent();
	}
	PlaceCaret((int)((float)(_cursor.x-dx-m_border-2)/(float)8+0.5f)+m_Scrollx, (int)((float)(_cursor.y-dy-m_border-2)/(float)13)+m_Scrolly);
}
void CGUITextBox::AddChar(TCHAR_char){
	if(m_Caretx!=m_SelStartx || m_Carety!=m_SelStarty)
		DeleteSelectionText();
	
	if(_char!='\n'){
		m_Text[m_Carety]=m_Text[m_Carety].substr(0, m_Caretx)+_char+m_Text[m_Carety].substr(m_Caretx);
		m_MaxCaretx++;
		if(m_pHSB!=NULL && m_MaxCaretx>=((int)(m_width/8)))
			m_pHSB->SetMaxValue(m_pHSB->GetMaxValue()+1);
		PlaceCaret(m_Caretx+1, m_Carety);
		if(m_Caretx-m_Scrollx>=((int)(m_width/8))){
			m_Scrollx++;
			if(m_pHSB!=NULL)
				m_pHSB->SetValue(m_Scrollx);
		}
	}else{
		m_MaxCarety++;
		if(m_pVSB!=NULL && m_MaxCarety>=((int)(m_height/13)))
			m_pVSB->SetMaxValue(m_pVSB->GetMaxValue()+1);
		m_Text.push_back("");
		for(UINT a=m_MaxCarety;a>m_Carety+1;a--)
			m_Text[a]=m_Text[a-1];
		m_Text[m_Carety+1]=m_Text[m_Carety].substr(m_Caretx);
		m_Text[m_Carety]=m_Text[m_Carety].substr(0, m_Caretx);
		PlaceCaret(0, m_Carety+1);
		if(m_Carety-m_Scrolly>=((int)(m_height/13))){
			m_Scrolly++;
			if(m_pVSB!=NULL)
				m_pVSB->SetValue(m_Scrolly);
		}
	}
}
void CGUITextBox::AddLine(std::wstring _text){
	if(m_Text.size()==1 && m_Text[0]==L"")
		m_Text[0]=_text;
	else{
		m_MaxCarety++;
		if(m_pVSB!=NULL && m_MaxCarety>=((int)(m_height/13)))
			m_pVSB->SetMaxValue(m_pVSB->GetMaxValue()+1);
		m_Text.push_back(_text);

		if(m_MaxCarety>=((int)(m_height/13))){
			m_Scrolly++;
			if(m_pVSB!=NULL)
				m_pVSB->SetValue(m_Scrolly);
		}
	}
	PlaceCaret(m_Text[m_MaxCarety].length(),m_MaxCarety);
	SyncSelStart();
}
void CGUITextBox::SyncSelStart(){
	m_SelStartx=m_Caretx;
	m_SelStarty=m_Carety;
}

bool CGUITextBox::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUITextBox::CanHaveFocus(){ return true; }
void CGUITextBox::OnFocusIn() { m_bHasFocus = true; }
void CGUITextBox::OnFocusOut() { m_bHasFocus = false; }
void CGUITextBox::OnMouseEnter() { 
//	SetCursor(g_pCursor);
	m_bMouseOver = true; 
}
void CGUITextBox::OnMouseLeave() { m_bMouseOver = false; }
void CGUITextBox::OnHotkey() {
	
}

void CGUITextBox::EnableVSB(VSBspr* VSBsprlist){
	int HSBpush=0;
	if(m_pHSB!=NULL){
		HSBpush=m_pHSB->GetHeight();
		m_pHSB->Resize(m_pVSB->GetWidth()-20, m_pVSB->GetHeight());
	}
	GUI_PARAMS params(m_pos+Vector2(m_width-20, 0), 20, m_height-HSBpush, 0, 0, true, true, OnGUIEvent);
	m_pVSB = new CGUIVerticalScrollBar(&params, NULL, VSBsprlist);
	m_pVSB->SetParent(m_pParent);
	m_pVSB->SetMinValue(0);
	m_pVSB->SetMaxValue(max(0, (int)(m_MaxCarety-((int)m_height/13))));
	m_pVSB->SetValue(m_Scrolly);
}

void CGUITextBox::EnableHSB(HSBspr* HSBsprlist){
	int VSBpush=0;
	if(m_pVSB!=NULL){
		VSBpush=m_pVSB->GetWidth();
		m_pVSB->Resize(m_pVSB->GetWidth(), m_pVSB->GetHeight()-20);
	}
	GUI_PARAMS params(m_pos+Vector2(0, m_height-20), m_width-VSBpush, 20, 0, 0, true, true, OnGUIEvent);
	m_pHSB = new CGUIHorizontalScrollBar(&params, NULL, HSBsprlist);
	m_pHSB->SetParent(m_pParent);
	m_pHSB->SetMinValue(0);

	m_pHSB->SetMaxValue(max(0, (int)(m_MaxCaretx-((int)m_width/8))));
	m_pHSB->SetValue(m_Scrollx);
}

void CGUITextBox::Step(){
	if(m_CaretBlink>0)
		m_CaretBlink--;
	else{
		m_CaretBlink=30;
		m_bCaretOn=!m_bCaretOn;
	}
	if(m_bButtonDown)
		PlaceCaret(m_MousePos);
	if(m_pVSB!=NULL){
		m_pVSB->Step();
		if(m_Scrolly!=(int)m_pVSB->GetValue()){
			m_Scrolly=(int)m_pVSB->GetValue();
			SetTextDraw();
		}
	}
	if(m_pHSB!=NULL){
		m_pHSB->Step();
		if(m_Scrollx!=(int)m_pHSB->GetValue()){
			m_Scrollx=(int)m_pHSB->GetValue();
			SetTextDraw();
		}
	}
}
void CGUITextBox::Draw(CGraphicsHandler *g){
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

		if((m_Caretx!=m_SelStartx || m_Carety!=m_SelStarty) && GetFocus()){
			if(m_SelStarty==m_Carety && !(m_SelStarty<m_Scrolly) && !(m_SelStarty>=m_Scrolly+((int)(m_height/13))))
				g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_SelStartx-m_Scrollx)*8, m_pos.y+m_border+2+(m_SelStarty-m_Scrolly)*13), (m_Caretx-m_SelStartx)*8, 13, ColorARGB(255, 0,0,255));
			else{
				if(m_Carety>m_SelStarty){
					if(!(m_SelStarty<m_Scrolly) && !(m_SelStarty>=m_Scrolly+((int)(m_height/13))))
						g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_SelStartx-m_Scrollx)*8, m_pos.y+m_border+2+(m_SelStarty-m_Scrolly)*13), m_Text[m_SelStarty].substr(m_SelStartx).length()*8, 13, ColorARGB(255, 0,0,255));
					
					for(UINT a=m_SelStarty+1; a<m_Carety; a++)
						if(!(a<m_Scrolly) && !(a>=m_Scrolly+((int)(m_height/13))))
							g->DrawRectTL(Vector2(m_pos.x+m_border+2+(-(int)m_Scrollx)*8, m_pos.y+m_border+2+(a-m_Scrolly)*13), m_Text[a].length()*8, 13, ColorARGB(255, 0,0,255));
					if(!(m_Carety<m_Scrolly) && !(m_Carety>=m_Scrolly+((int)(m_height/13))))
						g->DrawRectTL(Vector2(m_pos.x+m_border+2+(-(int)m_Scrollx)*8, m_pos.y+m_border+2+(m_Carety-m_Scrolly)*13), m_Text[m_Carety].substr(0, m_Caretx).length()*8, 13, ColorARGB(255, 0,0,255));
				}else{
					if(!(m_SelStarty<m_Scrolly) && !(m_SelStarty>=m_Scrolly+((int)(m_height/13))))
						g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_SelStartx-(int)m_Scrollx)*8, m_pos.y+m_border+2+(m_SelStarty-(int)m_Scrolly)*13), -(int)m_Text[m_SelStarty].substr(0, m_SelStartx).length()*8, 13, ColorARGB(255, 0,0,255));
					
					for(UINT a=m_SelStarty-1; a>m_Carety; a--)
						if(!(a<m_Scrolly) && !(a>=m_Scrolly+((int)(m_height/13))))
							g->DrawRectTL(Vector2(m_pos.x+m_border+2+(-(int)m_Scrollx)*8, m_pos.y+m_border+2+(a-m_Scrolly)*13), m_Text[a].length()*8, 13, ColorARGB(255, 0,0,255));
					if(!(m_Carety<m_Scrolly) && !(m_Carety>=m_Scrolly+((int)(m_height/13))))
						g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_Text[m_Carety].length())*8, m_pos.y+m_border+2+(m_Carety-(int)m_Scrolly)*13), -(int)m_Text[m_Carety].substr(m_Caretx).length()*8, 13, ColorARGB(255, 0,0,255));
				}
			}
		}
		
		if(m_TextDraw!=L"" && m_pColors){
			//if(m_Carety==m_SelStarty)
				g->DrawTextTL(m_TextDraw, Vector2(m_pos.x+m_border+2, m_pos.y+m_border+3), m_pColors->Text);
			//else{
				//g->DrawTextTL(m_TextDraw, Vector2(m_pos.x+m_border+2, m_pos.y+m_border+3), ColorARGB(255, 255, 0,0));
			//}
		}
		if(m_bCaretOn && GetFocus() && m_bSelectable && !(m_Carety<m_Scrolly) && !(m_Carety>=m_Scrolly+((int)(m_height/13))))
			g->DrawRectTL(Vector2(m_pos.x+m_border+2+(m_Caretx-m_Scrollx)*8, m_pos.y+m_border+2+(m_Carety-m_Scrolly)*13), 1, 13, ColorARGB(255, 0,0,0));

		if(m_pVSB!=NULL)
			m_pVSB->Draw(g);
		if(m_pHSB!=NULL)
			m_pHSB->Draw(g);

		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}