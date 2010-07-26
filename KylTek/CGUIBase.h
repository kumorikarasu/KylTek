#pragma once
#ifndef H_GUIBASE
#define H_GUIBASE

#include "CGraphicsHandler.h"

#ifndef GUITYPE
#define GUITYPE
#define GUITYPE_WINDOW				1
#define GUITYPE_TEXTBOX				2
#define GUITYPE_RADIO				3
#define GUITYPE_CHECKBOX			4
#define GUITYPE_BUTTON				5
#define GUITYPE_TABGROUP			6
#define GUITYPE_LABEL				7
#define GUITYPE_INPUTBOX			8
#define GUITYPE_VSB					9
#define GUITYPE_HSB					10
#endif

#ifndef GUIEvents
#define GUIEvents
#define EVENT_TOGGLEFULLSCREEN		1
#define EVENT_NEWGAME				2
#define EVENT_NETWORK				3
#define EVENT_SAVEGAME				4
#define EVENT_EXITGAME				5
#define EVENT_OPENWINDOW			6
#define EVENT_CLOSEWINDOW			7
#define EVENT_CONSOLE				8
#define EVENT_SETGLOBAL				9
#define EVENT_SWITCHTABS			10
#define EVENT_SCROLLUP				11
#define EVENT_SCROLLDOWN			12
#define EVENT_SCROLLLEFT			13
#define EVENT_SCROLLRIGHT			14
#define EVENT_SCROLLMOUSE			15
#endif

class CGUIBase;
typedef void (*EVENTFUNCTION)(UINT Event, CGUIBase* Src, void* Dest);
void OnGUIEvent(UINT Event, CGUIBase* Src, void* Dest);

struct GUI_PARAMS{
	GUI_PARAMS(){
		pos=Vector2(0, 0);
		width=0;
		height=0;
		enabled=false;
		visible=false;
		border=0;
		hotkey=0;
		GUIEvent=NULL;
	}
	GUI_PARAMS(Vector2 _pos, UINT _width, UINT _height, UINT _hotkey, UINT _border, bool _visible, bool _enabled, EVENTFUNCTION _GUIEvent){
		pos=_pos;
		width=_width;
		height=_height;
		enabled=_enabled;
		visible=_visible;
		border=_border;
		hotkey=_hotkey;
		GUIEvent=_GUIEvent;
	}
	Vector2 pos;
	UINT width, height, hotkey, border;
	bool visible, enabled;
	EVENTFUNCTION GUIEvent;
};

struct GUI_BUTTON_SPRITES{
	GUI_BUTTON_SPRITES(){
		Default=NULL;
		MouseOver=NULL;
		MouseDown=NULL;
	}
	GUI_BUTTON_SPRITES(CSprite* _default, CSprite* mouseOver, CSprite* mouseDown){
		Default=_default;
		MouseOver=mouseOver;
		MouseDown=mouseDown;
	}
	CSprite* Default;
	CSprite* MouseOver;
	CSprite* MouseDown;
};
struct GUI_SPRITES{
	GUI_SPRITES(){
		Button = new GUI_BUTTON_SPRITES();
		Button->Default=NULL;
		Button->MouseOver=NULL;
		Button->MouseDown=NULL;
		Cursor=NULL;
		Activated=NULL;
	}
	GUI_SPRITES(GUI_BUTTON_SPRITES* button, CSprite* cursor, CSprite* activated){
		Button = button;
		Cursor=cursor;
		Activated=activated;
	}
	GUI_SPRITES(CSprite* _default, CSprite* mouseover, CSprite* mousedown, CSprite* cursor, CSprite* activated){
		Button = new GUI_BUTTON_SPRITES();
		Button->Default=_default;
		Button->MouseOver=mouseover;
		Button->MouseDown=mousedown;
		Cursor=cursor;
		Activated=activated;
	}
	GUI_BUTTON_SPRITES* Button;
	CSprite* Activated; 
	CSprite* Cursor;
};
struct GUI_COLORS{
	GUI_COLORS(){
		BG=NULL;
		Border=NULL;
		MouseOver=NULL;
		MouseDown=NULL;
		Text=NULL;
	}
	GUI_COLORS(Color defaultcolor){
		BG=defaultcolor;
		Border=defaultcolor;
		MouseOver=defaultcolor;
		MouseDown=defaultcolor;
		Text=defaultcolor;
	}
	GUI_COLORS(Color bg, Color border, Color mouseover, Color mousedown, Color text){
		BG=bg;
		Border=border;
		MouseOver=mouseover;
		MouseDown=mousedown;
		Text=text;
	}
	Color BG;
	Color Border;
	Color MouseOver;
	Color MouseDown;
	Color Text;
};

class CGUIBase{
protected:
	RECT m_BoundingBox;
	bool m_bEnabled;
	bool m_bVisible;
	bool m_bMouseOver, m_bButtonDown;
	bool m_bHasFocus, m_bLockFocus, m_bCanFocus;
	UINT m_Type;
	GUI_SPRITES* m_pSprites;
	GUI_COLORS* m_pColors;
	CGUIBase* m_pParent;
	
	EVENTFUNCTION OnGUIEvent;

