#pragma once
#ifndef CGUIRADIO_H_
#define CGUIRADIO_H_

#include "Foreach.h"

class CGUIRadioGroup;

class CGUIRadioButton : public CGUIBase{
private:
	UINT m_Id;
	bool m_bActive;
	UINT m_Event;
	void* m_pDest;
	CGUIRadioGroup* m_pGroup;
public:
	CGUIRadioButton(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, CGUIRadioGroup* group);
	~CGUIRadioButton();

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

	virtual void Step();
	virtual void Draw(CGraphicsHandler *g);

	bool GetActive(){ return m_bActive; }
	void SetActive(bool active){ m_bActive=active; }
	UINT GetId(){ return m_Id; }
	void SetId(UINT id){ m_Id=id; }
	CGUIRadioGroup* GetGroup(){ return m_pGroup; }
	void SetGroup( CGUIRadioGroup* group ){ m_pGroup=group; }
};

class CGUIRadioGroup{
private:
	std::list<CGUIRadioButton*> m_pButtonList;
	std::list<CGUIRadioButton*>::iterator m_ButtonIt;
	CGUIRadioButton* m_pActiveButton;
	UINT m_numOfButtons;
public:
	CGUIRadioGroup();
	~CGUIRadioGroup();

	void AddButton( GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, UINT _event, void* _dest, bool active);
	void AddButton( CGUIRadioButton* button, bool active );

	CGUIBase* GetButton(UINT id){
		FOREACHL(CGUIRadioButton*, m_pButtonList, m_ButtonIt){
			if(((CGUIRadioButton*)(*m_ButtonIt))->GetId()==id)
				return *m_ButtonIt;
		}
		return NULL;
	};

	void SetActive( UINT _id ){
		if(m_pActiveButton)
			m_pActiveButton->SetActive(false);

		FOREACHL(CGUIRadioButton*, m_pButtonList, m_ButtonIt){
			if(((CGUIRadioButton*)(*m_ButtonIt))!=NULL){
				if(((CGUIRadioButton*)(*m_ButtonIt))->GetId()==_id)
				m_pActiveButton=*m_ButtonIt;
			}
		}
		if(m_pActiveButton)
			m_pActiveButton->SetActive(true);
	};
	UINT GetActive() { return m_pActiveButton->GetId(); }
};

#endif