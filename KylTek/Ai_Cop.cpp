#include "Main.h"
#include "CGlobal.h"
#include "Ai_Cop.h"
#include "CollisionManager.h"
#include "Debug.h"
#include <math.h>

Ai_Cop::Ai_Cop(ENTITY_PARAMS* params){
	Init(params);
	m_friction =			Vector2(0.03f,0.03f);

	m_pCollider->			SetCollisionType(Dynamic);
	m_pCollider->			SetSize(Vector2(64,64));
	Global->				pColMan->AddICol(m_pCollider);
/*
	m_phitboxhead =			new ICol_AABB(this);
	m_phitboxhead->			SetCollisionType(HitBox);
	m_phitboxhead->			m_h = 50;
	m_phitboxhead->			m_w = 50;
	m_phitboxhead->			m_hy = 25;
	m_phitboxhead->			m_hx = 25;
*/
	m_prevpos =				m_pos;
}

Ai_Cop::~Ai_Cop(){

}

void Ai_Cop::Step(CDeviceHandler* DH){
	//Global->pColMan->getQT()->RemoveEntity(m_pCollider);

	if(m_animationa>=360)
		m_animationa=0;
	if(m_animationa<=-360)
		m_animationa=0;

	m_animationa+=m_accel.x/2;
	m_animationb+=0.05f;

	Verlet();	

	if (m_pos.x<0) m_pos.x=0;
	if (m_pos.x>Global->LEVEL_WIDTH) m_pos.x=Global->LEVEL_WIDTH;
	if (m_pos.y<0)
		m_state |= Dead;
	if (m_pos.y>Global->LEVEL_HEIGHT) 
		m_state |= Dead;

	m_pCollider -> UpdatePos();
};

void Ai_Cop::Draw(CGraphicsHandler *g){
	/*
		g->DrawSprite(
		g->SprList->Char->ME->Feet,
		0,
		Vector2(m_pos.x-sin(m_animationa)*8,m_pos.y+6-(m_pos.y-m_prevpos.y)),
		Vector2(m_scale.x*m_dir, m_scale.y),
		((m_pos.y-m_prevpos.y)*6)*m_dir,
		CLR_WHITE);

	g->DrawSprite(
		g->SprList->Char->ME->Feet,
		0,
		Vector2(m_pos.x+sin(m_animationa)*8+(3*m_dir),m_pos.y+6-(m_pos.y-m_prevpos.y)),
		Vector2(m_scale.x*m_dir, m_scale.y),
		((m_pos.y-m_prevpos.y)*6)*m_dir,
		CLR_WHITE);
	


	g->DrawSprite(
		g->SprList->Char->ME->FullHead,
		0,
		Vector2(m_pos.x+(((m_pos.x-m_prevpos.x)/1.2f)),m_pos.y-39-sin(m_animationb)*2),
		Vector2(m_scale.x*m_dir, m_scale.y),
		D3DXToDegree(atan2(MOUSE.x-m_pos.x, MOUSE.y-m_pos.y))-(90*m_dir),
		CLR_WHITE);
*/


//	g->DrawSprite(
//	g->SprList->Char->ME->Torso,
//	0,
//	Vector2(m_pos.x,m_pos.y-46));
};

void Ai_Cop::DebugDraw(CGraphicsHandler *g){
	if(Debug->CheckFlag(CDebug::HitBoxes)){
	//	g->DrawRect( Vector2(m_pos.x,m_pos.y), getWidth(), getHeight(), D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(255,0,0,255));
	//	g->DrawRect( Vector2(m_pos.x,m_pos.y), (int)m_phitboxhead->m_w, (int)m_phitboxhead->m_h, D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(255,255,0,255));
	}
}