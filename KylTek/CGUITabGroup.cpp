#include "main.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CGUIButton.h"
#include "CGUITabGroup.h"
#include "Functions.h"

CGUITabGroup::CGUITabGroup(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites)
	: CGUIBase(params, colors, sprites){
	m_pTabList = new std::list<Tab*>();
	m_pActiveTab=NULL;
	m_Type=GUITYPE_TABGROUP;
	m_numOfTabs=0;
	m_nextTabStartx=0;
};
CGUITabGroup::~CGUITabGroup(){};


bool CGUITabGroup::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(!GetEnabled())
		return false;
	m_TabIt= m_pTabList->begin();
	while(m_TabIt!=m_pTabList->end()){
		if(((Tab*)(*m_TabIt))!=NULL && ((Tab*)(*m_TabIt))->Button->WndProc( hWnd, uMsg, wParam, lParam))
			return true;
		++m_TabIt;
	}
	if(m_pActiveTab!=NULL && m_pActiveTab->Window!=NULL && m_pActiveTab->Window->WndProc( hWnd, uMsg, wParam, lParam)){
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
bool CGUITabGroup::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
	if(GetFocus() && wParam==m_Hotkey){
		OnHotkey();
		return true;
	}
	switch(uMsg){
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
			switch(wParam){
				case VK_TAB:
					//FocusNext();
					return true;
				default:
					return false; 
			}
		default:
			return false;
	}
}
bool CGUITabGroup::HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) { 
	POINT MousePos;
	GetCursorPos(&MousePos);
	switch(uMsg){
		case WM_MOUSEMOVE:
			if(!m_bMouseOver && PtInBB(MousePos) && GetVisible()){
				OnMouseEnter();
				return true;
			}if(m_bMouseOver && !PtInBB(MousePos)){
				OnMouseLeave();
				return true;
			}

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
			m_bButtonDown=false;
			break;
	}
	if(m_bMouseOver)
		return true;
	return false;
}
bool CGUITabGroup::RequestFocus() {
	if(m_pParent){
		if(CanHaveFocus()){
			return ((CGUIWindow*)m_pParent)->SetFocusElement(this);
		}
	}
	return false;
}
bool CGUITabGroup::CanHaveFocus(){ return true; }
void CGUITabGroup::OnFocusIn() { 
	m_bHasFocus = true; 
	SetVisible(true);
}
void CGUITabGroup::OnFocusOut() { 
	m_bHasFocus = false; 
}
void CGUITabGroup::OnMouseEnter() { 
	m_bMouseOver = true;
//	g_pCursor=m_pSprites->g_pCursor;
}
void CGUITabGroup::OnMouseLeave() {
	m_bMouseOver = false; 
}
void CGUITabGroup::OnHotkey() {}

void CGUITabGroup::AddTab(std::wstring title, Vector2 buttonDimensions, GUI_COLORS* buttoncolors, GUI_SPRITES* buttonsprites, GUI_COLORS* windowcolors, GUI_SPRITES* windowsprites){
	m_TabIt= m_pTabList->begin();
	while(m_TabIt!=m_pTabList->end()){
		if(((Tab*)(*m_TabIt))!=NULL && ((Tab*)(*m_TabIt))->Title==title){
			title+=L"?";
			AddTab(title, buttonDimensions, buttoncolors, buttonsprites, windowcolors, windowsprites);
			return;
		}
		++m_TabIt;
	}
	
	Tab* pNewTab = new Tab();

	pNewTab->ListID = m_numOfTabs;
	++m_numOfTabs;

	pNewTab->Title=title;

	GUI_PARAMS Params;
	Params.pos=m_pos+Vector2((float)m_nextTabStartx,0);
	m_nextTabStartx+=(UINT)buttonDimensions.x;
	Params.border=2;
	Params.width=(UINT)buttonDimensions.x;
	Params.height=(UINT)buttonDimensions.y;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;
	pNewTab->Button = new CGUIButton(&Params, buttoncolors, buttonsprites, EVENT_SWITCHTABS, this, title.c_str(), false);
	pNewTab->Button->SetParent(m_pParent);

	ZeroMemory(&Params, sizeof(GUI_PARAMS));
	Params.pos=m_pos+Vector2(0,buttonDimensions.y);
	Params.border=2;
	Params.width=m_width;
	Params.height=m_height-(UINT)buttonDimensions.y;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;
	pNewTab->Window = new CGUIWindow(&Params, windowcolors, windowsprites, false, "");
	pNewTab->Window->SetParent(GetParent());

	m_pTabList->push_back(pNewTab);
	ActivateTab(pNewTab->ListID);
}
void CGUITabGroup::ActivateTab(UINT ListID){
	if (ListID>=m_numOfTabs)
		return;
	
	if(m_pActiveTab){
		m_pActiveTab->Window->SetEnabled(false);
		m_pActiveTab->Window->SetVisible(false);
	}
	m_TabIt= m_pTabList->begin();
	for(UINT a=0;a<ListID;++a)
		m_TabIt++;
	m_pActiveTab=((Tab*)(*m_TabIt));
	m_pActiveTab->Window->SetVisible(true);
	m_pActiveTab->Window->SetEnabled(true);

}
void CGUITabGroup::ActivateTab(std::wstring title){
	m_TabIt= m_pTabList->begin();
	while(m_TabIt!=m_pTabList->end()){
		if(((Tab*)(*m_TabIt))!=NULL && ((Tab*)(*m_TabIt))->Title==title)
			ActivateTab(((Tab*)(*m_TabIt))->ListID);
		++m_TabIt;
	}
}

void CGUITabGroup::AddToTabWindow(UINT ListID, CGUIBase* _GUI){
	if (ListID>=m_numOfTabs)
		return;

	m_TabIt= m_pTabList->begin();
	for(UINT a=0;a<ListID;++a)
		m_TabIt++;
	((Tab*)(*m_TabIt))->Window->Add(_GUI);

}
void CGUITabGroup::AddToTabWindow(std::wstring title, CGUIBase* _GUI){
	m_TabIt= m_pTabList->begin();
	while(m_TabIt!=m_pTabList->end()){
		if(((Tab*)(*m_TabIt))!=NULL && ((Tab*)(*m_TabIt))->Title==title)
			AddToTabWindow(((Tab*)(*m_TabIt))->ListID, _GUI);
		++m_TabIt;
	}
}

void CGUITabGroup::Step(){
	if(m_pActiveTab!=NULL && m_pActiveTab->Window!=NULL){
		m_pActiveTab->Window->Step();
	}
}

void CGUITabGroup::Draw(CGraphicsHandler *g){
	if(GetVisible()){
		UpdateBB();

		m_TabIt= m_pTabList->begin();
		while(m_TabIt!=m_pTabList->end()){
			if(((Tab*)(*m_TabIt))!=NULL){
				((Tab*)(*m_TabIt))->Button->Draw(g);
				if(((Tab*)(*m_TabIt))==m_pActiveTab)
					g->DrawRectTL(((Tab*)(*m_TabIt))->Button->GetPos(), ((Tab*)(*m_TabIt))->Button->GetWidth(), ((Tab*)(*m_TabIt))->Button->GetHeight(), ColorARGB(100, 255, 255, 255), 0, NULL);
			}
			++m_TabIt;
		}
		if(m_pActiveTab!=NULL)
			m_pActiveTab->Window->Draw(g);

		if(!GetEnabled())
			g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
		else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
			g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
	}
}