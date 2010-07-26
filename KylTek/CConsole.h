/*
Commands so far are

debug 0/1 - turn debug mode off/on
debug_showhitboxes 0/1
debug_showquadtree 0/1
debug_netgraph 0/1
*/
#pragma once
#ifndef CCONSOLE_H_
#define CCONSOLE_H_

#include "GUI.h"

class CConsole : public CGUIWindow{
private:
	CGUITextBox* m_pTextBox;
	CGUIInputBox* m_pEditBox;
public:
	CConsole(GUI_PARAMS* params, GUI_COLORS* _colors, GUI_SPRITES* _sprites, bool _movable, LPTSTR _title);
	~CConsole();
	
	void Step();
	void Draw(CGraphicsHandler *g);

	void AddLine(std::wstring text);
	void Command(std::wstring cmd);
};

#endif
