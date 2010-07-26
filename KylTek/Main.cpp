#include "Main.h"
#include "CDeviceHandler.h"
#include "CGraphicsHandler.h"
#include "CApplication.h"
#include "CollisionManager.h"
#include "CGlobal.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CApplication* pApp;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	WNDCLASS wcex;
	ZeroMemory(&wcex,sizeof(WNDCLASS));
	
	// Provide the address of the function Windows should call with messages	
	wcex.lpfnWndProc= (WNDPROC)WndProc;
	wcex.style= CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance= hInstance;
	wcex.hCursor= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName= L"MainWindow";

	// Register my new window class with the Windows API
	RegisterClass(&wcex);
    DWORD style=WS_POPUP|WS_VISIBLE;
	
	// Create the window
	HWND hWnd = CreateWindowA("MainWindow", "KylTek", 
		style, 0, 0, 1024, 768,
		NULL, NULL, hInstance, NULL);

	if (hWnd==0)
		return -1;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Timer Stuff
	int testt1=0;
	float testt2=0;	

	Global->pColMan = new ColManager();

	pApp = new CApplication();
	pApp->Initialize(hWnd, hInstance);

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT && !Global->Exit)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{	
			pApp->Step();
			if(Global->Exit)
				break;
			pApp->Render();
		}
	}
	
	//Clean up
	SAFE_DELETE(pApp);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
		case WM_DESTROY:
            PostQuitMessage(0);
		default:
			// We do not want to handle this message so pass it back to Windows
			// to handle it in a default way
			if(!(pApp!=NULL && pApp->WndProc(hWnd, uMsg, wParam, lParam)))
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}
