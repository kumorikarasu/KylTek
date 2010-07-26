#pragma once
#ifndef CASSAULT_H_
#define CASSAULT_H_

#include "Weapon.h"

class CAssault : public CWeapon{
private:


public:
	CAssault();
	CAssault(bool _equipped, CEntity* _parent);
	~CAssault();

	virtual CEntity* Shoot();
	virtual void Step(CDeviceHandler *DH);
	virtual void Draw(CGraphicsHandler *g);

};


#endif