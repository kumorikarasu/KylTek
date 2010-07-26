#include "Main.h" 
#include "Functions.h"
#include "CPlayer.h"
#include "Bullet.h"
#include "CAssault.h"
#include "Foreach.h"
#include "QuadTree.h"
#include "Debug.h"
#include "CGlobal.h"
#include "Ai_Cop.h"

CPlayer::CPlayer(ENTITY_PARAMS* params){
	Init(params);

	//Vectors
	m_prevpos=			m_pos;
	m_nextpos=			m_pos;
	m_friction=			Vector2(0.09f,0.05f);
	m_lastCollisionPos= Vector2(0, 0);

	//Ints
	m_objectid=			1000;
	m_animationa=		0;
	m_animationb=		0;
	m_frame=			0;

	//Bool
	m_bJumped=			false;
	m_bRUNONCE=			true;
	m_bShooting=		false;
	m_bVaribeJump=		true;
	m_runtimer=			0;
	m_bCantJump=		false;

	//Pointers
	m_pWeapon=			NULL;
	m_pAssault =		new CAssault(true, this);
	
	//Init Collider
	m_pCollider->SetCollisionType(Dynamic);
	m_pCollider->SetSize(Vector2(32,32));
	Global->pColMan->AddICol(m_pCollider);

	m_scale.x=1.5f;
	m_scale.y=1.5f;

/*
	TEST = new CConvex();
	TEST->AddVertex(new Vector4(100,100));
	TEST->AddVertex(new Vector4(100,150));
	TEST->AddVertex(new Vector4(150,100));
	TEST->AddVertex(new Vector4(150,150));
	TEST->CalcConstraints();

	TEST2 = new CConvex();
	TEST2->AddVertex(new Vector4(200,200));
	TEST2->AddVertex(new Vector4(200,250));
	TEST2->AddVertex(new Vector4(250,200));
	TEST2->AddVertex(new Vector4(250,250));
	TEST2->CalcConstraints();
*/

	//QT = new QuadTree();
	//QT->CalcQuadTree(Vector2(0, 0),Vector2(1024,768));
	//TESTQUAD = new ICol();
	
};

CPlayer::~CPlayer(){
	Global->pColMan->getQT()->RemoveEntity(m_pCollider);
};

void CPlayer::Step(CDeviceHandler* DH){
	//Run First Frame Player is Created ONLY
	m_angle=atan2(DH->MousePos().y-m_pos.y,DH->MousePos().x-m_pos.x);
	if (m_bRUNONCE){
		m_bRUNONCE=false;
		Global->EntList->add(m_pAssault);
		Debug->setFlags(CDebug::HitBoxes);
		Debug->setFlags(CDebug::QuadCells);
	}

	//Input

	//Shooting
	if (m_pWeapon != NULL){
		m_pWeapon->setScale(Vector2(m_dir,1));
		
		if (DH->MouseButtonDown(0) && m_pWeapon == m_pAssault){	
			m_bShooting=true;
			Global->EntList->add(m_pWeapon->Shoot());
		
		ENTITY_PARAMS Params;

		Params.pos=Vector2(250, 100);
		Params.width=32;
		Params.height=32;
		Params.accel=Vector2(0, 0);
		Params.stateflags=CEntity::EmptyState;
		Params.depth=0;
		Params.colTYPE = Circle;

		Global->EntList->add(new Ai_Cop(&Params));

		//m_state |= Dead;
		}else{
			m_bShooting=false;
		}
	}
	//Left
	if (DH->KeyDown(DIK_A)){
		m_accel.x = -0.5f;
		m_dir=-1;

		if (m_frame<9){
			if (m_runtimer <= 0){
				++m_frame;
				m_runtimer=10;
			}
			--m_runtimer;
		}else{
				m_frame=6;
		}

		if (m_frame<7)
			m_frame=7;
	}

	//Right
	else if (DH->KeyDown(DIK_D)){
		m_accel.x = 0.5f;
		m_dir=1;

		if (m_frame<9){
			if (m_runtimer <= 0){
				++m_frame;
				m_runtimer=10;
			}
			--m_runtimer;
		}else{
				m_frame=6;
		}

		if (m_frame<7)
			m_frame=7;
	}
	else{
		m_accel.x=0;
		m_frame=0;
	}

	//Switch Weapons
	if (DH->KeyDown(DIK_1)){
		m_pWeapon = m_pAssault;
	}


	//Jumping
	if (m_bJumped)
		m_frame=1;

	m_accel.y=0;

	if (DH->KeyDown(DIK_W)){
		if (!m_bJumped && !m_bCantJump){
			m_bJumped=true;
			m_bVaribeJump=true;
			m_accel.y = -22.0f;
		}
	}else{
		if (m_bJumped && m_bVaribeJump){
			m_bVaribeJump=false;
			int dy = m_prevpos.y-m_pos.y;
			if (dy>0)
				m_accel.y = dy/2;//min (dy/2, 5);
		}
	}

	if ((m_bJumped) || DH->KeyDown(DIK_Z))
		m_bCantJump=true;
	else
		m_bCantJump=false;

	m_bJumped=true;


	//Animations
    if(m_animationa>=360)
		m_animationa=0;
	    if(m_animationa<=-360)
		m_animationa=0;

	m_animationa+=m_accel.x/2;
	m_animationb+=0.05f;


	//Movement
	// VERLET
	Verlet();
	
	//CAMERA
	Global->CAMERA.x=m_pos.x-Global->WINDOW_CENTER.x;
	Global->CAMERA.y=0;//m_pos.y-WINDOW_CENTER.y-200;

	Global->CAMERA.x = floor(min(max(Global->CAMERA.x, 0), Global->LEVEL_WIDTH-Global->WINDOW_WIDTH));
	Global->CAMERA.y = floor(min(max(Global->CAMERA.y, 0), Global->LEVEL_HEIGHT-Global->WINDOW_HEIGHT));

	//Postion Limits
	if (m_pos.x<0) m_pos.x=0;
	if (m_pos.x>Global->LEVEL_WIDTH) m_pos.x=Global->LEVEL_WIDTH;


	//Update Colliders Postion
	m_pCollider ->UpdatePos();
};

