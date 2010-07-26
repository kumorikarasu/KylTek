#pragma once
#ifndef COBJECT_H_
#define COBJECT_H_

#include "CEntity.h"

class CObject : public CEntity{
protected:
	CSprite* m_pSprite;
public:
	CObject();
	CObject(ENTITY_PARAMS* params, CSprite* _sprite);
	~CObject();

	virtual void Step(CDeviceHandler *DH);
	virtual void Draw(CGraphicsHandler *g);
};

#endif