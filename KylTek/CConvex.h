#pragma once
#ifndef CONVEX_H_
#define CONVEX_H_

#include "main.h"
#include "CVObject.h"
#include "CConstraint.h"
#include "CollisionManager.h"


class CConvex : public CVObject{ //CONVEX POLYGONS BIATCHES CAUSE THEY WILL WORK HA

	vector<CConstraint*> Constraints;
	//Vertex should be here
	ICol_Convex* m_pCollider;
	
public:

	vector<Vector4*> Vertexes;

	CConvex();
	~CConvex();

	Vector2 m_min,m_max;

	void AddVertex(Vector4* V);
	void CalcConstraints();
	void Step();
	void Draw(CGraphicsHandler *g);
	void CalcMinMax();
};

#endif