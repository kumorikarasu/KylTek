#include "Main.h"
#include "CGraphicsHandler.h"

//Take out all DX stuff not related with GFX, keyboard...
CGraphicsHandler::CGraphicsHandler(int _API){
	m_pD3DDevice = NULL;
	m_pD3DObject = NULL;
	m_pSprite	 = NULL;
	m_pLine		 = NULL;
	m_pFont		 = NULL;
	Rect		 = NULL;
	
	D3DXMatrixIdentity(&m_FontMatrix);

	Camera=Vector2(0, 0);

	API |= _API;
}
CGraphicsHandler::~CGraphicsHandler(){
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pLine);
	SAFE_RELEASE(m_pFont);
}

HRESULT CGraphicsHandler::InitializeAPI(HWND hWnd, HINSTANCE hInstance){
	// Create the main Direct3D object. This object is used to initialise Direct3D
	// and to provide us with a device interface object
	   if( NULL == ( m_pD3DObject = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return false;

    // Set up the structure used to create the g_pD3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the g_pD3DDevice
    if( FAILED( m_pD3DObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pD3DDevice ) ) )
    {
        return false;
    }
	D3DXCreateSprite(m_pD3DDevice,&m_pSprite);
	D3DXCreateLine(m_pD3DDevice, &m_pLine);
	D3DXFONT_DESC FontDesc = {12,
                          0,
                          400,
                          0,
                          false,
                          DEFAULT_CHARSET,
                          OUT_TT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          DEFAULT_PITCH,
                          L"Courier"};

	D3DXCreateFontIndirect( m_pD3DDevice, &FontDesc, &m_pFont );
	Rect = new CSprite(m_pD3DDevice, L"../Resources/Sprites/Misc/rect.PNG",0,0);
	Circle = new CSprite(m_pD3DDevice, L"../Resources/Sprites/Misc/circle.PNG",0,0, ColorARGB(255, 111,49,152));

	return true;
}

HRESULT CGraphicsHandler::Initialize2D(){
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	return true;
}

HRESULT CGraphicsHandler::Initialize3D(){
return true;
}

HRESULT CGraphicsHandler::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle){
	return m_pD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT CGraphicsHandler::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget){
	return m_pD3DDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT CGraphicsHandler::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget){
	m_pLine->End();
	m_pSprite->End();
	HRESULT r = m_pD3DDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pLine->Begin();
	return r;
}

HRESULT CGraphicsHandler::ClearRenderTarget(Color fill){
	return m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, fill, 1.0f, 0 );
}

HRESULT CGraphicsHandler::BeginRender(){

	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, NULL, 1.0f, 0 ); //Clear Render Target

	if (FAILED(m_pD3DDevice->BeginScene()))
		return false;
	
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pLine->Begin();

	return true;
}
HRESULT CGraphicsHandler::EndRender(){

	m_pLine->End();
	m_pSprite->End();

	m_pD3DDevice->EndScene();
	if(FAILED(m_pD3DDevice->Present(NULL,NULL,NULL,NULL)))
		return false;
	return true;
}



/*------------------------------------------------------------------------------------------------
-Draw Functions
------------------------------------------------------------------------------------------------*/

void CGraphicsHandler::DrawTileset(CSprite *sprite, int xframe, int yframe, Vector2 pos, Vector2 scale, float angle, Color argb){
	angle*= -1;
	RECT rct;

	if (xframe<1){xframe=1;}
	if (yframe<1){yframe=1;}

	rct.right=(long)sprite->SpriteWidth*xframe;
	rct.left=(long)(sprite->SpriteWidth*xframe)-(long)(sprite->SpriteWidth);
	rct.bottom=(long)(sprite->SpriteHeight*yframe);
	rct.top=(long)(sprite->SpriteHeight*yframe)-(long)(sprite->SpriteHeight);

	angle = min(max(angle, -360), 360);
	angle = D3DXToRadian(angle);
	
	D3DXMatrixScaling(&sprite->scale, scale.x,scale.y,1.0f);
	D3DXMatrixTranslation(&sprite->trans,pos.x-Camera.x,pos.y-Camera.y,0);
	D3DXMatrixRotationZ(&sprite->rot, angle);

	sprite->final = sprite->scale * sprite->rot * sprite->trans;
	m_pSprite->SetTransform(&sprite->final);
	m_pSprite->Draw(sprite->texture, &rct, &sprite->SpriteCenter, NULL, argb);
};

