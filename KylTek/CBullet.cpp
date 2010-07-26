#include "Main.h" 
#include "CGlobal.h"
#include "Debug.h"
#include "bullet.h"
#include "Weapon.h"



Bullet::Bullet(CEntity* _parent, float _dir, float _dis){
	Init(NULL);
	m_pParent =				_parent;
	m_direction =			(int)_dir;
	m_dis =					_dis;
	m_accel.x =				cos((float)_dir) * _dis;
	m_accel.y =				sin((float)_dir) * _dis;
	m_alpha =				255;
	m_objectid =			105;

	//m_pCollider->SetCollisionFlags(NoPush);
	m_pCollider->SetCollisionType(Passive);
	m_pCollider->SetSize(Vector2(16,16));
	Global->pColMan->AddICol(m_pCollider);
	//Debug->ToConsole("+ Bullet Created");
}
Bullet::~Bullet(){

};


void Bullet::onCollision(const Vector2& result, CEntity* pCollideEnt)
{
	m_accel=Vector2(0, 0);
	m_state |= Dead;
	
	//pCollideEnt->setStateFlags(Dead);
}


void Bullet::Step(CDeviceHandler* DH){
	m_pCollider->RemoveQT();

		m_alpha-=3;
	if (m_alpha < 0)
		m_state |= Dead;

	m_pos+=m_accel;
	/*Node<CEntity>* it = _EntList->first();
	CEntity* Entity;
	bool collision=false;
	while(it!=NULL){
		Entity = it->object;

		//COLLISIONS
		if(Entity->getObjectID() != 105 && Entity != m_pParent->m_pParent && Entity !=m_pParent)
		if(Entity->Solid || !Entity->Invincible){
			if (m_pos.x>Entity->getPos().x && m_pos.x<Entity->getPos().x+32 && m_pos.y>Entity->getPos().y && m_pos.y<Entity->getPos().y+32){
				m_accel=Vector2(0, 0);
			}
		}
		it = it -> next;
	}

	m_pos+=m_accel;

	it = _EntList->first();
	while(it!=NULL){
		Entity = it->object;

		//COLLISIONS
		if(Entity->getObjectID() != 105 && Entity != m_pParent->m_pParent && Entity !=m_pParent)
		if(Entity->Solid || !Entity->Invincible){
			if (m_pos.x>Entity->getPos().x && m_pos.x<Entity->getPos().x+32 && m_pos.y>Entity->getPos().y && m_pos.y<Entity->getPos().y+32){
				m_accel=Vector2(0, 0);
			}
		}
		it = it -> next;
	}*/
	m_pCollider->UpdatePos();
}

void Bullet::Draw(CGraphicsHandler *g){
	//g->DrawSprite(
	//	g->SprList->Char->ME->Torso,
	//	0,
	//	Vector2(m_pos.x,m_pos.y),
	//	Vector2(0.1f, 0.1f),
	//	0,
	//	CLR_WHITE);
	g->DrawLine(m_pos, Vector2((float)m_xs, (float)m_ys), D3DCOLOR_ARGB(m_alpha, 255, 0, 255) );

//	m_pCollider
}