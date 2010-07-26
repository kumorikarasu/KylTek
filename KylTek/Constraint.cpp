#include "Main.h" 

#include "CConstraint.h"
#include "Functions.h"

CConstraint::CConstraint(Vector4* objA, Vector4* objB){
	m_pObjA=objA;
	m_pObjB=objB;
	float dx=m_pObjB->x-m_pObjA->x;
	float dy=m_pObjB->y-m_pObjA->y;
	m_distance=sqrt(dx*dx+dy*dy);
};
CConstraint::CConstraint(Vector4* objA, Vector4* objB, float distance){
	m_pObjA=objA;
	m_pObjB=objB;
	m_distance=distance;
};
CConstraint::~CConstraint(){
	m_pObjA=NULL;
	m_pObjB=NULL;
};
void CConstraint::Satisfy(){
	float x1 = m_pObjA->x;
	float x2 = m_pObjB->x;
	float y1 = m_pObjA->y;
	float y2 = m_pObjB->y;
	float dx = x2-x1;
	float dy = y2-y1;
	float d1 = sqrt (dx*dx + dy*dy);
	float d2 = 0.5f*(d1-m_distance)/d1;
	dx = dx*d2;
	dy = dy*d2;

	m_pObjA->x += dx;
	m_pObjA->y += dy;
	m_pObjB->x += -dx;
	m_pObjB->y += -dy;
};

void CConstraint::Draw(CGraphicsHandler *g){
	g->DrawLine(Vector2(m_pObjA->x,m_pObjA->y),Vector2(m_pObjB->x,m_pObjB->y),D3DCOLOR(0xFF00FF00));
}