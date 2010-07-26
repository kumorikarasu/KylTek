#pragma once
#ifndef CBLOCK_H_
#define CBLOCK_H_

#include "CObject.h"

class CBlock : public CObject{
public:
	CBlock();
	CBlock(ENTITY_PARAMS* params, CSprite* _sprite);
	~CBlock();

	void Step(CDeviceHandler *DH);
	void Draw(CGraphicsHandler *g);
};

#endif