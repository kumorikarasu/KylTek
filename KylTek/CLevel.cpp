#include "main.h"
#include "Functions.h"
#include "CLevel.h"
#include "CVObject.h"
#include "CBlock.h"
#include "CPlayer.h"
#include "Ai_Cop.h"
#include "CGlobal.h"
#include "Debug.h"

CEntity* toEntity(wstring _line){
	ENTITY_PARAMS Params;
	ZeroMemory(&Params,sizeof(ENTITY_PARAMS));

	int objectid=toInt(_line.substr(0, _line.find_first_of('|')));
	_line=_line.substr(_line.find_first_of('|')+1);//# of parameters
	
	int Param = toInt(_line.substr(0, _line.find_first_of('|')));
	_line=_line.substr(_line.find_first_of('|')+1);
	
	float x=(float)toInt(_line.substr(0, _line.find_first_of('|')));
	_line=_line.substr(_line.find_first_of('|')+1);
	
	float y=(float)toInt(_line.substr(0, _line.find_first_of('|')));
	
		//Posible Varibles (since u can't declare in a switch statement)
	float width,height;

	switch(objectid){
	case 100:
		Params.pos=Vector2(x+16, y+16);
		Params.width=32;
		Params.height=32;
		Params.accel=Vector2(0, 0);
		Params.stateflags=CEntity::EmptyState;
		Params.depth=0;
		Params.colTYPE = Circle;
		return Global->pPlayer = new CPlayer(&Params);
	case 101: //NOT USED
		return new CObject();
	case 102: //NOT USED
		return new CVObject();
	case 103: //Basic Block
		//Parems: Width, Height
		_line=_line.substr(_line.find_first_of('|')+1);
		width=(float)toInt(_line.substr(0, _line.find_first_of('|')));
		_line=_line.substr(_line.find_first_of('|')+1);
		height=(float)toInt(_line.substr(0, _line.find_first_of('|')));
		
		Params.width=width;
		Params.height=height;
		Params.pos=Vector2(x+width/2, y+height/2);
		Params.accel=Vector2(0, 0);
		Params.stateflags=CEntity::Solid | CEntity::Immovable | CEntity::Invincible;
		Params.depth=0;
		return new CBlock(&Params, NULL);

	case 104:
		Params.pos=Vector2(x+16, y+16);
		Params.width=32;
		Params.height=32;
		Params.accel=Vector2(0, 0);
		Params.stateflags=CEntity::EmptyState;
		Params.depth=0;
		Params.colTYPE = Circle;
		return new Ai_Cop(&Params);
	default:
		return NULL;
	}
	return NULL;
}

CLevel::CLevel(){
	Global->EntList = new LinkedList<CEntity>();
//	 m_pCList= new LinkedList<CConstraint>();
	 m_bLoaded=false;
};

CLevel::~CLevel(){};

void CLevel::LoadLevel(LPTSTR filename){
	

	wstring line;
	ifstream file(filename);
	//file.open ;
	if(file.is_open()){
		getline(file, line);
		Global->LEVEL_WIDTH=(float)toInt(line);
		getline(file, line);
		Global->LEVEL_HEIGHT=(float)toInt(line);
		Global->pColMan->Init(Vector2(Global->LEVEL_WIDTH,Global->LEVEL_HEIGHT));
		getline(file, line);
		getline(file, line);
		while(!file.eof()){
			getline(file, line);
			if(line!="")
				Global->EntList->add(toEntity(line));
		}
	}
//Add Any Extra Testing Objects here (that arn't part of the level)


/*
	ENTITY_PARAMS Params;

		Params.pos=Vector2(250, 100);
		Params.width=32;
		Params.height=32;
		Params.accel=Vector2(0, 0);
		Params.stateflags=CEntity::EmptyState;
		Params.depth=0;
		Params.colTYPE = AABB;
	Global->EntList->add(new Ai_Cop(&Params));
*/

	Global->SortList=true;
	m_bLoaded=true;
};

void CLevel::DumpLevel(){
	m_pBackground=NULL;
	Global->EntList->clear();
	Global->EntList->Init();
	Global->pColMan->Clear();
//	m_pCList->clear();
//	m_pCList->Init();
	m_bLoaded=false;
};

/*-------------------------------------------------------------------
-Level Step
-------------------------------------------------------------------*/
void CLevel::Step(CDeviceHandler* DH){
	Global->MousePos.x=(float)DH->MousePos().x+Global->CAMERA.x;
	Global->MousePos.y=(float)DH->MousePos().y+Global->CAMERA.y;
	
//	Global->pColMan->DoCollisions();

	m_pEntityIt = Global->EntList->first();
	while(m_pEntityIt!=NULL){
		m_pEntity = m_pEntityIt->object;
		if(m_pEntity){
			m_pEntity -> Step(DH); //the objects reoccuring event (Time Step)

			if (m_pEntity->getState() & CEntity::Dead){ //deadflag set?
				delete m_pEntity;
				//SAFE_DELETE(m_pEntity);
				m_pEntityIt = Global->EntList->remove(m_pEntityIt);
			}
		}
		m_pEntityIt = m_pEntityIt -> next;
	}

	Global->pColMan->DoCollisions();
	//Satisfy Constraints
	/*m_pConstraintIt = m_pCList->first();
	while(m_pConstraintIt!=NULL){
		m_pConstraint = m_pConstraintIt->object;
		if(m_pConstraintIt->object!=NULL)
			m_pConstraint ->Satisfy();
		m_pConstraintIt = m_pConstraintIt -> next;
	}*/
};

/*-------------------------------------------------------------------
-Level Draw
-------------------------------------------------------------------*/
void CLevel::Draw(CGraphicsHandler *g){
	if(Global->SortList){
		//SortEntList(Global->EntList);
		Global->SortList=false;
	}
	if (!Global->inGame){
		
	}else{
//		g->DrawSprite(g->SprList->Bg->City1, 0, Vector2(((int)Global->pPlayer->getPos().x / 3) % 1536,150-32),Vector2(3,3),0,D3DCOLOR(0xFFFFFFFF));
//		g->DrawSprite(g->SprList->Bg->City1, 0, Vector2(((int)(Global->pPlayer->getPos().x / 3)-1536 ) % 1536,150-32),Vector2(3,3),0,D3DCOLOR(0xFFFFFFFF));
	}

	//Background

	//Tiles


	//	g->DrawSprite(	g->SprList->Char->ME->JetPack,	0,	Vector2(512,384)); //BACKGROUND
	m_pEntityIt = Global->EntList->first();
	while(m_pEntityIt!=NULL){
		m_pEntity = m_pEntityIt->object;
		if(m_pEntity){
			m_pEntity -> Draw(g);
			if(Debug->CheckFlag(CDebug::Enabled))
				m_pEntity->DebugDraw(g);
		}
		m_pEntityIt = m_pEntityIt -> next;
	}

	//Foreground Tiles

	Global->pColMan->DebugDraw(g);

	//		g->DrawSprite(g->SprList->Menu->menuBgk, 0, Vector2(512,384),Vector2(1,1),0,D3DCOLOR(0xFFFFFFFF));
};


void CLevel::CleanUp(){
	SAFE_DELETE( Global->EntList );
};

