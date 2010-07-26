#ifndef MAIN_H_
#define MAIN_H_
#ifndef WINVER
#define WINVER         0x0500
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600
#endif
#define VC_EXTRALEAN
//#define PI 3.14159265
#define DIRECTINPUT_VERSION 0x0800

#pragma warning (disable: 4244)
#pragma warning (disable: 4244)

typedef int U32,u32;
typedef short U16,u16;

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>
#include <dinput.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

// We might want to use our own vector later on, so this will ease the transition
#define Vector2 D3DXVECTOR2
#define Vector3 D3DXVECTOR3
#define Color D3DCOLOR
#define ColorARGB D3DCOLOR_ARGB
// more future planning
//#define CEntityList LinkedList<CEntity>

// I am smarter then the compiler DURHURR
#define FORCEINLINE __forceinline

// Always use this, it will stop when it tries to delete null pointers and it makes
// pointers null after deletion
#define SAFE_DELETE(p) { if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p){ delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p) {if(p){ (p)->Release(); (p) = NULL; } }

// might come in handy
#define DebuggerBreak()  __asm { int 3 }

using namespace std;

#endif