#pragma once
#ifndef BULLET_H_
#define BULLET_H_

#include "CObject.h"
#include "Weapon.h"

class Bullet : public CObject{
public:
	int m_xs, m_ys, m_direction; 
	float m_dis;
	int m_alpha;

	Bullet(){};
	Bullet(CEntity* _parent, float _dir, float _dis);
	~Bullet();

	virtual void Step(CDeviceHandler *DH);
	virtual void Draw(CGraphicsHandler *g);

	virtual void onCollision(const Vector2& result, CEntity* pCollideEnt);

};


#endif