#ifndef CGLOBAL_H_
#define CGLOBAL_H_

#include "Main.h"
#include "LinkedList.h"
#include "CollisionManager.h"
#include "CEntity.h"
#include "CPlayer.h"



class CGlobal{
private:
	static CGlobal* m_pInstance;
protected:
	CGlobal(){
		Exit=false;
		Paused=false;
		MousePos=Vector2(0, 0);
		CAMERA=Vector2(0, 0);
		GRAVITY=0.3f;
		LEVEL_WIDTH = 1024;
		LEVEL_HEIGHT = 768;
		WINDOW_WIDTH=1024;
		WINDOW_HEIGHT=768;
		WINDOW_CENTER = Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
		EntList = NULL;
		ENTITIES=0;
		SortList=false;
		pPlayer = NULL;
		pColMan = NULL;
		inGame=false;
	};
public:
	static CGlobal* GetInstance();

	int count;
	bool Exit;
	bool Paused;
	Vector2 MousePos, CAMERA;
	float GRAVITY;
	float ENTITIES;
	float LEVEL_WIDTH, LEVEL_HEIGHT;
	LinkedList<CEntity> *EntList;
	float WINDOW_WIDTH, WINDOW_HEIGHT;
	Vector2 WINDOW_CENTER;
	bool SortList;
	CPlayer* pPlayer;
	ColManager* pColMan;
	bool inGame;
};
#define Global CGlobal::GetInstance()
#endif