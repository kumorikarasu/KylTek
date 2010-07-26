#include "Main.h" 
#include "CGlobal.h"
#include "Functions.h"
#include "CAssault.h"
#include "Bullet.h"


CAssault::CAssault(){}

CAssault::CAssault(bool _equipped, CEntity* _parent){
	Init(NULL);
	m_ammo=				30;
	m_maxammo=			300;
	m_clipsize=			30;
	m_scale.x=			1;
	m_scale.y=			1;
	m_dir=				1;

	m_pCollider->SetCollisionType(NoCollide);

	m_bEquipped = _equipped;
	if (m_bEquipped)
		m_pParent=_parent;
}

CAssault::~CAssault(){

}

/*////////////
Creates a bullet of type X, moving towards the mouse
removes 1 from the ammo count
function will fail if ammo count is 0
*////////////
CEntity* CAssault::Shoot(){
	Vector2 Mouse = Global->MousePos;
	if (m_ammo>0){
		Bullet* bul = new Bullet(m_pParent,m_pParent->getAngle(),10);
		bul->setPos(m_pos);
		bul->m_xs = (int)m_pos.x;
		bul->m_ys = (int)m_pos.y;
		return bul;
	}else
		return NULL;
}

void CAssault::Step(CDeviceHandler* DH){
	if (!m_bEquipped){
		Verlet();
	}
}



void CAssault::Draw(CGraphicsHandler *g){
	//g->DrawSprite(g->SprList->Weapon->AssaultRifle,-1,m_pos, Vector2(m_scale.x*m_dir, m_scale.y), 0, CLR_WHITE);
}