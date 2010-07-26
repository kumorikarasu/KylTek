#include "Main.h" 
#include "CVObject.h"
#include "Functions.h"
#include "CGlobal.h"

CVObject::CVObject() {
	//Init(NULL);
	//m_objectid=102;
	//m_prevpos=Vector2(0, 0);
	//m_nextpos=Vector2(0, 0);
	//m_friction=Vector2(0, 0);
	//m_pSprite=NULL;

	//m_pCollider->SetCollisionType(Dynamic);
};

CVObject::CVObject(ENTITY_PARAMS* params, CSprite* _sprite){
	//Init(params);
	//m_objectid=102;
	//m_pSprite=_sprite;
	//m_prevpos=m_pos;
	//m_nextpos=m_pos;
	//m_friction=Vector2(0, 0);

	//m_pCollider->SetCollisionType(Dynamic);
};
CVObject::~CVObject(){};

void CVObject::Step(CDeviceHandler* DH){
	if(!(m_state & Immovable)){
		Verlet();
		m_pos.x = min(max(m_pos.x, m_halfwidth), Global->LEVEL_WIDTH-m_halfwidth);
		m_pos.y = min(max(m_pos.y, m_halfheight), Global->LEVEL_HEIGHT-m_halfheight);
	}
};
void CVObject::Draw(CGraphicsHandler *g){
	if(m_pSprite)
		g->DrawSprite(m_pSprite,0,m_pos);
};