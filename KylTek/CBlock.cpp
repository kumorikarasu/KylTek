#include "Main.h" 
#include "Functions.h"
#include "CBlock.h"
#include "CGlobal.h"

CBlock::CBlock(){
	Init(NULL);
	m_objectid=		101;
	m_pSprite=		NULL;
};


CBlock::CBlock(ENTITY_PARAMS* params, CSprite* _sprite){
	Init(params);
	m_objectid=		101;
	m_pSprite=		_sprite;

	//((CCollider_AABB*)m_pCollider)->SetSize(Vector2(getWidth()+1.0f,getHeight()));

	m_pCollider->SetCollisionType(Static);
	//m_pCollider->SetSize(Vector2(m_width,m_height));
	Global->pColMan->AddICol(m_pCollider);
	m_pCollider ->UpdatePos();

};


CBlock::~CBlock(){
	m_pCollider->isDead = true;
}

void CBlock::Step(CDeviceHandler* DH){
	if(!(m_state & Immovable)){
		m_pos+=m_accel;//Actually Velocity, not Acceleration
		m_pos.x = min(max(m_pos.x, m_halfwidth), Global->LEVEL_WIDTH-m_halfwidth);
		m_pos.y = min(max(m_pos.y, m_halfheight), Global->LEVEL_HEIGHT-m_halfheight);
	}
	m_pCollider ->UpdatePos();
};



void CBlock::Draw(CGraphicsHandler *g){
	//if(sprite!=NULL)

	ICol_AABB* pTemp;
	pTemp = (ICol_AABB*) m_pCollider;


	g->DrawRect( m_pos, (int)pTemp->m_w, (int)pTemp->m_h, D3DCOLOR_RGBA(0,0,255,255), 1, D3DCOLOR_RGBA(0,0,255,255));

//	g->DrawTileset(g->SprList->Level->TSBuilding1,4, 4,m_pos, m_scale, 0, CLR_WHITE);

	
};