	Vector2 m_pos;
	UINT m_width, m_height, m_Hotkey, m_border;
	  
	
public:
	CGUIBase(){
		Init(NULL, NULL, NULL);
	};
	CGUIBase(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites){
		Init(params, colors, sprites);
	};
	~CGUIBase(){};

	float m_depth;

	void Init(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites){
		m_pParent=NULL;
		if(params){
			m_pos=params->pos;
			m_width=params->width;
			m_height=params->height;
			m_Hotkey=params->hotkey;
			m_bVisible=params->visible;
			m_bEnabled=params->enabled;
			m_border=params->border;
			OnGUIEvent=params->GUIEvent;
			if(!OnGUIEvent && m_pParent)
				OnGUIEvent=m_pParent->GetEventFunction();
			UpdateBB();
		}else{
			m_pos=Vector2(0, 0);
			m_width=0;
			m_height=0;
			m_Hotkey=0;
			m_bVisible=false;
			m_bEnabled=false;
			m_border=0;
			m_pParent=NULL;
			OnGUIEvent=NULL;
			UpdateBB();
		}
		m_depth=0;

		if(sprites)
			m_pSprites=new GUI_SPRITES(*sprites);
		else
			m_pSprites=NULL;

		if(colors)
			m_pColors=new GUI_COLORS(*colors);
		else
			m_pColors=NULL;
		
		m_bMouseOver=false;
		m_bButtonDown=false;
		m_bHasFocus=false;
		m_bLockFocus=false;
		m_bCanFocus=false;
	}

	virtual bool WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }
	virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }
	virtual bool HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }

	void SetEventFunction(EVENTFUNCTION _func){ OnGUIEvent=_func; }
	EVENTFUNCTION GetEventFunction(){ return OnGUIEvent; }

	virtual void UpdateBB(){
		Vector2 m_realpos=m_pos;
		CGUIBase* p=m_pParent;
		while(p){
			m_realpos+=p->GetPos();
			p=p->GetParent();
		}
		m_BoundingBox.left=(long)m_realpos.x;
		m_BoundingBox.right=(long)m_realpos.x+m_width;
		m_BoundingBox.top=(long)m_realpos.y;
		m_BoundingBox.bottom=(long)m_realpos.y+m_height;
	}
	virtual void Move(Vector2 pos){ 
		m_pos+=pos;
		if(m_pParent){
			m_pos.x=min(max(m_pos.x, 0), m_pParent->GetWidth()-m_width);
			m_pos.y=min(max(m_pos.y, 0), m_pParent->GetHeight()-m_height);
		}
		UpdateBB();
	}
	virtual void Resize(int width, int height){
		m_width=width;
		m_height=height;
		UpdateBB();
	}

	bool GetFocus() { return m_bHasFocus; }
	virtual bool RequestFocus() { return false; }
	virtual bool CanHaveFocus() { return false; }
	virtual void OnFocusIn() { m_bHasFocus = true; }
	virtual void OnFocusOut() { m_bHasFocus = false; }
	virtual void OnMouseEnter() { m_bMouseOver = true; }
	virtual void OnMouseLeave() { m_bMouseOver = false; }
	virtual void OnHotkey() {}

	UINT GetType(){ return m_Type; }
	void SetHotkey( UINT _Hotkey ) { m_Hotkey = _Hotkey; }
	UINT GetHotkey() { return m_Hotkey; }
	void SetVisible( bool _Visible ) { m_bVisible=_Visible; }
	bool GetVisible() { return m_bVisible; }
	void SetEnabled( bool _Enabled ) { m_bEnabled=_Enabled; }
	bool GetEnabled() { return m_bEnabled; }
	void SetFocusLock( bool _locked ) { m_bLockFocus=_locked; }
	bool GetFocusLock() { return m_bLockFocus; }
	void SetDepth(float _depth){m_depth=_depth;}
	float getDepth(){return m_depth;}
	virtual void SetParent(CGUIBase* _parent) { m_pParent=_parent; }
	CGUIBase* GetParent() { return m_pParent; }
	Vector2 GetPos(){ return m_pos; }
	void SetPos(Vector2 pos){ m_pos=pos; }
	int GetWidth(){ return m_width; }
	int GetHeight(){ return m_height; }

	GUI_SPRITES* GetSprites() { return m_pSprites; }
	void SetSprites( GUI_SPRITES* _gspr ) { m_pSprites=_gspr; }
	GUI_COLORS* GetColors() { return m_pColors; }
	void SetColors( GUI_COLORS* _gcolors ) { m_pColors=_gcolors; }

	bool PtInBB(POINT _pt){ if(PtInRect(&m_BoundingBox, _pt))return true; return false; }
	bool PtInBB(Vector2 _pt){ 
		POINT pt={(long)_pt.x, (long)_pt.y};
		return PtInBB(pt); 
	}

	void Close(){
		OnFocusOut();
		SetVisible(false);
		SetEnabled(false);
	}
	void Open(){
		SetEnabled(true);
		RequestFocus();
	}

	virtual void Step(){}
	virtual void Draw(CGraphicsHandler *g){}
	void Debug(CGraphicsHandler *g){
	if(m_bButtonDown)
		g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 255, 0, 255));
	}
};

#endif