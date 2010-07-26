#pragma once
#ifndef CWEAPON_H_
#define CWEAPON_H_

#include "CVObject.h"

class CWeapon : public CVObject{
public:

	int m_ammo, m_maxammo, m_clipsize;
	float m_reloadtime, m_firetime, m_recoilamount, m_recoil;
	bool m_bEquipped;
	CEntity *m_pParent;

	CWeapon(){}

	virtual CEntity* Shoot() {return NULL;}
	virtual void Step(LinkedList<CEntity> *_EntList, BYTE _keys[256], DIMOUSESTATE _mouse){};
	virtual void Draw(CGraphicsHandler *g, Sprites* _Sprite){};

	virtual void PhysStep(const Vector2& collisionResult) {};
};


#endif