void CGraphicsHandler::DrawSprite(CSprite *sprite, int frame, Vector2 pos, Vector2 scale, float angle, Color argb){
	DrawSpriteTL(sprite, frame, Vector2(pos.x-Camera.x-sprite->SpriteCenter.x, pos.y-Camera.y-sprite->SpriteCenter.y), scale, angle, argb);
}
void CGraphicsHandler::DrawSprite(CSprite *sprite, int frame, Vector2 pos){
	DrawSpriteTL(sprite, frame, Vector2(pos.x-Camera.x-sprite->SpriteCenter.x, pos.y-Camera.y-sprite->SpriteCenter.y), Vector2(1, 1), 0, ColorARGB(255,255,255,255));
}
void CGraphicsHandler::DrawSpriteTL(CSprite *sprite, int frame, Vector2 pos, Vector2 scale, float angle, Color argb){

	angle*= -1;
	++frame;
	RECT rct;
	rct.left=0;
	rct.top=0;
	rct.bottom=(long)sprite->SpriteHeight;

	if (frame>sprite->numOfFrames){frame=sprite->numOfFrames;}
	if (frame<0){frame=0;}

	if (frame == 0){
		
		rct.right=(long)sprite->SpriteWidth*sprite->AnimationCount;
		rct.left=(long)(sprite->SpriteWidth*sprite->AnimationCount)-(long)(sprite->SpriteWidth);
		if (sprite->AnimationCount <= sprite->numOfFrames){
			if (sprite->AnimationStep < sprite->AnimationCount){
				sprite->AnimationStep++;
			}else{
				sprite->AnimationCount++;
				sprite->AnimationStep=0;
			}
		}
		if (sprite->AnimationCount > sprite->numOfFrames){
			sprite->AnimationCount=1;
		}
		
	}else{
		rct.right=(long)sprite->SpriteWidth*frame;
		rct.left=(long)(sprite->SpriteWidth*frame)-(long)(sprite->SpriteWidth);
	}
	angle = min(max(angle, -360), 360);
	angle = D3DXToRadian(angle);
	
	D3DXMatrixScaling(&sprite->scale, scale.x,scale.y,1.0f);
	D3DXMatrixTranslation(&sprite->trans,pos.x,pos.y,0);
	D3DXMatrixRotationZ(&sprite->rot, angle);

	sprite->final = sprite->scale * sprite->rot * sprite->trans;
	m_pSprite->SetTransform(&sprite->final);
	m_pSprite->Draw(sprite->texture, &rct, NULL, NULL, argb);

}
void CGraphicsHandler::DrawSpriteTL(CSprite *sprite, int frame, Vector2 pos){
	DrawSpriteTL(sprite, frame, pos, Vector2(1,1), 0, ColorARGB(255,255,255,255));
}


void CGraphicsHandler::DrawRect(Vector2 pos, int w, int h, Color color){
	DrawRectTL(Vector2(pos.x-Camera.x-(w/2), pos.y-Camera.y-(h/2)), w, h, color);
}
void CGraphicsHandler::DrawRect(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor){
	DrawRectTL(Vector2(pos.x-Camera.x-(w/2), pos.y-Camera.y-(h/2)), w, h, bgcolor, bordersize, bordercolor);
}
void CGraphicsHandler::DrawRectTL(Vector2 pos, int w, int h, Color color){
	RECT rct;
	rct.left=0;
	rct.top=0;
	rct.bottom=h;
	rct.right=w;
	D3DXMATRIX trans;
	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans,pos.x,pos.y,0);
	
	m_pSprite->SetTransform(&trans);
	m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, color);
}
void CGraphicsHandler::DrawRectTL(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor){
	if(bordersize==0)
		DrawRectTL(pos, w, h, bgcolor);
	else{
		RECT rct;
		rct.left=bordersize;
		rct.top=bordersize;
		rct.bottom=h-bordersize;
		rct.right=w-bordersize;

		D3DXMATRIX trans;
		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans,(pos.x+bordersize),(pos.y+bordersize),0);
		
		m_pSprite->SetTransform(&trans);
		m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, bgcolor);
		rct.left=0;
		rct.top=0;
		rct.right=w;
		rct.bottom=bordersize;
		D3DXMatrixTranslation(&trans,pos.x,pos.y,0);
		m_pSprite->SetTransform(&trans);
		m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, bordercolor);
		D3DXMatrixTranslation(&trans,pos.x,pos.y+(float)(h-bordersize),0);
		m_pSprite->SetTransform(&trans);
		m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, bordercolor);
		rct.bottom=h-(bordersize*2);
		rct.right=bordersize;
		D3DXMatrixTranslation(&trans,pos.x,pos.y+(float)(bordersize),0);
		m_pSprite->SetTransform(&trans);
		m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, bordercolor);
		D3DXMatrixTranslation(&trans,pos.x+(float)(w-bordersize),pos.y+(float)(bordersize),0);
		m_pSprite->SetTransform(&trans);
		m_pSprite->Draw(Rect->texture, &rct, NULL, NULL, bordercolor);
	}
}

