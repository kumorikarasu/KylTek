#pragma once
#ifndef CVOBJECT_H_
#define CVOBJECT_H_

#include "CEntity.h"

class CVObject : public CEntity{
protected:
	Vector2 m_prevpos, m_nextpos, m_friction;
public:
 
	CSprite* m_pSprite;

	CVObject();
	CVObject(ENTITY_PARAMS* params, CSprite* _sprite);
	~CVObject();

	virtual void Step(CDeviceHandler *DH);
	virtual void Draw(CGraphicsHandler *g);

	virtual void onCollision(const Vector2& result, CEntity* pCollideEnt) {}

	void Verlet(){
		
		/*
		m_prevpos.x=m_pos.x;
		m_prevpos.y=m_pos.y;

		m_pos.x+=m_accel.x;
		m_accel.y+=0.5f;
		m_pos.y+=m_accel.y;
		*/

		
		m_nextpos.x=(2-m_friction.x)*m_pos.x-(1-m_friction.x)*m_prevpos.x+m_accel.x;
		m_nextpos.y=(2-m_friction.y)*m_pos.y-(1-m_friction.y)*m_prevpos.y+(m_accel.y+0.7f); //Gravity is 0.3f, global not working
		m_prevpos.x=m_pos.x;
		m_prevpos.y=m_pos.y;
		m_pos.x=m_nextpos.x;
		m_pos.y=m_nextpos.y;
		
	}

	inline Vector2 getPrevPos() const { return m_prevpos; }

	void setPrevPos(Vector2 _pos){m_prevpos=_pos;}
	void addPrevPos(Vector2 _pos){m_prevpos+=_pos;}
};

#endif