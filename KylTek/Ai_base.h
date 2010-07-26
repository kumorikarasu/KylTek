#pragma once
#ifndef AI_BASE_H_
#define AI_BASE_H_

#include "CVObject.h"

class Ai_base : public CVObject{

protected:
	float m_animationa;
	float m_animationb;
	ICol* m_phitboxhead; //will have at least 2 hitboxes (head, body, feet)
	ICol* m_phitboxbody;

public:
	Ai_base();
	void Step(CDeviceHandler *DH);
	void Draw(CGraphicsHandler *g);


};

#endif