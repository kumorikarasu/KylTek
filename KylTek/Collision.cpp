/*
#include "Collision.h"
#include "Math.h"
#include "Bullet.h"
#include "CEntity.h"
#include "CGlobal.h"
#include "Foreach.h"

void CCollisionManager::DestroyGrid()
{
	for(vector<Cell>::iterator i = m_vGrid.begin(); i != m_vGrid.end(); ++i)
	{
		i->m_entList.clear();
	}

	m_vGrid.clear();
}

void CCollisionManager::DoCollisions()
{
	if( !Global->EntList )
		return;

	CEntity* pEnt = NULL;
	Node<CEntity>* i;
	i = Global->EntList->first();
	
	for(;i != NULL; i = i->next)
	{
		pEnt = i->object;
		if(pEnt){
			if( pEnt->Collider()->m_collisionType == NoCollide )
				continue;

			UpdateGridPosition(pEnt);
			
			if( pEnt->Collider()->m_collisionType != Static )
			{
				CollideEntity(pEnt);
			}
		}
	}
}


void CCollisionManager::CollideEntity(CEntity* pEnt)
{
	if( !pEnt )
		return;

	ICollider *pCol = pEnt->Collider();

	Vector2 result;

	// reference to the current entities touched cells
	set<Cell*>& pCells = pCol->m_pCells;
	
	//bool flag = false;

	FOREACHS(Cell*,pCells,c){
		
		if (pEnt->m_objectid=1000){
			int a = pCells.size();
			c++;
			if( c == pCells.end()) break;
			/*c++;
			
			c++;
			if( c == pCells.end() ) break;
			c++;
			if( c == pCells.end() ) break;
			c++;
			if( c == pCells.end() ) break;*
		}

		if( !(*c) )
			continue;

		
		// for each entity in the cell list
		FOREACHS(CEntity*,(*c)->m_entList,e)
		{
				if( pEnt == (*e) )
					continue;
				
				// for hitboxed objects
				ICollider *a = (*e)->Collider();
				if (pCol->GetCollisionType() == CanHitBox && !a->m_hitBoxList.empty()){

					FOREACHL(ICollider*,a->m_hitBoxList,f){
						
						result = pCol->Collide( (*f) );

						if( result.x || result.y ){
							//if( !(pCol->GetCollisionFlags() & NoPush) )
							pEnt->addPos(result);
						}

						pEnt->onCollision( result, (*e) );
					}

				}
			
				result = pCol->Collide( (*e)->Collider() );

				if( result.x || result.y )
				{

					if( !(pCol->GetCollisionFlags() & NoPush) )
						pEnt->addPos(result);

					if( pEnt->Collider()->m_collisionType == Dynamic )
					{
						
					}
					pEnt->onCollision( result, (*e) );
				}
		}
	}
	// for each cell that the entity is currently in
	FOREACHS(Cell*,pCells,c)
	{
		if (pEnt->m_objectid=1000){
		int a = pCells.size();
		}

		if( !(*c) )
			continue;
		// for each entity in the cell list
		FOREACHS(CEntity*,(*c)->m_entList,e)
		{
				if( pEnt == (*e) )
					continue;
				
				// for hitboxed objects
				ICollider *a = (*e)->Collider();
				if (pCol->GetCollisionType() == CanHitBox && !a->m_hitBoxList.empty()){

					FOREACHL(ICollider*,a->m_hitBoxList,f){
						
						result = pCol->Collide( (*f) );

						if( result.x || result.y ){
							//if( !(pCol->GetCollisionFlags() & NoPush) )
							pEnt->addPos(result);
						}

						pEnt->onCollision( result, (*e) );
					}

				}
			
				result = pCol->Collide( (*e)->Collider() );

				if( result.x || result.y )
				{

					if( !(pCol->GetCollisionFlags() & NoPush) )
						pEnt->addPos(result);

					if( pEnt->Collider()->m_collisionType == Dynamic )
					{
						
					}
					pEnt->onCollision( result, (*e) );
				}
		}
	}
}


void CCollisionManager::SetSize(int levelX, int levelY, int cellSize)
{
	int x_size = levelX/cellSize;
	int y_size = (levelY/cellSize)+1;

	int size = x_size*y_size;
	//int pos = 0;
	int j = 0;      

	if( !size )
		return;

	m_cellSize = cellSize;
	m_cellHalf = cellSize/2;
	m_cellColumns = x_size;

	DestroyGrid();

	// efficient and prevents invalidation
	m_vGrid.reserve(size);
	m_vGrid.insert( m_vGrid.begin(), size, Cell() );

	// Init relational cell pointers
	for(vector<Cell>::iterator i = m_vGrid.begin(); i != m_vGrid.end(); ++i, ++j)
	{
		i->pos.x = (float)(((j*m_cellSize)+m_cellHalf) % levelX);
		i->pos.y = (floor((float)j/x_size)*m_cellSize)+m_cellHalf;

		/*pos = j - x_size;

		// north cell
		if( pos >= 0 )
			i->m_nCell = &m_vGrid[pos];

		pos = j - 1;

		// west cell
		if( pos >= 0 )
			i->m_wCell = &m_vGrid[pos];

		pos = j + 1;

		// east cell
		if( pos < size )
			i->m_eCell = &m_vGrid[pos];

		pos = j + x_size;

		// south cell
		if( pos < size )
			i->m_sCell = &m_vGrid[pos];*
			
	}
}


void CCollisionManager::DebugDraw()
{
/*	FOREACH(Cell,m_vGrid,i)
	{
		DrawRect(i->pos,256,256,D3DCOLOR_RGBA(0,0,0,0),1,D3DCOLOR_RGBA(0,255,0,128));
	} 
*
	//Cell* playerCell = g_pPlayer->Collider()->
}




CCollisionManager::Cell* CCollisionManager::GetGridCell(const Vector2& position)
{
	int pos = int((floor(position.x/m_cellSize) + (floor(position.y/m_cellSize)*m_cellColumns)));

	if( IsValidPos(pos) )
	{
		return &(m_vGrid.at(pos));
	}
	else
		return NULL;
}


void CCollisionManager::UpdateGridPosition(CEntity* pEnt)
{
	Cell *oldPos[4],*newPos[4];
	ICollider* pCollider = pEnt->Collider();
//TODO: Create loop here to cycle through hitboxes

	switch(pCollider->m_collisionType)
	{
	case Static:
	case HitBox:
		//TODO: Add hitboxes to this
		if( pCollider->m_pCells.size() )
			break;

		GetPositions(pEnt,oldPos);
		AddEntityToCells(pEnt,oldPos); //Change to ICol
		break;

	case Dynamic:
		GetPositions(pEnt,newPos);
		// HACK: only CVObjects should be dynamic
		GetOldPositions((CVObject*)pEnt,oldPos);

		// skip if nothing changed
		if( (oldPos[0] == newPos[0]) &&
			(oldPos[1] == newPos[1]) &&
			(oldPos[2] == newPos[2]) &&
			(oldPos[3] == newPos[3]) )
				break;

		RemoveEntityFromCells(pEnt,oldPos);
		AddEntityToCells(pEnt,newPos);
		AddCellsToEntity(newPos,pEnt);
		break;

	case Passive:
	case CanHitBox:
		GetPositions(pEnt,oldPos);
		AddCellsToEntity(oldPos,pEnt);
		break;
	}
	

}

void CCollisionManager::GetPositions(CEntity* pEnt, Cell** oldPositions)
{
	float hx = pEnt->Collider()->m_hx;
	float hy = pEnt->Collider()->m_hy;

	Vector2 topLeft,bottomLeft,topRight,bottomRight;
	topLeft = pEnt->getPos();
	bottomLeft = topRight = bottomRight = topLeft;

	topLeft.x -= hx;
	topLeft.y -= hy;

	topRight.x += hx;
	topRight.y -= hy;
	
	bottomLeft.x -= hx;
	bottomLeft.y -= hy;

	bottomRight.x += hx;
	bottomRight.y += hy;

	oldPositions[0] = GetGridCell( topLeft );
	oldPositions[1] = GetGridCell( topRight );
	oldPositions[2] = GetGridCell( bottomRight );
	oldPositions[3] = GetGridCell( bottomLeft );
}



void CCollisionManager::GetOldPositions(CVObject* pEnt, Cell** newPositions)
{
	float hx = pEnt->Collider()->m_hx;
	float hy = pEnt->Collider()->m_hy;

	Vector2 topLeft,bottomLeft,topRight,bottomRight;
	topLeft = pEnt->getPrevPos();
	bottomLeft = topRight = bottomRight = topLeft;

	topLeft.x -= hx;
	topLeft.y -= hy;

	topRight.x += hx;
	topRight.y -= hy;
	
	bottomLeft.x -= hx;
	bottomLeft.y -= hy;

	bottomRight.x += hx;
	bottomRight.y += hy;

	newPositions[0] = GetGridCell( topLeft );
	newPositions[1] = GetGridCell( topRight );
	newPositions[2] = GetGridCell( bottomRight );
	newPositions[3] = GetGridCell( bottomLeft );
}


	//TODO: Chagne all these to use ICol
void CCollisionManager::RemoveEntityFromCells(CEntity* pEnt, Cell** cells)
{
	// FIXME: Optimize this so that it doesn't have to remove itself from every set
	// this is the easy way if we're going to support more complex objects

	// erase the entity from all the old cells
	for(int i = 0; i < 4; ++i )
	{
		if(cells[i])
			cells[i]->m_entList.erase(pEnt);
	}
}



void CCollisionManager::AddEntityToCells(CEntity* pEnt, Cell** cells)
{
	// add the entity to all the new cells
	// sets only hold unique items
	for(int j = 0; j < 4; ++j )
	{
		if( cells[j] )
			cells[j]->m_entList.insert(pEnt);
	}
}



void CCollisionManager::AddCellsToEntity(Cell** cells, CEntity* pEnt )
{
	// when adding cells we must remove old ones :(
	pEnt->Collider()->m_pCells.clear();

	for(int j = 0; j < 4; ++j )
	{
		pEnt->Collider()->m_pCells.insert(cells[j]);
	}
}
	//END




ICollider* MakeCollider(eColliderType type, CEntity* pParent, bool bInheritSize)
{
	if( !pParent )
		return (ICollider*)NULL;

	switch( type )
	{
	case AABB:
		return new CCollider_AABB(pParent,bInheritSize);
	default:
		return (ICollider*)NULL;
	}
}


// -------------------------------------------------
//   Collider_AABB
// -------------------------------------------------
Vector2 CCollider_AABB::Collide(ICollider* pTarget)
{
	Vector2 v(0.0f,0.0f);

	switch(pTarget->GetType())
	{
	case AABB:
		CollideAABBvsAABB(this, (CCollider_AABB*) pTarget, v);
		break;
	}

	return v;
}


CCollider_AABB::CCollider_AABB(CEntity *pParent,bool bInheritSize) : ICollider(pParent) 
{
	if( m_pParent )
	{
		if( bInheritSize )
		{
			m_w = (float)m_pParent->getWidth();
			m_h = (float)m_pParent->getHeight();
			m_hx = (float)m_pParent->getWidth()/2;
			m_hy = (float)m_pParent->getHeight()/2;
		}
	}
}


void CCollider_AABB::SetSize(Vector2 size)
{
	m_h = size.y;
	m_w = size.x;

	m_hx = size.x/2;
	m_hy = size.y/2;
}

// -------------------------------------------------
//   Collider_Circle
// -------------------------------------------------
Vector2 CCollider_Circle::Collide(ICollider* pTarget)
{
	Vector2 v(0.0f,0.0f);

	switch(pTarget->GetType())
	{
	case AABB:
		CollideCirclevsAABB(this, (CCollider_AABB*) pTarget, v);
		break;
	}

	return v;
}


CCollider_Circle::CCollider_Circle(CEntity *pParent,bool bInheritSize) : ICollider(pParent) 
{
	if( m_pParent )
	{
		if( bInheritSize )
		{
			m_w = (float)m_pParent->getWidth();
			m_h = (float)m_pParent->getHeight();
			m_hx = (float)m_pParent->getWidth()/2;
			m_hy = (float)m_pParent->getHeight()/2;
			m_r = m_hx;
		}
	}
}


void CCollider_Circle::SetSize(Vector2 size)
{
	m_h = size.y;
	m_w = size.x;

	m_hx = size.x/2;
	m_hy = size.y/2;
}

//********************
// AABB vs AABB
//********************

// Let's try to make this readable this time...
/*
void CollideAABBvsAABB(ICol_AABB* a, ICol_AABB* b, Vector2& result){
/
		Vector2 pos		= a->GetParent()->getPos();
		float A_left	= pos.x - a->m_hx;
		float A_right	= pos.x + a->m_hx;
		float A_top		= pos.y - a->m_hy;
		float A_bottom	= pos.y + a->m_hy;
		float A_hx		= a->m_hx;			// x half width
		float A_hy		= a->m_hy;			// y half width
		float A_hx_pos  = pos.x; 
		float A_hy_pos  = pos.y;		// half width positions

		pos				= b->GetParent()->getPos();
		float B_left	= pos.x - b->m_hx;
		float B_right	= pos.x + b->m_hx;
		float B_top		= pos.y - b->m_hy;
		float B_bottom	= pos.y + b->m_hy;
		float B_hx		= b->m_hx;
		float B_hy		= b->m_hy;
		float B_hx_pos	= pos.x;
		float B_hy_pos	= pos.y;
				
		float dx, dy, px, py; // differences

		result.x = 0;
		result.y = 0;


		if( A_right	>=	B_left &&
			A_left	<=	B_right &&
			A_bottom >=	B_top &&
			A_top <=	B_bottom )	// Check if A is inside of B
		{
			// Half-width difference
			dx = A_hx_pos - B_hx_pos;

			// Get how much x is penetrating
			px = (A_hx+B_hx)-abs(dx);

			if( px > 0 )
			{
				dy = A_hy_pos - B_hy_pos;
				// how much is y penetrating?
				py = (A_hy+B_hy)-abs(dy);

				if( py > 0 ){
					// does x have less penetration?
					if( px < py ){
						// move left
						if( dx < 0 )
							result.x = -px;
						else
							result.x = px;
					}
					else{ // more y penetration
						if( dy < 0 )
							result.y = -py;
						else
							result.y = py;
					}
				}
			}

		}
		*
}

//********************
// Circle vs AABB
//********************

void CollideCirclevsAABB(ICol_Circle* a, ICol_AABB* b, Vector2& result){
/*
	result.x = 0;
	result.y = 0;

	Vector2 ColCornerPoint = Vector2(0,0);

	Vector2 pos = b->GetParent()->getPos();
	float B_left	= pos.x - b->m_hx;
	float B_right	= pos.x + b->m_hx;
	float B_top		= pos.y - b->m_hy;
	float B_bottom	= pos.y + b->m_hy;
	float B_hx		= b->m_hx;
	float B_hy		= b->m_hy;
	float B_hx_pos	= pos.x;
	float B_hy_pos	= pos.y;

	pos = a->GetParent()->getPos();
	float radius = a->m_r;
	float A_left	= pos.x - a->m_hx;
	float A_right	= pos.x + a->m_hx;
	float A_top		= pos.y - a->m_hy;
	float A_bottom	= pos.y + a->m_hy;
	float A_hx		= a->m_hx;			// x half width
	float A_hy		= a->m_hy;			// y half width
	float A_hx_pos  = pos.x; 
	float A_hy_pos  = pos.y;		// half width positions





	//Check Corners of rectangle
    if( A_hx_pos < B_left ) ColCornerPoint.x = B_left;
    else if( A_hx_pos > B_right ) ColCornerPoint.x = B_right;
    if( A_hy_pos < B_top ) ColCornerPoint.y = B_top;
    else if( A_hy_pos > B_bottom ) ColCornerPoint.y = B_bottom;
    
	if (ColCornerPoint.x != 0 && ColCornerPoint.y !=0){
		Vector2 diff = ColCornerPoint - pos;
		float dis = sqrt(diff.x * diff.x + diff.y * diff.y);
		if(dis < radius ){ //Penetrated
			dis = radius - dis; //Calc Displacement Vector
			float dir = atan2(pos.y-ColCornerPoint.y,pos.x-ColCornerPoint.x);//
			//Calc Amount to move the circle
			result.x =   cos(dir)*dis;
			result.y =   sin(dir)*dis;
		}
	}
	else
	{
	//Check Sides of rectangle (AABB)

		float dx, dy, px, py; // differences

		if( A_right	>=	B_left &&
			A_left	<=	B_right &&
			A_bottom >=	B_top &&
			A_top <=	B_bottom )	// Check if A is inside of B
		{
			// Half-width difference
			dx = A_hx_pos - B_hx_pos;

			// Get how much x is penetrating
			px = (A_hx+B_hx)-abs(dx);

			if( px > 0 )
			{
				dy = A_hy_pos - B_hy_pos;
				// how much is y penetrating?
				py = (A_hy+B_hy)-abs(dy);

				if( py > 0 ){
					// does x have less penetration?
					if( px < py ){
						// move left
						if( dx < 0 )
							result.x = -px;
						else
							result.x = px;
					}
					else{ // more y penetration
						if( dy < 0 )
							result.y = -py;
						else
							result.y = py;
					}
				}
			}

		}

	}
	*
}

/*
void CCollider_AABB::CollideWithAABB(CCollider_AABB *pTarget, Vector2& result)
{
		Vector2 pos		= m_pParent->getPos();
		float A_left	= pos.x - m_hx;
		float A_right	= pos.x + m_hx;
		float A_top		= pos.y - m_hy;
		float A_bottom	= pos.y + m_hy;
		float A_hx		= m_hx;			// x half width
		float A_hy		= m_hy;			// y half width
		float A_hx_pos  = pos.x; 
		float A_hy_pos  = pos.y;		// half width positions

		pos				= pTarget->GetParent()->getPos();
		float B_left	= pos.x - pTarget->m_hx;
		float B_right	= pos.x + pTarget->m_hx;
		float B_top		= pos.y - pTarget->m_hy;
		float B_bottom	= pos.y + pTarget->m_hy;
		float B_hx		= pTarget->m_hx;
		float B_hy		= pTarget->m_hy;
		float B_hx_pos	= pTarget->GetParent()->getPos().x;
		float B_hy_pos	= pTarget->GetParent()->getPos().y;
				
		float dx, dy, px, py; // differences

		result.x = 0;
		result.y = 0;


		if( A_right	>=	B_left &&
			A_left	<=	B_right &&
			A_bottom >=	B_top &&
			A_top <=	B_bottom )	// Check if A is inside of B
		{
			// Half-width difference
			dx = A_hx_pos - B_hx_pos;

			// Get how much x is penetrating
			px = (A_hx+B_hx)-abs(dx);

			if( px > 0 )
			{
				dy = A_hy_pos - B_hy_pos;
				// how much is y penetrating?
				py = (A_hy+B_hy)-abs(dy);

				if( py > 0 ){
					// does x have less penetration?
					if( px < py ){
						// move left
						if( dx < 0 )
							result.x = -px;
						else
							result.x = px;
					}
					else{ // more y penetration
						if( dy < 0 )
							result.y = -py;
						else
							result.y = py;
					}
				}
			}

		}
}

//********************
// Circle vs AABB
//********************
void CollideWithAABB( CCollider_AABB* pTarget, Vector2& v ){

    V A_pos = m_pParent->getPos();
	V B_pos = m_pTarget->m_pParent->getPos();


/*
    if( center.x < min.x ) closestPoint.x = min.x;
    else if( center.x > max.x ) closestPoint.x = max.x;
    if( center.y < min.y ) closestPoint.y = min.y;
    else if( center.y > max.y ) closestPoint.y = max.y;
    
    var diff:Point = closestPoint.subtract( center );
    if( diff.x * diff.x + diff.y * diff.y > radius * radius ) return null;
    
    return closestPoint;
	*
}

//********************
// Circle vs Circle
//********************
void CollideWithCircle( CCollider_Circle* pTarget, Vector2& v ){
	
}
*



void ICollider::AddHitBox(ICollider *col){
	m_hitBoxList.push_back(col);
}


*/