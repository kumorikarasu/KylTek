#pragma once
#ifndef AI_COP_H_
#define AI_COP_H_

#include "Ai_base.h"

class Ai_Cop : public Ai_base{

private:
	float m_animationa;
	float m_animationb;

	Vector2 m_lastCollisionPos;

public:
	Ai_Cop(){};
	~Ai_Cop();
	Ai_Cop(ENTITY_PARAMS* params);
	void Step(CDeviceHandler *DH);
	void Draw(CGraphicsHandler *g);
	void DebugDraw(CGraphicsHandler *g);

};

#endif