/*Entity IDs;
100 - Player
101 - Object
102 - Verlet Object
103 - CBlock
*/

#pragma once
#ifndef CENTITY_H_
#define CENTITY_H_

#include "CollisionManager.h"
#include "Debug.h"
#include <list>

struct ENTITY_PARAMS{
	Vector2 pos, accel;
	UINT width, height;
	float depth;
	int stateflags;
	int colTYPE;
};

class CEntity{
protected:

	Vector2		m_pos, m_accel, m_scale,m_vel;
	float		m_angle, m_depth;
	UINT		m_width, m_height, m_halfwidth, m_halfheight;
	short		m_objectid, m_dir, m_hp;

	int m_state; // holds a bitmask of eStateFlags

	ICol*	m_pCollider;
	

	//Resource Pointers
	//Sprites *Sprite;

public:

	CEntity* m_pParent;

	// CEntity::Solid, CEntity::Invincible, etc
	enum eStateFlags
	{
		EmptyState,				// 0000b
		Solid = 1 << 0,			// 0001b
		Invincible = 1 << 1,	// 0010b
		Immovable = 1 << 2,		// 0100b
		Dead = 1 << 3			// 1000b, etc

	};

	CEntity();
	CEntity(ENTITY_PARAMS* params);
	~CEntity();

	void Init(ENTITY_PARAMS* params);

	virtual void Step(CDeviceHandler *DH) {};
	virtual void Draw(CGraphicsHandler *g) {};

	virtual void DebugDraw(CGraphicsHandler *g) {
		if(Debug->CheckFlag(CDebug::HitBoxes))
			g->DrawRect( m_pos, getWidth(), getHeight(), D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(255,0,0,255));
	}

	virtual void onCollision(const Vector2& result, CEntity* pCollideEnt) {}

	void setPos	(Vector2& _pos)		{ m_pos=_pos; }
	void addPos	(Vector2 _pos)		{ m_pos+=_pos; }
	void setAccel (Vector2& _accel)	{ m_accel=_accel; }
	void addAccel (Vector2& _accel)	{ m_accel+=_accel; }
	void setScale (Vector2& _scale)	{ m_scale=_scale; }
	void addScale (Vector2& _scale)	{ m_scale+=_scale; }
	void setAngle (float _angle)	{ m_angle=_angle; }
	void addAngle (float _angle)	{ m_angle+=_angle; }
	void setDepth (float _depth)	{ m_depth=_depth; }

	void setStateFlags(int flags)	{ m_state |= flags; }
	void unsetStateFlags(int flags)	{ m_state &= ~flags; }

//	void setListID(u32 _id){listid=_id;} //Universal Function
	inline void setObject(u32 _id){m_objectid=_id;}

	ICol*				Collider()	{ return m_pCollider; }
	inline Vector2	getPos()	{ return m_pos; }
	inline Vector2	getAccel()	{ return m_accel; }
	inline Vector2	getScale()	{ return m_scale; }
	inline UINT				getWidth()	{ return m_width; }
	inline UINT				getHeight()	{ return m_height; }
	inline float			getAngle()	{ return m_angle; }
	inline float			getDepth()	{ return m_depth; }
	inline u16				getDir()	{ return m_dir; }
//	u32		getListID()		{ return listid; } 
	inline u32		getObjectID()	{ return m_objectid; }

	int getState() const	{ return m_state; }
};

#endif