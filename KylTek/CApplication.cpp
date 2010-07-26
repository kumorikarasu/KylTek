#include "Main.h"
#include "CDeviceHandler.h"
#include "CGraphicsHandler.h"
#include "GUI.h"
#include "CApplication.h"

#include "Debug.h"
#include "Functions.h"
#include "CGlobal.h"
#include "Sprite.h"

CApplication::CApplication(){
	m_pDeviceHandler = NULL;
	m_pGraphicsHandler = NULL;
	m_pGUIWindow = NULL;
	m_pCurrentLevel = NULL;
}
CApplication::~CApplication(){
	SAFE_DELETE(m_pDeviceHandler);
	SAFE_DELETE(m_pGraphicsHandler);
	SAFE_DELETE(m_pGUIWindow);
	SAFE_DELETE(m_pCurrentLevel);
}

void CApplication::Initialize(HWND hWnd, HINSTANCE hInstance){
	m_pGraphicsHandler=new CGraphicsHandler(CGraphicsHandler::DirectX);
	m_pGraphicsHandler->InitializeAPI(hWnd, hInstance);
	m_pGraphicsHandler->Initialize2D();
	m_pGraphicsHandler->SetupSprites();

	m_pDeviceHandler = new CDeviceHandler(hInstance);
	m_pDeviceHandler->InitializeKeyboard(hWnd);
	m_pDeviceHandler->InitializeMouse(hWnd);

	m_pCurrentLevel = new CLevel();

/*-----------------------------------------------------------------------------------------------------------------
- Main Menu
-----------------------------------------------------------------------------------------------------------------*/

	GUI_COLORS Colors;
	Colors.BG=ColorARGB(128,0,0,0);

	GUI_PARAMS Params;
	Params.pos=Vector2(0, 0);
	Params.width=WINDOW_WIDTH;
	Params.height=WINDOW_HEIGHT;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;
	m_pGUIWindow=new CGUIWindow(&Params, &Colors, NULL, false, "");
	m_pGUIWindow->SetEventFunction( NULL );

	m_pGUIWindow->Add(Debug->GetConsole());
	Debug->GetConsole()->Close();

	ZeroMemory(&Colors, sizeof(GUI_COLORS));
	Colors.Text=ColorARGB(255,255,255,255);

	ZeroMemory(&Params, sizeof(GUI_PARAMS));
	Params.pos=Vector2(100, 500);
	Params.width=96;
	Params.height=24;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;

	m_pGUIWindow->Add(new CGUIButton(&Params, &Colors, NULL, EVENT_NEWGAME, m_pCurrentLevel, "New Game", false));
	Params.pos=Vector2(100, 532);
	m_pGUIWindow->Add(new CGUIButton(&Params, &Colors, NULL, EVENT_NETWORK, m_pCurrentLevel, "Online", false));
	Params.pos=Vector2(100, 564);
	//m_pGUIWindow->Add(new CGUIButton(&Params, &Colors, NULL, EVENT_OPENWINDOW, NULL, "Options"));
	//Params.pos=Vector2(100, 596);
	m_pGUIWindow->Add(new CGUIButton(&Params, &Colors, NULL, EVENT_EXITGAME, this, "Exit Game", false));


/*-----------------------------------------------------------------------------------------------------------------
- End of Menu
-----------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------
- Options Menu
-----------------------------------------------------------------------------------------------------------------*/
	/*
	ZeroMemory(&Params, sizeof(GUI_PARAMS));
	Params.pos=Vector2(400, 200);
	Params.width=600;
	Params.height=400;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;

	ZeroMemory(&Colors, sizeof(GUI_COLORS));
	Colors.BG=ColorARGB(255, 80, 80, 80);
	Colors.Border=ColorARGB(255, 0, 0, 0);
	Colors.Text=ColorARGB(255, 0,0,0);

	CGUITabGroup* test = new CGUITabGroup(&Params, &Colors, NULL);
	m_pGUIWindow->Add(test);
	test->AddTab("Tab 1", Vector2(64, 24), &Colors, NULL, &Colors, NULL);
	test->AddTab("Tab 2", Vector2(64, 24), &Colors, NULL, &Colors, NULL);
	test->AddTab("Tab 3", Vector2(64, 24), &Colors, NULL, &Colors, NULL);
	test->AddTab("Tab 4", Vector2(64, 24), &Colors, NULL, &Colors, NULL);
	

	ZeroMemory(&Colors, sizeof(GUI_COLORS));
	Colors.Text=ColorARGB(255,255,255,255);

	ZeroMemory(&Params, sizeof(GUI_PARAMS));
	Params.pos=Vector2(0, 0);
	Params.width=96;
	Params.height=24;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;

	test->AddToTabWindow(0, new CGUIButton(&Params, &Colors, NULL, EVENT_EXITGAME, this, "Exit Game", false));
	test->AddToTabWindow(2, new CGUILabel(&Params, &Colors, NULL, "Label Test"));
	Params.border=2;
	Colors.BG=ColorARGB(255, 100,100,100);
	Colors.Border=ColorARGB(255,0,0,0);
	test->AddToTabWindow(1, new CGUICheckBox(&Params, &Colors, NULL, EVENT_TOGGLEFULLSCREEN, NULL, true));
	Debug->ToConsole("Test Console Output");
	Debug->ToConsole("Commands");
	Debug->ToConsole("quadtree 0 - 1 : Show Quadtree");
	Debug->ToConsole("hitboxes 0 - 1 : Show Hitboxes");
	Debug->ToConsole("AddEntity 201 : Add Another AI to the level");
	Debug->ToConsole("fill # : using for testing scroll bar, gunne be removed");
	Params.pos=Vector2(0, 0);
	Params.width=24;
	Params.height=24;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;
	Colors.BG=ColorARGB(255, 255, 0, 0);
	Colors.Border=ColorARGB(255, 0,0,0);
	Colors.Text=ColorARGB(255, 0,0,0);
	CGUIRadioGroup* rg = new CGUIRadioGroup();
	rg->AddButton(&Params, &Colors, NULL, 0, NULL, false);
	Params.pos=Vector2(24, 0);
	rg->AddButton(&Params, &Colors, NULL, 0, NULL, false);
	Params.pos=Vector2(48, 0);
	rg->AddButton(&Params, &Colors, NULL, 0, NULL, false);
	test->AddToTabWindow(3, rg->GetButton(0));
	test->AddToTabWindow(3, rg->GetButton(1));
	test->AddToTabWindow(3, rg->GetButton(2));
*/
/*-----------------------------------------------------------------------------------------------------------------
- End of Options Menu
-----------------------------------------------------------------------------------------------------------------*/
}

