#include "main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "Functions.h"

CGUIWindow::CGUIWindow(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, bool _movable, LPTSTR _title)
	: CGUIBase(params, colors, sprites){

	m_pFocusElement=NULL;
	m_Title=_title;
	m_bMovable = _movable;
	m_pGUIList = new std::list<CGUIBase*>();
	m_pElement=NULL;
	m_Type=GUITYPE_WINDOW;
	m_numOfElements=0;

	m_pDrawSurface=NULL;
};
CGUIWindow::~CGUIWindow(){};

bool SortPredicate(const CGUIBase* lhs, const CGUIBase* rhs){
	return lhs->m_depth > rhs->m_depth;
}

bool CGUIWindow::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(!GetEnabled())
		return false;

	if(!m_bButtonDown){
		if(m_pFocusElement && m_pFocusElement->WndProc( hWnd, uMsg, wParam, lParam ))
			return true;

		m_GUIit= m_pGUIList->begin();
		while(m_GUIit!=m_pGUIList->end()){
			m_pElement = *m_GUIit;
			//If component in the window handles the msg, no need for others to try
			if(m_pElement!=NULL && m_pElement!=m_pFocusElement && m_pElement->WndProc( hWnd, uMsg, wParam, lParam )){
				return true;
			}
			++m_GUIit;
		}
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
bool CGUIWindow::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	switch(uMsg){
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
			switch(wParam){
				case VK_TAB:
					FocusNext();
					return true;
				default:
					return false; 
			}
		default:
			return false;
	}
}
bool CGUIWindow::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
	POINT MousePos;
	GetCursorPos(&MousePos);
	switch(uMsg){
		case WM_MOUSEMOVE:
			if(m_bButtonDown && m_bMovable){
				Move(Vector2((float)(MousePos.x-m_mouseStart.x), (float)(MousePos.y-m_mouseStart.y)));
				m_mouseStart=MousePos;
				return true;
			}
			if(!m_bMouseOver && PtInBB(MousePos) && GetVisible()){
				OnMouseEnter();
				return true;
			}if(m_bMouseOver && !PtInBB(MousePos)){
				OnMouseLeave();
				return true;
			}

			break;
		case WM_LBUTTONDOWN:
			SetFocusElement(NULL);
			if(m_bMouseOver){
				if(!m_bButtonDown)
					if(GetFocus() || RequestFocus()){
						m_bButtonDown=true;
						m_mouseStart=MousePos;
					}
				return true;
			}
			break;
		case WM_LBUTTONUP:
			m_bButtonDown=false;
			break; 
	}
	if(m_bMouseOver)
		return true;
	return false;
}

