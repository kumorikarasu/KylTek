#include "Main.h"
#include "Debug.h"
#include "CGlobal.h"
#include "Functions.h"

CDebug* CDebug::m_pInstance = 0;
CDebug* CDebug::GetInstance(){
	if (m_pInstance == 0) {
        m_pInstance = new CDebug;
    }
    return m_pInstance;
}

CDebug::CDebug(){
	m_flags=Enabled;

	GUI_COLORS Colors;
	Colors.Text=ColorARGB(255,0,255,0);

	GUI_PARAMS Params;
	Params.pos=Vector2(32,32);
	Params.width=512;
	Params.height=256;
	Params.enabled=true;
	Params.visible=true;
	Params.GUIEvent=OnGUIEvent;

	m_pDisplay = new CGUITextBox(&Params, &Colors, NULL, false, false, "Debug: ");

	setFlags(Enabled);

	Params.pos=Vector2(200, 200);
	Params.width=640;
	Params.height=480;
	Params.border=2;
	Params.enabled=true;
	Params.visible=true;

	ZeroMemory(&Colors, sizeof(GUI_COLORS));
	Colors.BG=ColorARGB(255, 100, 100, 100);
	Colors.Border=ColorARGB(255, 0,0,0);
	Colors.Text=ColorARGB(255, 255,255,255);
	Colors.MouseDown=ColorARGB(255, 128, 128, 128);
	Colors.MouseOver=ColorARGB(255,255,255,255);

	m_pConsole = new CConsole(&Params, &Colors, NULL, true, "Console");
}
CDebug::~CDebug(){
	SAFE_DELETE(m_pDisplay);
	SAFE_DELETE(m_pConsole);
}

void CDebug::Draw(CGraphicsHandler *g){
	if(CheckFlag(Enabled)){
		if(CheckFlag(NetGraph)){
			std::wstring txt=L"fps: N/A \tCursor X: "+toStr(Global->MousePos.x);
			txt+=L"\nms:  N/A \tCursor Y: "+toStr(Global->MousePos.y);
			g->DrawTextTL(txt, Vector2(800, 600), ColorARGB(255, 0, 255, 0));
		}
	}
}
