#ifndef CAPPLICATION_H_
#define CAPPLICATION_H_

#include "CDeviceHandler.h"
#include "CGraphicsHandler.h"
#include "CGUIBase.h"
#include "CGUIWindow.h"
#include "CLevel.h"

class CApplication{
private:
	CDeviceHandler* m_pDeviceHandler;
	CGraphicsHandler* m_pGraphicsHandler;
	CGUIWindow* m_pGUIWindow;
	CLevel* m_pCurrentLevel;
	
public:
	CApplication();
	~CApplication();

	const static UINT WINDOW_WIDTH=1024;
	const static UINT WINDOW_HEIGHT=768;

	bool WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Initialize(HWND hWnd, HINSTANCE hInstance);

	void Step();
	void Render();
};

#endif