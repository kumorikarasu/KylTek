#include "Main.h" 
#include "Functions.h"
#include "CBlock.h"
#include "CGlobal.h"

CObject::CObject(){
	//Init(NULL);
	//m_objectid=101;
	//m_pSprite=NULL;
};
CObject::CObject(ENTITY_PARAMS* params, CSprite* _sprite){
	//Init(params);
	//m_objectid=101;
////	m_pSprite=_sprite;
};
CObject::~CObject(){};
void CObject::Step(CDeviceHandler* DH){
	if(!(m_state & Immovable))
	{
		m_pos+=m_accel;//Actually Velocity, not Acceleration
		m_pos.x = min(max(m_pos.x, m_halfwidth), Global->LEVEL_WIDTH-m_halfwidth);
		m_pos.y = min(max(m_pos.y, m_halfheight), Global->LEVEL_HEIGHT-m_halfheight);
	}
};
void CObject::Draw(CGraphicsHandler *g){
	if(m_pSprite)
		g->DrawSprite(m_pSprite,0,m_pos);
};
