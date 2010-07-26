#include "Main.h"
#include "QuadTree.h"
#include "Foreach.h"
#include "Debug.h"
#include "CGlobal.h"

QuadTree::QuadTree(QuadTree* _parent){
	parent=_parent;
	Type = STEM;
	children[0]=NULL;
	children[1]=NULL;
	children[2]=NULL;	
	children[3]=NULL;
	Drawn = false;
}

QuadTree::~QuadTree(){
}

void QuadTree::CalcQuadTree(Vector2& _min, Vector2& _max){
	//Set Cell Sizes
	min = _min;
	max = _max;
	size.x = max.x - min.x;
	size.y = max.y - min.y;

	//cell size check
	if (size.x > CELLMINSIZE){
		//subdivide
		for (int a=0;a<4;a++){ //Create 4 sections
			children[a] = new QuadTree(this);
			children[a]->Type = STEM;
		}

		Vector2 topleft,bottomright;
		//TOP LEFT
			topleft.x = min.x;
			topleft.y = min.y;
			bottomright.x= max.x - size.x /2;
			bottomright.y= max.y - size.y /2;
			children[0]->CalcQuadTree(topleft ,bottomright);

		//TOP RIGHT
			topleft.x = min.x + size.x /2;
			topleft.y = min.y;
			bottomright.x= max.x;
			bottomright.y= max.y - size.y /2;
			children[1]->CalcQuadTree(topleft,bottomright);

		//BOTTOM LEFT
			topleft.x= min.x;
			topleft.y= min.y + size.y /2;
			bottomright.x= max.x - size.x /2;
			bottomright.y= max.y;
			children[2]->CalcQuadTree(topleft,bottomright);

		//BOTTOM RIGHT
			topleft.x = min.x + size.x /2;
			topleft.y = min.y + size.y /2;
			bottomright.x = max.x;
			bottomright.y = max.y;
			children[3]->CalcQuadTree(topleft,bottomright);
/**/
	}else{
		Type=LEAF;
	}
}


QuadTree* QuadTree::AddEntity(Entity* _Ent){  //Most basic quadtree adding, alot more complicated/better ones exist, thou i don't know them
	Drawn=false;
	//Check if its a leaf or stem
	if (Type == STEM){
		//TOP LEFT
		QuadTree* QT = children[0]; 
		if (QT->max.x > _Ent->min.x && QT->max.y > _Ent->min.y){
			QT->AddEntity(_Ent);
		}
			
		//TOP RIGHT
		QT = children[1]; 
		if (QT->min.x < _Ent->max.x && QT->max.y > _Ent->min.y){
			QT->AddEntity(_Ent);
		}

		//BOTTOM LEFT
		QT = children[2]; 
		if (QT->max.x > _Ent->min.x && QT->min.y < _Ent->max.y){
			QT->AddEntity(_Ent);
		}

		//BOTTOM RIGHT
		QT = children[3];
		if (QT->min.x < _Ent->max.x && QT->min.y < _Ent->max.y){
			QT->AddEntity(_Ent);
		}

	}else{
		Entlist.push_back(_Ent);
		return NULL;
	}
	return NULL;
}

void QuadTree::RemoveEntity(Entity* _Ent){ // O(n)
	//Start at root node, and work its way down
		if (Type == STEM){
		//TOP LEFT
		QuadTree* QT = children[0]; 
		if (QT->max.x > _Ent->min.x && QT->max.y > _Ent->min.y){
			QT->RemoveEntity(_Ent);
		}
			
		//TOP RIGHT
		QT = children[1]; 
		if (QT->min.x < _Ent->max.x && QT->max.y > _Ent->min.y){
			QT->RemoveEntity(_Ent);
		}

		//BOTTOM LEFT
		QT = children[2]; 
		if (QT->max.x > _Ent->min.x && QT->min.y < _Ent->max.y){
			QT->RemoveEntity(_Ent);
		}

		//BOTTOM RIGHT
		QT = children[3];
		if (QT->min.x < _Ent->max.x && QT->min.y < _Ent->max.y){
			QT->RemoveEntity(_Ent);
		}

	}else{
		FOREACH(Entity*,Entlist,a){
			if ((*a) == _Ent){
				Entlist.erase(a);
				break;
			}
			//if (Entlist.empty())
			//	break;
		}
	}
	/*
	FOREACH(Entity*,Entlist,a){
		if ((*a) == _Ent){
			Entlist.clear();
			break;
		}
	}
	

	if (Type == STEM)
		for (int a=0;a<4;a++)
			children[a]->RemoveEntity(_Ent);
	*/
}

void QuadTree::GetEntinCell(Entity* _Ent, set<Entity*> *list){

	Global->count++;

	if (Type == STEM){
		//TOP LEFT
		QuadTree* QT = children[0]; 
		if (QT->max.x > _Ent->min.x && QT->max.y > _Ent->min.y){
			QT->GetEntinCell(_Ent, list);
		}
			
		//TOP RIGHT
		QT = children[1]; 
		if (QT->min.x < _Ent->max.x && QT->max.y > _Ent->min.y){
			QT->GetEntinCell(_Ent, list);
		}

		//BOTTOM LEFT
		QT = children[2]; 
		if (QT->max.x > _Ent->min.x && QT->min.y < _Ent->max.y){
			QT->GetEntinCell(_Ent, list);
		}

		//BOTTOM RIGHT
		QT = children[3];
		if (QT->min.x < _Ent->max.x && QT->min.y < _Ent->max.y){
			QT->GetEntinCell(_Ent, list);
		}
	}else{
		FOREACH(Entity*,Entlist,a)
			list->insert((*a));
	}
}

void QuadTree::DebugDraw(CGraphicsHandler *g){
	if(Debug->CheckFlag(CDebug::QuadCells)){
		if (!Drawn){
			Drawn=true;
			if (!Entlist.empty() || Type==STEM){
				g->DrawRect(min+size/2,(int)size.x,(int)size.y,D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(255,255,0,255));
				if (parent!=NULL)
					parent->DebugDraw(g);
			}
			/*if (Type == STEM)
				for (int a=0;a<4;a++)
					children[a]->DebugDraw(g);
		*/
			
		}
	}
}