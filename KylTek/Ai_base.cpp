#include "Main.h"
#include "Ai_base.h"
#include "CGlobal.h"
#include <math.h>

Ai_base::Ai_base(){

};

void Ai_base::Step(CDeviceHandler* DH){

	if(m_animationa>=360)
		m_animationa=0;
	if(m_animationa<=-360)
		m_animationa=0;

	m_animationa+=m_accel.x/2;
	m_animationb+=0.05f;

	Verlet();		
};

void Ai_base::Draw(CGraphicsHandler *g){
	/*
	Vector2 Mouse = Global->MousePos;
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
		g->SprList->Char->ME->Torso,
		0,
		Vector2(m_pos.x,m_pos.y-18-((m_pos.y-m_prevpos.y)/1.5f)-sin(m_animationb)*3), 
		Vector2(m_scale.x*m_dir, m_scale.y),
		m_angle/2,
		CLR_WHITE);

	g->DrawSprite(
		g->SprList->Char->ME->FullHead,
		0,
		Vector2(m_pos.x+(((m_pos.x-m_prevpos.x)/1.2f)),m_pos.y-39-sin(m_animationb)*2),
		Vector2(m_scale.x*m_dir, m_scale.y),
		D3DXToDegree(atan2(Mouse.x-m_pos.x, Mouse.y-m_pos.y))-(90*m_dir),
		CLR_WHITE);
	*/
};