void CPlayer::onCollision(const Vector2& collisionResult, CEntity* pCollideEnt)
{
	if ( collisionResult.x == 0){
		m_accel.y = 0;
		if( collisionResult.y < 0 ){
			m_bJumped=false;
		}
	}
}

void CPlayer::Draw(CGraphicsHandler *g){	

	//g->Camera.x = m_pos.x;
	g->setCamera(Global->CAMERA);

/*	g->DrawSprite(
		g->SprList->Char->ME->Feet,
		m_frame,
		Vector2(m_pos.x-sin(m_animationa)*8,m_pos.y+6-(m_pos.y-m_prevpos.y)),
		Vector2(m_scale.x*m_dir, m_scale.y),
		((m_pos.y-m_prevpos.y)*6)*m_dir,
		CLR_WHITE);

	g->DrawSprite(
		g->SprList->Char->ME->Feet,
		m_frame,
		Vector2(m_pos.x+sin(m_animationa)*8+(3*m_dir),m_pos.y+6-(m_pos.y-m_prevpos.y)),
		Vector2(m_scale.x*m_dir, m_scale.y),
		((m_pos.y-m_prevpos.y)*6)*m_dir,
		CLR_WHITE);
	
	g->DrawSprite(
		g->SprList->Char->ME->Torso,
		m_frame,
		Vector2(m_pos.x,m_pos.y-18-((m_pos.y-m_prevpos.y)/1.5f)-sin(m_animationb)*3), 
		Vector2(m_scale.x*m_dir, m_scale.y),
		m_angle/2,
		CLR_WHITE);

	g->DrawSprite(
		g->SprList->Char->ME->FullHead,
		0,
		Vector2(m_pos.x+(((m_pos.x-m_prevpos.x)/1.2f)),m_pos.y-39-sin(m_animationb)*2),
		Vector2(m_scale.x*m_dir, m_scale.y),
		D3DXToDegree(atan2(MOUSE.x-m_pos.x, MOUSE.y-m_pos.y))-(90*m_dir),
		CLR_WHITE);

*/
	
		if (m_bShooting)
			m_frame+=4;
			
/*
		g->DrawSprite(
		g->SprList->Char->ME->Torso,
		m_frame,
		Vector2(m_pos.x+32-(64*m_dir),m_pos.y-64), //
		Vector2(m_scale.x*m_dir, m_scale.y)*2,
		0,//D3DXToDegree(atan2(MOUSE.x-m_pos.x, MOUSE.y-m_pos.y))-(90*m_dir),
		CLR_WHITE);
*/
	/*			g->DrawSprite(
		g->SprList->Char->ME->Torso,
		0,
		Vector2(m_pos.x,m_pos.y),
		Vector2(m_scale.x*m_dir, m_scale.y),
		0,//D3DXToDegree(atan2(MOUSE.x-m_pos.x, MOUSE.y-m_pos.y))-(90*m_dir),
		CLR_WHITE);
		if (m_bShooting)
			m_frame-=4;
	*/
 
	if (m_pWeapon && m_pWeapon->m_pSprite){
		m_pWeapon->setPos(Vector2(m_pos.x,m_pos.y));
	}


	//TEST->Draw(g);
	//TEST2->Draw(g);

	//g_pDrawLine(m_pos, Vector2(0, 0), D3DCOLOR_ARGB(255, 255, 0, 0));
	//DrawText(g_pFontList->Arial20B, "<-Pro Line", m_pos.x, m_pos.y-32, D3DCOLOR_ARGB(255, 0, 0, 255));
	//for(int i=1; i<=4; i++)
		//for(int j=1; j<=4;j++)
	
//	g->DrawTextTL(toStr(Global->ENTITIES),Vector2(16,10),CLR_BLACK);
			//DrawTileset(g->SprList->Level->TSBuilding1, j, i, 200+(32*(float)j), 500+(32*(float)i), 1, 1, 0, D3DXVECTOR3(255, 255, 255), 255);

	/*if( m_lastCollisionPos.x || m_lastCollisionPos.y )
		DrawLine(m_pos,m_lastCollisionPos,D3DCOLOR_RGBA(255,0,0,255));*/

/*	FOREACHS(CCollisionManager::Cell*,m_pCollider->m_pCells,i)
	{
		if( !*i )
			continue;

		FOREACHS(CEntity*,(*i)->m_entList, e)
		{
			DrawLine(m_pos,(*e)->getPos(),D3DCOLOR_RGBA(255,0,0,255));
		}
	}*/
};
void CPlayer::DebugDraw(CGraphicsHandler *g){
	//CEntity::DebugDraw(g);
	//QT->DebugDraw(g);
	//TESTQUAD->DebugDraw(g);
}