void CApplication::Step(){
	m_pDeviceHandler->UpdateKeyboardState();
	m_pDeviceHandler->UpdateMouseState();

	Global->MousePos=m_pDeviceHandler->MousePos();

	if(m_pDeviceHandler->KeyDown(DIK_F10))
		Global->Exit=true;

	if(m_pDeviceHandler->KeyPress(DIK_GRAVE))
		if(Debug->GetConsole()->GetVisible()){
			Debug->GetConsole()->Close();
			if(m_pGUIWindow->GetVisible()){
				m_pGUIWindow->Close();
			}
		}else{
			Debug->GetConsole()->Open();
				if(!m_pGUIWindow->GetVisible()){
					m_pGUIWindow->Open();
					m_pGUIWindow->OnFocusIn();
				}
		}

	if(m_pDeviceHandler->KeyPress(DIK_ESCAPE)){
		if(m_pGUIWindow->GetVisible()){
			m_pGUIWindow->Close();
		}else{
			m_pGUIWindow->Open();
			m_pGUIWindow->OnFocusIn();
		}
	}
	m_pGUIWindow->Step();

	m_pCurrentLevel->Step(m_pDeviceHandler);
}

void CApplication::Render(){
	m_pGraphicsHandler->BeginRender();
	
	m_pCurrentLevel->Draw(m_pGraphicsHandler);

	Debug->Draw(m_pGraphicsHandler);
	/*if(m_pGUIWindow->GetVisible())
		if (Global->pPlayer!=NULL)
			m_pGraphicsHandler->DrawRectTL(Vector2(0, 0), 1024, 768, ColorARGB(100, 0, 0, 0));
		else
			m_pGraphicsHandler->DrawSprite(	m_pGraphicsHandler->SprList->Menu->menuBgk,	0,	Vector2(512,384));
	*/
	m_pGUIWindow->Draw(m_pGraphicsHandler);

	m_pGraphicsHandler->EndRender();
}

bool CApplication::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	if(m_pGUIWindow)
		return m_pGUIWindow->WndProc(hWnd, uMsg, wParam, lParam);
	return false;
}