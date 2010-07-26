#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <set>
#include "CEntity.h"
#include "CGraphicsHandler.h"


#define Entity ICol //Change to whatever the quadtree contains (IE. not making a template class, cause that would kill me)
#define CELLMINSIZE 128

enum QuadTreeTypes{
	LEAF,
	STEM,
	ROOT
};

class QuadTree{
private:

	QuadTree* parent;
	Vector2 size;
	int Type;
	Vector2 min;
	Vector2 max;
	bool Drawn;

public:
	QuadTree* children[4];

	std::vector<Entity*> Entlist;

	QuadTree(){
		Type = STEM;
		parent = NULL;
		children[0]=NULL;
		children[1]=NULL;
		children[2]=NULL;	
		children[3]=NULL;
		Drawn = false;
	}
	QuadTree(QuadTree* _parent);
	~QuadTree();

	void CalcQuadTree(Vector2& _min, Vector2& _max);
	QuadTree* AddEntity(Entity* _Ent); //Adds the Entity in the correct spot on the gird
	void RemoveEntity(Entity* _Ent); //Removes selected entity from Grid - O(n)
	void GetEntinCell(Entity* _Ent, std::set<Entity*> *list); //Returns a vector with ALL entities in target cell (includes children)
	void DebugDraw(CGraphicsHandler *g);

	inline int getType(){return Type;}
	inline void setType(int _T){Type =_T;}
	inline QuadTree* getParent() {return parent;}
	inline Vector2 getMin(){return min;}
	inline Vector2 getMax(){return max;}
};

#endif 