void CGraphicsHandler::DrawOval(Vector2 pos, int w, int h, Color color){
	DrawOvalTL(Vector2(pos.x-Camera.x-(w/2), pos.y-Camera.y-(h/2)), w, h, color);
}
void CGraphicsHandler::DrawOval(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor){
	DrawOvalTL(Vector2(pos.x-Camera.x-(w/2), pos.y-Camera.y-(h/2)), w, h, bgcolor, bordersize, bordercolor);
}
void CGraphicsHandler::DrawOvalTL(Vector2 pos, int w, int h, Color color){

	D3DXMATRIX trans, scale, final;
	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixTranslation(&trans,pos.x,pos.y,0);
	D3DXMatrixScaling(&scale, (float)w/128.0f,(float)h/128.0f,1.0f);
	final = scale * trans;
	
	m_pSprite->SetTransform(&final);
	m_pSprite->Draw(Circle->texture, NULL, NULL, NULL, color);
}
void CGraphicsHandler::DrawOvalTL(Vector2 pos, int w, int h, Color bgcolor, int bordersize, Color bordercolor){
	if(bordersize==0)
		DrawOvalTL(pos, w, h, bgcolor);
	else{
		D3DXMATRIX trans, scale, final;
		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&scale);
		D3DXMatrixTranslation(&trans,pos.x,pos.y,0);
		D3DXMatrixScaling(&scale, (float)w/128.0f,(float)h/128.0f,1.0f);
		final = scale * trans;

		m_pSprite->SetTransform(&final);
		m_pSprite->Draw(Circle->texture, NULL, NULL, NULL, bordercolor);

		
		
		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&scale);
		D3DXMatrixTranslation(&trans,pos.x+bordersize,pos.y+bordersize,0);
		D3DXMatrixScaling(&scale, (float)(w-bordersize*2)/128.0f,(float)(h-bordersize*2)/128.0f,1.0f);
		final = scale * trans;
		
		m_pSprite->SetTransform(&final);
		m_pSprite->Draw(Circle->texture, NULL, NULL, NULL, bgcolor);
	}
}

	
void CGraphicsHandler::DrawText(std::wstring tstr, Vector2 pos, Color color){
	DrawTextTL(tstr, Vector2(pos.x-Camera.x, pos.y-Camera.y), color);
}
void CGraphicsHandler::DrawText(std::wstring tstr, Vector2 pos, float width, float height, Color color, DWORD flags){
	DrawTextTL(tstr, Vector2(pos.x-Camera.x, pos.y-Camera.y), width, height, color, flags);
}
void CGraphicsHandler::DrawTextTL(std::wstring tstr, Vector2 pos, Color color){
	RECT TextRect={(long)pos.x,(long)pos.y,0,0};
	m_pSprite->SetTransform(&m_FontMatrix);
	m_pFont->DrawText(m_pSprite, tstr.c_str(), -1, &TextRect, DT_CALCRECT, 0 );
	m_pFont->DrawText(m_pSprite, tstr.c_str(), -1, &TextRect, DT_LEFT, color );
}
void CGraphicsHandler::DrawTextTL(std::wstring tstr, Vector2 pos, float width, float height, Color color, DWORD flags){
	RECT TextRect={(long)pos.x,(long)pos.y,0,0};
	m_pFont->DrawText(m_pSprite, tstr.c_str(), -1, &TextRect, DT_CALCRECT, 0 );
	m_pFont->DrawText(m_pSprite, tstr.c_str(), -1, &TextRect, DT_LEFT, color );
	//font->Draw(m_pSprite, str.c_str(), pos.x, pos.y, width, height, color, flags);
}


void CGraphicsHandler::DrawLine(Vector2 pos1, Vector2 pos2, Color color){
	DrawLineTL(pos1-Camera, pos2-Camera, color);
}
void CGraphicsHandler::DrawLineTL(Vector2 pos1, Vector2 pos2, Color color){
	linepts[0]=pos1;
	linepts[1]=pos2;
	m_pLine->Draw(linepts, 2, color);
}

void CGraphicsHandler::DrawSurface(IDirect3DSurface9 * pSourceSurface, Vector2 pos, int w, int h, CONST RECT * pSourceRect){
	RECT DstRect;
	DstRect.left = pos.x;
	DstRect.top = pos.y;
	DstRect.right = pos.x+w;
	DstRect.bottom = pos.y+h;
	
	DrawSurface(pSourceSurface, pSourceRect, &DstRect);
}

void CGraphicsHandler::DrawSurface(IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, CONST RECT * pDestRect){
	IDirect3DSurface9* target = NULL;
	m_pD3DDevice->GetRenderTarget( 0, &target );
	m_pD3DDevice->StretchRect( pSourceSurface, pSourceRect, target, pDestRect, D3DTEXF_NONE );
}