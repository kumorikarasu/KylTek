#pragma once
#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "CVObject.h"
#include "Weapon.h"
#include "CConvex.h"

class CPlayer : public CVObject{
private:
	int m_frame;
	float m_animationa;
	float m_animationb;
	int m_hspeed;
	int m_vspeed;
	bool m_bJumped;
	CWeapon* m_pWeapon;
	CWeapon* m_pAssault;
	bool m_bRUNONCE;
	bool m_bVaribeJump;
	QuadTree* QT;
	int m_runtimer;
	bool m_bShooting;
	bool m_bCantJump;

	//TEST VARIBLES
	ICol* TESTQUAD;
	QuadTree* QTTESTQUAD;
	//
	CConvex* TEST;
	CConvex* TEST2;
	
	Vector2 m_lastCollisionPos;

public:

	CPlayer();
	CPlayer(ENTITY_PARAMS* params);
	~CPlayer();

	void Step(CDeviceHandler *DH);
	void Draw(CGraphicsHandler *g);
	void DebugDraw(CGraphicsHandler *g);

	void onCollision(const Vector2& result, CEntity* pEnt);
	
};

#endif