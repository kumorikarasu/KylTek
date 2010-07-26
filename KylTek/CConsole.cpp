#include "Main.h"
#include "CConsole.h"
#include "Debug.h"
#include "Functions.h"
#include "CGlobal.h"
#include "Ai_Cop.h"

CConsole::CConsole(GUI_PARAMS* params, GUI_COLORS* _colors, GUI_SPRITES* _sprites, bool _movable, LPTSTR _title)
	: CGUIWindow(params, _colors, _sprites, _movable, _title)
{

	GUI_PARAMS Params;
	Params.pos=Vector2((float)GetWidth()-24, 4);
	Params.width=20;
	Params.height=20;
	Params.hotkey=VK_SPACE;
	Params.border=0;
	Params.enabled=true;
	Params.visible=true;

	Add(new CGUIButton(&Params, _colors, NULL, EVENT_CLOSEWINDOW, this, "X", false));

	GUI_COLORS Colors;
	Colors.BG=ColorARGB(255, 80, 80, 80);
	Colors.Border=ColorARGB(255, 0, 0, 0);
	Colors.Text=ColorARGB(255, 0,0,0);

	Params.pos=Vector2(12, 24);
	Params.width=GetWidth()-24;
	Params.height=GetHeight()-64;
	Params.hotkey=0;
	Params.border=2;
	Params.enabled=true;
	Params.visible=true;
	m_pTextBox=new CGUITextBox(&Params, &Colors, NULL, false, true, "");

	Params.pos=Vector2(12, (float)GetHeight()-32);
	Params.width=GetWidth()-24;
	Params.height=24;
	Params.hotkey=VK_RETURN;
	Params.border=2;
	Params.enabled=true;
	Params.visible=true;
	m_pEditBox = new CGUIInputBox(&Params, &Colors, NULL, ANYCHAR, EVENT_CONSOLE, this, "");

	Add(m_pTextBox);
	Add(m_pEditBox);

	
}
CConsole::~CConsole(){
	SAFE_DELETE(m_pTextBox);
	SAFE_DELETE(m_pEditBox);
}

void CConsole::Step(){
	CGUIWindow::Step();
}

void CConsole::Draw(CGraphicsHandler *g){
	CGUIWindow::Draw(g);
	if(m_pTextBox->GetVSB()==NULL)
		m_pTextBox->EnableVSB(g->SprList->GUI->VSB);
	if(m_pTextBox->GetHSB()==NULL)
		m_pTextBox->EnableHSB(g->SprList->GUI->HSB);
}

void CConsole::AddLine(std::wstring text){
	m_pTextBox->AddLine(text);
}
void CConsole::Command(std::wstring str){
	if(str==L"")
		return;
	std::wstring cmd = str.substr(0, str.find_first_of(' '));
	if(cmd==L""){
		Command(str.substr(1, str.length()));
		return;
	}
	str=str.substr(str.find_first_of(' ')+1);
	int value;
	value = toInt(str);
	
	if(cmd==L"debug"){
		if(value==0){
			Debug->unsetFlags(CDebug::Enabled);
			AddLine(L"Debug: Off");
		}else if(value==1){
			Debug->setFlags(CDebug::Enabled);
			AddLine(L"Debug: On");
		}else{
			AddLine(L"Debug: Invalid value");
		}
	}
	else if(cmd==L"quadtree"){
		if(value==0){
			Debug->unsetFlags(CDebug::QuadCells);
			AddLine(L"Show QuadTree Cells Disabled");
		}else if(value==1){
			Debug->setFlags(CDebug::QuadCells);
			AddLine(L"Show QuadTree Cells Enabled");
		}else{
			AddLine(L"Invalid value");
		}
	}
	else if(cmd==L"hitboxes"){
		if(value==0){
			Debug->unsetFlags(CDebug::HitBoxes);
			AddLine(L"Show HitBoxes Disabled");
		}else if(value==1){
			Debug->setFlags(CDebug::HitBoxes);
			AddLine(L"Show HitBoxes Enabled");
		}else{
			AddLine(L"Invalid value");
		}
	}
	else if(cmd==L"netgraph"){
		if(value==0){
			Debug->unsetFlags(CDebug::NetGraph);
			AddLine(L"Show NetGraph Disabled");
		}else if(value==1){
			Debug->setFlags(CDebug::NetGraph);
			AddLine(L"Show NetGraph Enabled");
		}else{
			AddLine(L"Invalid value");
		}
	}else if(cmd==L"AddEntity"){
		if(value==201){
			ENTITY_PARAMS Params;

			Params.pos=Vector2(250, 100);
			Params.width=32;
			Params.height=32;
			Params.accel=Vector2(0, 0);
			Params.stateflags=CEntity::EmptyState;
			Params.depth=0;
			Params.colTYPE = AABB;

			Global->EntList->add(new Ai_Cop(&Params));

			AddLine(L"AI Cop Created @ 250, 100");
		}else{
			AddLine(L"Invalid value");
		}
	}else if(cmd==L"fill"){
		for(int a=0;a<value;a++)
			AddLine(L"Dick #"+toStr(a));
	}else{
		AddLine(L"Invalid Command: "+cmd);
	}
}

