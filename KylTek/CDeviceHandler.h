#ifndef CDEVICEHANDLER_H_
#define CDEVICEHANDLER_H_

enum CONTROL{
	empty, 
	PlayerUp=DIK_W,
	PlayerDown=DIK_S, 
	PlayerLeft=DIK_A, 
	PlayerRight=DIK_D
};

class CDeviceHandler{
private:
	LPDIRECTINPUT8 m_pD3DInputObject;
	LPDIRECTINPUTDEVICE8 m_pD3DMouseDevice; //Mouse
	LPDIRECTINPUTDEVICE8 m_pD3DInputDevice; //Keyboard

	BYTE keys[256];
	DIMOUSESTATE mouse_state;
	bool m_bKeyDown[256];
	bool m_bButtonDown[4];
public:
	CDeviceHandler(HINSTANCE hInstance);
	~CDeviceHandler();

	HRESULT InitializeMouse(HWND hWnd);
	HRESULT InitializeKeyboard(HWND hWnd);
	//HRESULT InitializeJoystick(HWND hWnd);

	void UpdateKeyboardState();
	void UpdateMouseState();

	bool ReassignKey(UINT Key, CONTROL control);
	bool ReassignMouseButton(UINT Button, CONTROL control);

	bool KeyDown();
	bool KeyDown(UINT Key);

	bool KeyPress(UINT Key);
	bool KeyRelease(UINT Key);

	bool MouseButtonDown();
	bool MouseButtonDown(UINT Button);

	bool MouseButtonPress(UINT Key);
	bool MouseButtonRelease(UINT Key);

	Vector2 MousePos();

};

#endif