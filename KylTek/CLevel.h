#pragma once
#ifndef CLEVEL_H_
#define CLEVEL_H_

#include "CEntity.h"
#include "Linkedlist.h"

class CLevel{
private:
	CSprite* m_pBackground;
	//Entity Management Varibles
	//std::list<CEntity*>* m_pEntList;
	//std::list<CEntity*>::iterator m_EntListIt;
	CEntity* m_pEntity;
	Node<CEntity>* m_pEntityIt;

	bool m_bLoaded, m_bSortList;
	UINT Width, Height;
public:
	CLevel();
	~CLevel();

	bool LevelLoaded(){return m_bLoaded;}

	void LoadLevel(LPTSTR filename);
	void DumpLevel();
	void Step(CDeviceHandler* DH);
	void Draw(CGraphicsHandler *g);
	void CleanUp();
};


#endif