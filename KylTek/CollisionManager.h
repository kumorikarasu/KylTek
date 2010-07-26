#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include "CDeviceHandler.h"
#include "CGraphicsHandler.h"
#include "LinkedList.h"

class CEntity;
class CConvex;
class QuadTree;

enum eColType
{
    Undefined,
    AABB,
    Circle,
    Convex
};

enum eCollisionType
{
	// does not collide with anything
	NoCollide,

	// opposite of passive, things can collide against it but does not try to collide with objects (in grid)
	Static,

	// trys to collide with objects and objects try to collide with it (in grid)
	Dynamic,

	// opposite of static, trys to collide with objects but nothing trys to collide with it (not in grid)
	Passive,

	// The Special Case required to collide with hitboxes (same as Passive otherwise)
	CanHitBox,

	// For Hitboxs
	HitBox
};

enum eCollisionFlag
{
	// do not correct position on collision
	NoPush = 1 << 0
};

//*****************************
// Interface Collider
//*****************************
class ICol{
private:
	
public:
	bool			isDead;
	CEntity*		m_pParent;
	float			m_w, m_h, m_hy, m_hx, m_xoffset, m_yoffset;
	Vector2			max,min;
	QuadTree*		QT;
	int				Type;
	int				m_collisionFlags;

	ICol(CEntity* pParent, int collisionType=Static);


	~ICol();

	virtual int		GetType() const = 0;
	virtual Vector2	Collide( ICol* pTarget ) = 0; 
	virtual void	SetSize(Vector2 size);

	inline CEntity* Parent();
	inline void		Parent(CEntity* _ent);

	inline int		GetCollisionType() const { return Type; }
	inline int		GetCollisionFlags() const { return m_collisionFlags; }
	inline void		SetCollisionFlags(int flags){ m_collisionFlags |= flags; }
	inline void		SetCollisionType(eCollisionType _Type){	Type=_Type; }
	inline void		RemoveQT();
	
	void			UpdatePos();
	void			DebugDraw(CGraphicsHandler *g);

	void			(CEntity::*Collision)(const Vector2&, CEntity*); //Collision Function Pointer
};

//*****************************
// Interface AABB Collider
//*****************************
class ICol_AABB : public ICol
{
public:
	
	ICol_AABB(CEntity *pParent,bool bInheritSize=true);
	~ICol_AABB () {}

	inline int		GetType()	const { return AABB; }
	Vector2			Collide( ICol* pTarget );
	void			SetSize(Vector2 size);
};


//*****************************
// Interface Circle Collider
//*****************************
class ICol_Circle : public ICol
{
public:
	float m_r;

	ICol_Circle(CEntity *pParent,bool bInheritSize=true);
	~ICol_Circle () {}

	inline int		GetType()	const { return Circle; }
	Vector2			Collide( ICol* pTarget );
	void			SetSize(Vector2 size);
};

//*****************************
// Interface Convex Collider
//*****************************
class ICol_Convex : public ICol
{
public:
	ICol_Convex(CEntity *pParent,bool bInheritSize=true);
	~ICol_Convex () {}

	inline int		GetType()	const { return Convex; }
	Vector2			Collide( ICol* pTarget );
	void			SetSize(Vector2 size);

	void			UpdatePos();
};


//*****************************
// Collision Manager
//*****************************
class ColManager{
private:
	QuadTree* QT;
	LinkedList<ICol>* ICollist;

public:
	ColManager();
	~ColManager();

	void			Clear(); //Removes Everything
	void			Init(Vector2& Size); //Creates a new QuadTree
	void			AddICol(ICol *_icol);
	void			DoCollisions();
	inline QuadTree*getQT(){return QT;}
	void			DebugDraw(CGraphicsHandler *g);
	inline void		New(){ICollist->clear();}
};

void CollideAABBvsAABB(ICol_AABB* a, ICol_AABB* b, Vector2& result);
void CollideCirclevsAABB(ICol_Circle* a, ICol_AABB* b, Vector2& result);
void CollideCirclevsCircle(ICol_Circle* a, ICol_Circle* b, Vector2& result);
void CollideConvexvsConvex(ICol_Convex* a, ICol_Convex* b, Vector2& result);

#endif