bool CGUIWindow::FocusNext(){
	if(m_pFocusElement==NULL)
		return false;

	bool nextFocus=false;
	m_GUIit= m_pGUIList->begin();
	while(m_GUIit!=m_pGUIList->end()){
		m_pElement = *m_GUIit;
		if(nextFocus && m_pElement){
			if(m_pElement->RequestFocus())
				return true;
			if(m_pElement==m_pFocusElement && !m_pElement->CanHaveFocus()){
				SetFocusElement(NULL);
				return true;
			}
		}

		if(m_pElement!=NULL && m_pElement->GetFocus()){
			if(nextFocus)
				return false;
			nextFocus=true;
		}
		++m_GUIit;
		if(nextFocus && m_GUIit==m_pGUIList->end())
			m_GUIit=m_pGUIList->begin();
	}
	return false;
}
bool CGUIWindow::RequestFocus() {
	if(GetFocus())
		return true;
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUIWindow::CanHaveFocus(){
	if((m_pParent && ((CGUIWindow*)m_pParent)->GetFocusElement() && ((CGUIWindow*)m_pParent)->GetFocusElement()->GetFocusLock()) || !GetEnabled() || (m_pFocusElement && m_pFocusElement->GetFocusLock()))
		return false; 

	return true;
}
bool CGUIWindow::SetFocusElement(CGUIBase* _element){
	if(m_pFocusElement!=NULL)
		m_pFocusElement->OnFocusOut();
	m_pFocusElement=_element;
	if(m_pFocusElement!=NULL){
		m_pFocusElement->OnFocusIn();
		float d=m_pFocusElement->getDepth();
		m_GUIit= m_pGUIList->begin();
		while(m_GUIit!=m_pGUIList->end()){
			m_pElement = *m_GUIit;
			if(m_pElement && m_pFocusElement->GetType()==GUITYPE_WINDOW && m_pElement->getDepth()>d)
				m_pElement->SetDepth(m_pElement->getDepth()-1);
			++m_GUIit;
		}
		if(m_pFocusElement->GetType()==GUITYPE_WINDOW){
			m_pFocusElement->SetDepth(1);
			m_pGUIList->sort(SortPredicate);
		}
	}
	//if(m_pParent) if u can fix the error, add this, if not, no big deal
		//m_pParent->RequestFocus();
	return true;
}
void CGUIWindow::OnFocusIn() { 
	m_bHasFocus = true; 
	SetVisible(true);
}
void CGUIWindow::OnFocusOut() { 
	m_bHasFocus = false; 
}
void CGUIWindow::OnMouseEnter() { 
	m_bMouseOver = true;
//	g_pCursor=m_pSprites->g_pCursor;
}
void CGUIWindow::OnMouseLeave() {
	m_bMouseOver = false; 
}
void CGUIWindow::OnHotkey() {}

void CGUIWindow::Add(CGUIBase* _GUI){
	if(!_GUI->GetParent())
		_GUI->SetParent(this);
	if(!_GUI->GetEventFunction())
		_GUI->SetEventFunction( OnGUIEvent );
	_GUI->SetDepth((float)m_numOfElements*-1);
	m_pGUIList->push_back(_GUI);
	m_pGUIList->sort(SortPredicate);
	++m_numOfElements;
}

void CGUIWindow::Step(){
	if(GetEnabled()){
		m_GUIit= m_pGUIList->begin();
		while(m_GUIit!=m_pGUIList->end()){
			m_pElement = *m_GUIit;
			if(m_pElement)
				m_pElement->Step();
			++m_GUIit;
		}
	}
}

void CGUIWindow::Draw(CGraphicsHandler *g){
	if(!m_pDrawSurface){
		this;
		IDirect3DSurface9* orig;
		g->GetRenderTarget( 0, &orig );
		
		D3DSURFACE_DESC desc;
		orig->GetDesc( &desc );

		g->CreateRenderTarget( m_width, m_height, D3DFMT_X8R8G8B8, 
                                  desc.MultiSampleType, desc.MultiSampleQuality,
                                  false, &m_pDrawSurface, NULL );
	}
	if(GetVisible()){
		IDirect3DSurface9* orig;
		g->GetRenderTarget( 0, &orig );
		g->SetRenderTarget( 0, m_pDrawSurface );
		g->ClearRenderTarget(NULL);

		g->DrawSprite(g->SprList->Menu->menuBgk,0,Vector2(512,256+128));

		UpdateBB();
		if(m_pSprites){
			if(m_bButtonDown && m_bMouseOver && m_pSprites->Button->MouseDown && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, Vector2(0, 0));
			else if((m_bMouseOver || m_bButtonDown) && m_pSprites->Button->MouseOver && m_bEnabled)
				g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, Vector2(0, 0));
			else if(m_pSprites->Button->Default)
				g->DrawSpriteTL(m_pSprites->Button->Default, 0, Vector2(0, 0));
		}else if(m_pColors)
			g->DrawRectTL(Vector2(0, 0), m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);

		if(m_Title!=L"" && m_pColors)
			g->DrawTextTL(m_Title+L" "+toStr(m_depth), Vector2(4, 4), m_pColors->Text);

		m_ReverseGUIit= m_pGUIList->rbegin();
		while(m_ReverseGUIit!=m_pGUIList->rend()){
			m_pElement = *m_ReverseGUIit;
			if(m_pElement){
				m_pElement->Draw(g);
				//m_pElement->Debug(g);
			}
			m_ReverseGUIit++;
		}

		if(!GetEnabled())
			g->DrawRectTL(Vector2(0, 0), m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, Vector2(0, 0));


		g->SetRenderTarget( 0, orig );
		g->DrawSurface(m_pDrawSurface, m_pos, m_width, m_height, NULL); 
	}
}