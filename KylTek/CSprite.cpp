#include "Main.h"
#include "CSprite.h"

CSprite::CSprite(LPDIRECT3DDEVICE9 _device,  LPCTSTR _filename, int _frames, int _speed){
	texture=NULL;

	if (_frames<1){_frames=1;}
	numOfFrames = _frames;

	if (_speed<1){_speed=1;}
	AnimationSpeed = _speed;

	AnimationStep = 0;
	AnimationCount = 1;
	
	if (SUCCEEDED(D3DXCreateTextureFromFile(_device, _filename, &texture))){
		
		D3DXGetImageInfoFromFile(_filename, &imginfo);
		ImgWidth=imginfo.Width;
		ImgHeight=imginfo.Height;
		SpriteWidth=ImgWidth/numOfFrames;
		SpriteHeight=ImgHeight;
		SpriteCenter=Vector3((float)SpriteWidth/2, (float)SpriteHeight/2, 0);
	}
}
CSprite::CSprite(LPDIRECT3DDEVICE9 _device,  LPCTSTR _filename, int _frames, int _speed, Color _transparency){
	texture=NULL;

	if (_frames<1){_frames=1;}
	numOfFrames = _frames;

	if (_speed<1){_speed=1;}
	AnimationSpeed = _speed;

	AnimationStep = 0;
	AnimationCount = 1;
	
	if (SUCCEEDED(D3DXCreateTextureFromFileExA(_device, _filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL,
		D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, _transparency, NULL, NULL, &texture))){
		
		D3DXGetImageInfoFromFile(_filename, &imginfo);
		ImgWidth=imginfo.Width;
		ImgHeight=imginfo.Height;
		SpriteWidth=ImgWidth/numOfFrames;
		SpriteHeight=ImgHeight;
		SpriteCenter=Vector3((float)SpriteWidth/2, (float)SpriteHeight/2, 0);
	}

}

CSprite::CSprite(LPDIRECT3DDEVICE9 _device,  LPCWSTR _filename, int _xframes, int _yframes, int _speed, Color _transparency){
	texture=NULL;

	if (_xframes<1){_xframes=1;}
	if (_yframes<1){_yframes=1;}
	numOfFrames = _xframes*_yframes;

	if (_speed<1){_speed=1;}
	AnimationSpeed = _speed;

	AnimationStep = 0;
	AnimationCount = 1;
	
	if (SUCCEEDED(D3DXCreateTextureFromFileExA(_device, _filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL,
		D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, _transparency, NULL, NULL, &texture))){
		
		D3DXGetImageInfoFromFile(_filename, &imginfo);
		ImgWidth=imginfo.Width;
		ImgHeight=imginfo.Height;
	}
	SpriteWidth=ImgWidth/_xframes;
	SpriteHeight=ImgHeight/_yframes;
	SpriteCenter=Vector3((float)SpriteWidth/2, (float)SpriteHeight/2, 0);
}

CSprite::~CSprite(){
	SAFE_RELEASE(texture);
}

