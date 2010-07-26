#ifndef CGRAPHICSHANDLER_H_
#define CGRAPHICSHANDLER_H_
#include "Main.h"
#include "CSprite.h"
#include "Sprite.h"

class CGraphicsHandler{
private:
	
	LPDIRECT3D9			m_pD3DObject;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXLINE			m_pLine;
	LPD3DXFONT			m_pFont;
	D3DXMATRIX			m_FontMatrix;

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

	Vector2 Camera;
	Vector2 linepts[2];

	CSprite* Rect;
	CSprite* Circle;

	int API;
public:
LPDIRECT3DDEVICE9	m_pD3DDevice;
	CGraphicsHandler(int API);
	~CGraphicsHandler();

	Sprites* SprList;

	enum eAPI{
		Empty,
		OpenGL = 1 << 0,
		DirectX = 1 << 1
	};
//SetFont - size, bla bla

	HRESULT InitializeAPI(HWND hWnd, HINSTANCE hInstance);

	HRESULT Initialize2D();
	HRESULT Initialize3D(); //No 3D Yet...

	HRESULT CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
	HRESULT GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget);
	HRESULT SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget);
	HRESULT ClearRenderTarget(Color fill);

	HRESULT BeginRender(); //Clear Backbuffer
	HRESULT EndRender(); //Flip Front and Back Buffers

	//2D Drawing Functions
	//Normal Functions draw sprites from centre, and will follow the camera/view
	//TL Functions draw sprites from top left, and will not follow the camera/view
	void DrawTileset(CSprite *sprite, int xframe, int yframe, Vector2 pos, Vector2 scale, float angle, Color argb);
	void DrawSprite(CSprite *sprite, int frame, Vector2 pos, Vector2 scale, float angle, Color argb);
	void DrawSprite(CSprite *sprite, int frame, Vector2 pos);
	void DrawSpriteTL(CSprite *sprite, int frame, Vector2 pos, Vector2 scale, float angle, Color argb);
	void DrawSpriteTL(CSprite *sprite, int frame, Vector2 pos);

	void DrawRect(Vector2 pos, int w, int h, Color color);
	void DrawRect(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor);
	void DrawRectTL(Vector2 pos, int w, int h, Color color);
	void DrawRectTL(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor);

	void DrawOval(Vector2 pos, int w, int h, Color color);
	void DrawOval(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor);
	void DrawOvalTL(Vector2 pos, int w, int h, Color color);
	void DrawOvalTL(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor);

	void DrawText(std::wstring tstr, Vector2 pos, Color color);
	void DrawText(std::wstring tstr, Vector2 pos, float width, float height, Color color, DWORD flags);
	void DrawTextTL(std::wstring tstr, Vector2 pos, Color color);
	void DrawTextTL(std::wstring tstr, Vector2 pos, float width, float height, Color color, DWORD flags);

	void DrawLine(Vector2 pos1, Vector2 pos2, Color color);
	void DrawLineTL(Vector2 pos1, Vector2 pos2, Color color);
	
	//void DrawPrimitives(Vector2 pos);

	void DrawSurface(IDirect3DSurface9 * pSourceSurface, Vector2 pos, int w, int h, CONST RECT * pSourceRect);
	void DrawSurface(IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, CONST RECT * pDestRect);

	void SetupSprites();
	void CleanupSprites();

	void setCamera(Vector2& _cam){
		Camera = _cam;
	}
	Vector2& getCamera(){
		return Camera;
	}
};

#endif