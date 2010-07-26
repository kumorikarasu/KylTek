#include "Main.h"
#include "CEntity.h"


CEntity::CEntity()
{
	Init(NULL);
}

CEntity::CEntity(ENTITY_PARAMS* params){
	Init(params);
}

CEntity::~CEntity()
{
	Debug->ToConsole("- Entity Destroyed");
	m_pCollider->isDead=true;
}



void CEntity::Init(ENTITY_PARAMS* params)
{
	m_pCollider = NULL;

	if(params){
		m_pos=			params->pos;
		m_accel=		params->accel;
		m_width=		params->width;
		m_height=		params->height;
		m_halfwidth=	(int)(m_width/2);
		m_halfheight=	(int)(m_height/2);
		m_depth=		params->depth;
		m_state=		params->stateflags;

		switch (params->colTYPE){
			case AABB:
				m_pCollider = new ICol_AABB(this);//CCollider_AABB(this);
			break;
			case Circle:
				m_pCollider = new ICol_Circle(this);
			break;
			default:
				m_pCollider = new ICol_AABB(this);;
		}
	}else{
		m_pos=			Vector2(0, 0);
		m_accel=		Vector2(0, 0);
		m_vel=			Vector2(0, 0);
		m_width=		0;
		m_height=		0;
		m_halfwidth=	0;
		m_halfheight=	0;
		m_depth=		0;
		m_state=		0;
		m_pCollider=	new ICol_AABB(this);
	}
	//	m_pCollider = new CCollider_AABB(this);
	// FIXME: not all entities need this



	m_scale=		Vector2(1, 1);
	m_angle=		0;
	m_dir=			1;
	m_hp=			100;
}