#include "Main.h"
#include "CGlobal.h"
#include "CDeviceHandler.h"

//Take out all DX stuff not related with GFX, keyboard...
CDeviceHandler::CDeviceHandler(HINSTANCE hInstance){
	m_pD3DInputObject = NULL;
	m_pD3DInputDevice = NULL;
	m_pD3DMouseDevice = NULL;
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pD3DInputObject, NULL);

	ZeroMemory(&m_bKeyDown, sizeof(m_bKeyDown));

}
CDeviceHandler::~CDeviceHandler(){
	SAFE_RELEASE(m_pD3DInputObject);
	SAFE_RELEASE(m_pD3DInputDevice);
	SAFE_RELEASE(m_pD3DMouseDevice);
}

HRESULT CDeviceHandler::InitializeMouse(HWND hWnd){
	if (FAILED(m_pD3DInputObject->CreateDevice(GUID_SysMouse, &m_pD3DMouseDevice, NULL)))
		return false;
	if (FAILED(m_pD3DMouseDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND |
											DISCL_NONEXCLUSIVE)))
		return false;
	if (FAILED(m_pD3DMouseDevice->SetDataFormat(&c_dfDIMouse)))
		return false;
	if (FAILED(m_pD3DMouseDevice->Acquire()))
		return false;

	return true;
}

HRESULT CDeviceHandler::InitializeKeyboard(HWND hWnd){
	if (FAILED(m_pD3DInputObject->CreateDevice(GUID_SysKeyboard, &m_pD3DInputDevice, NULL))) return false;
	if (FAILED(m_pD3DInputDevice->SetDataFormat( &c_dfDIKeyboard ))) return false;
	if (FAILED(m_pD3DInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;

	ZeroMemory(keys, sizeof(keys) );
	m_pD3DInputDevice->GetDeviceState( sizeof(keys), keys );

	return true;
}

void CDeviceHandler::UpdateKeyboardState(){
	if (FAILED(m_pD3DInputDevice->GetDeviceState( sizeof(keys), keys ))){
		// If input is lost then acquire and keep trying until we get it back
		while( m_pD3DInputDevice->Acquire() == DIERR_INPUTLOST ){        
			//Do Nothing
		}
		// Now get them again	
		m_pD3DInputDevice->GetDeviceState( sizeof(keys), keys );
	}
	for(UINT a=0;a<256;++a)
		if(m_bKeyDown[a] && !KeyDown(a))
			m_bKeyDown[a]=false;
}
void CDeviceHandler::UpdateMouseState(){
	m_pD3DMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state);
	for(UINT a=0;a<4;++a)
		if(m_bButtonDown[a] && !MouseButtonDown(a))
			m_bButtonDown[a]=false;
}

bool CDeviceHandler::KeyDown(){
	for(UINT a=0;a<256;++a)
		if (keys[a] & 0x80)
			return true;

	return false;
}

bool CDeviceHandler::KeyDown(UINT Key){
	if (keys[Key] & 0x80)
			return true;

	return false;
}

bool CDeviceHandler::KeyPress(UINT Key){
	if(KeyDown(Key) && !m_bKeyDown[Key]){
		m_bKeyDown[Key]=true;
		return true;
	}
	return false;
}

bool CDeviceHandler::KeyRelease(UINT Key){
	if(!KeyDown(Key) && m_bKeyDown[Key]){
		m_bKeyDown[Key]=false;
		return true;
	}
	return false;
}

bool CDeviceHandler::MouseButtonDown(){
	for(UINT a=0;a<5;++a)
		if(mouse_state.rgbButtons[a])
			return true;

	return false;
}

bool CDeviceHandler::MouseButtonDown(UINT Button){
	if(mouse_state.rgbButtons[Button])return true;
	return false;
}

bool CDeviceHandler::MouseButtonPress(UINT Key){
	if(MouseButtonDown(Key) && !m_bButtonDown[Key]){
		m_bButtonDown[Key]=true;
		return true;
	}
	return false;
}
bool CDeviceHandler::MouseButtonRelease(UINT Key){
	if(!MouseButtonDown(Key) && m_bButtonDown[Key]){
		m_bButtonDown[Key]=false;
		return true;
	}
	return false;
}

Vector2 CDeviceHandler::MousePos(){
	POINT pos;
	GetCursorPos(&pos);
	return Vector2((float)pos.x + Global->CAMERA.x, (float)pos.y + Global->CAMERA.y);
}