#pragma once
#ifndef CCONSTRAINT_H_
#define CCONSTRAINT_H_

#include "main.h"
#include "CVObject.h"

struct Vector4{
	float x,y,px,py;
	Vector4(float _x, float _y, float _px, float _py){
		x=_x;
		y=_y;
		px=_px;
		py=_py;
	}
	Vector4(float _x, float _y){
		x=_x;
		y=_y;
		px=_x;
		py=_y;
	}
};

class CConstraint{
private:
	float m_distance;
public:

	Vector4* m_pObjA;
	Vector4* m_pObjB;

	CConstraint(Vector4* objA, Vector4* objB);
	CConstraint(Vector4* objA, Vector4* objB, float distance);
	~CConstraint();
	void Satisfy();
	
	void Draw(CGraphicsHandler *g);
};

#endif