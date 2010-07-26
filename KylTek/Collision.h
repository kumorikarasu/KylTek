/*
#ifndef _COLLISION_H
#define _COLLISION_H

#include "CEntity.h"
#include "LinkedList.h"
#include <set>
#include <list>

class CVObject;
class Cell;
class CEntity;
class CCollisionManager;
class ICollider;

#define DEFAULT_CELL_SIZE 256

enum eColliderType
{
    Undefined,
    AABB,
    Circle,
    ConvexPolygon
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


class CCollisionManager
{
	int m_cellSize;
	int m_cellHalf;

	int m_cellColumns;

	void DestroyGrid();
	void UpdateGridPosition(CEntity* pEnt);
	void CollideEntity(CEntity* pEnt);

public:

	class Cell
	{
	public:
		set<CEntity*> m_entList;
		Vector2 pos;

		Cell() : pos(0.0f,0.0f) {}

	};

	CCollisionManager() : m_cellSize(DEFAULT_CELL_SIZE) {}
	~CCollisionManager() {}

	void DoCollisions();
	void DebugDraw();

	void SetSize(int levelX,int levelY,int cellSize=DEFAULT_CELL_SIZE);

	inline int GetSize() const { return (int)m_vGrid.size(); }
	inline int GetCellSize() const { return m_cellSize; }
	Cell* GetGridCell(const Vector2& position);

	inline bool IsValidPos(const int pos) const { return (pos >= 0 && pos < GetSize()); }
private:

	void GetPositions(CEntity* pEnt, Cell** oldPositions);
	void GetOldPositions(CVObject* pEnt, Cell** newPositions);

	inline void RemoveEntityFromCells(CEntity* pEnt, Cell** cells);
	inline void AddEntityToCells(CEntity* pEnt, Cell** cells);
	inline void AddCellsToEntity(Cell** cells, CEntity* pEnt);

	vector<Cell> m_vGrid;
};




class ICollider
{
	// only the collision manager is allowed to handle collisions
	friend class CCollisionManager;

protected:

	list <ICollider*> m_hitBoxList;

	CEntity* m_pParent;
	int m_collisionFlags;
	int m_collisionType;

	// used by the collision manager
	set<CEntity*> m_pCollideEnts;

	virtual Vector2	Collide( ICollider* pTarget ) = 0; 

public:
	
	set<CCollisionManager::Cell*> m_pCells;
	
	float m_w, m_h, m_hy, m_hx;
	


	// ALWAYS tied to an entity
	ICollider(CEntity* pParent, int collisionType=Static) 
		: m_pParent(pParent), m_collisionType(collisionType), m_collisionFlags(0)
	{}

    virtual ~ICollider() {}

    virtual int			GetType() const = 0;
	CEntity*			GetParent() const { return m_pParent; }
	int					GetCollisionType() const { return m_collisionType; }
	int					GetCollisionFlags() const { return m_collisionFlags; }

	void				SetCollisionType(eCollisionType type) { m_collisionType = type; }
	void				SetCollisionFlags(int flags){ m_collisionFlags |= flags; }
	void				AddHitBox(ICollider* col);
    
};





class CCollider_AABB : public ICollider
{


public:
	
	CCollider_AABB(CEntity *pParent,bool bInheritSize=true);
	~CCollider_AABB () {}

	inline int	GetType()	const { return AABB; }
	Vector2		Collide( ICollider* pTarget );
	void		SetSize(Vector2 size);
};


class CCollider_Circle : public ICollider
{
	

public:
	float m_r;

	CCollider_Circle(CEntity *pParent,bool bInheritSize=true);
	~CCollider_Circle () {}

	inline int	GetType()	const { return Circle; }
	Vector2		Collide( ICollider* pTarget );
	void		SetSize(Vector2 size);
};

// Helper functions which will come into play so that I dont duplicate code
// objects a or b can be either collider
/*
void CollideAABBvsAABB(CCollider_AABB* a, CCollider_AABB* b, Vector2& result);
void CollideCirclevsAABB(CCollider_Circle* a, CCollider_AABB* b, Vector2& result);

ICollider* MakeCollider(eColliderType type, CEntity* pParent, bool bInheritSize = true);
/
#endif

*/