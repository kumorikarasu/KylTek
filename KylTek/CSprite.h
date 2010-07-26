#ifndef CSPRITE_H_
#define CSPRITE_H_

#include "Main.h"

class CSprite{
private:
	D3DXIMAGE_INFO imginfo;
public:
	D3DXMATRIX scale;
	D3DXMATRIX rot;
	D3DXMATRIX trans;
	D3DXMATRIX final;
	LPDIRECT3DTEXTURE9 texture;

	
	int ImgWidth;
	int ImgHeight;
	int SpriteWidth;
	int SpriteHeight;
	Vector3 SpriteCenter;
	int numOfFrames;
	int AnimationSpeed;
	int AnimationStep;
	int AnimationCount;

	CSprite(LPDIRECT3DDEVICE9 _device,  LPTSTR _filename, int _frames, int _speed);
	CSprite(LPDIRECT3DDEVICE9 _device,  LPTSTR _filename, int _frames, int _speed, Color _transparency);
	CSprite(LPDIRECT3DDEVICE9 _device,  LPTSTR _filename, int _xframes, int _yframes, int _speed, Color _transparency);
	~CSprite();
};

#endif