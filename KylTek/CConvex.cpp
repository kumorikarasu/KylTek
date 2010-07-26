#include "CConvex.h"
#include "CGlobal.h"
#include "CConstraint.h"
#include "Foreach.h"

CConvex::CConvex(){
		//Constraints = new vector<CConstraint>;
		//Vertexes = new vector<Vector2>;
	m_pCollider = new ICol_Convex(this);
	m_pCollider->SetCollisionType(Dynamic);
	m_pCollider->UpdatePos();
	Global->pColMan->AddICol(m_pCollider);

	m_friction=Vector2(0,0);
}

CConvex::~CConvex(){

}

void CConvex::AddVertex(Vector4* V){
	Vertexes.push_back(V);
}

void CConvex::CalcConstraints(){ //Exponential Growth
	// 3 Nested Loops, FUCK!
	FOREACH(Vector4*, Vertexes, c){
		FOREACH(Vector4*, Vertexes, d){
			if ((*d) == (*c))
				continue;
			bool add=true;
			FOREACH(CConstraint*, Constraints, e){
				if ((*e)->m_pObjA == (*c) || (*e)->m_pObjA == (*d))
					if ((*e)->m_pObjB == (*c) || (*e)->m_pObjB == (*d)){
						add=false;
						break;
					}
			}
			
			if (add)
			Constraints.push_back(new CConstraint((*c),(*d)));
		}
	}
}

void CConvex::Step(){ 

	//update all vertex positions based on verlet
	FOREACH(Vector4*, Vertexes, c){
		m_pos.x=(*c)->x;
		m_pos.y=(*c)->y;
		m_prevpos.x=(*c)->px;
		m_prevpos.y=(*c)->py;
		Verlet();

		m_pos.y = min(m_pos.y,500);

		(*c)->x=m_pos.x;
		(*c)->y=m_pos.y;
		(*c)->px=m_prevpos.x;
		(*c)->py=m_prevpos.y;
		
	}

	
	//go throgh each constraint and satisfy them
	FOREACH(CConstraint*, Constraints, c){
		(*c)->Satisfy();
	}

	//Do it again for more accuracy
	/*FOREACH(CConstraint*, Constraints, c){
		(*c)->Satisfy();
	}*/
	m_pCollider ->UpdatePos();
}

void CConvex::Draw(CGraphicsHandler *g){
		FOREACH(CConstraint*, Constraints, c){
		(*c)->Draw(g);
	}
}

void CConvex::CalcMinMax(){
	//TODO: OPTIMIZE

	m_min = Vector2(Global->LEVEL_WIDTH,Global->LEVEL_HEIGHT);
	m_max = Vector2(0,0);
	
	FOREACH(Vector4*, Vertexes, c){
		m_min.x=min(m_min.x,(*c)->x);
		m_min.y=min(m_min.y,(*c)->y);
		m_max.x=max(m_max.x,(*c)->x);
		m_max.y=max(m_max.y,(*c)->y);
	}
}