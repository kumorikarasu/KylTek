#include <set>
#include "Main.h"
#include "CollisionManager.h"
#include "QuadTree.h"
#include "Foreach.h"
#include "CGlobal.h"
#include "Math.h"

// -------------------------------------------------
//   ICol
// -------------------------------------------------
ICol::ICol(CEntity* pParent, int collisionType) 
		: m_pParent(pParent), Type(collisionType), m_collisionFlags(0)	
	{
		QT = NULL;
		min *= 0;
		max *= 0;
		m_xoffset *= 0;
		m_yoffset *= 0;
		isDead=false;
		Collision = &CEntity::onCollision;
	}

ICol::~ICol(){

}

CEntity* ICol::Parent(){return m_pParent;}
void ICol::Parent(CEntity* _ent){m_pParent = _ent;}

void ICol::DebugDraw(CGraphicsHandler *g){
	if(Debug->CheckFlag(CDebug::HitBoxes)){
		if (GetType() == AABB)
			g->DrawRect(min + (max - min) / 2,(int) (max.x - min.x),(int) (max.y - min.y), D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(0,0,255,255));
		if (GetType() == Circle)
			g->DrawOval(min + (max - min) / 2,(int) (max.x - min.x),(int) (max.y - min.y), D3DCOLOR_RGBA(255,0,0,0), 1, D3DCOLOR_RGBA(0,255,255,255));
	}
}

inline void ICol::RemoveQT(){
	Global->pColMan->getQT()->RemoveEntity(this);
	/*
	FOREACH(ICol*,QT->Entlist,c){
		if (this == (*c)){
			QT->Entlist.erase(c);
			break;
		}
	}
	*/
}

void ICol::UpdatePos(){
	if (Type!=Passive)
		RemoveQT();

	//Update X and Y Position
	min.x=m_pParent->getPos().x-m_hx+m_xoffset;
	min.y=m_pParent->getPos().y-m_hy+m_yoffset;
	max.x=min.x+m_w;
	max.y=min.y+m_h;
	
	if (Type!=Passive)
		QT = Global->pColMan->getQT()->AddEntity(this);
	
}

void ICol::SetSize(Vector2 size)
{
	m_h = size.y;
	m_w = size.x;

	m_hx = size.x/2;
	m_hy = size.y/2;
}

// -------------------------------------------------
//   ICol_AABB
// -------------------------------------------------
Vector2 ICol_AABB::Collide(ICol* pTarget)
{
	Vector2 v(0.0f,0.0f);

	if (pTarget == NULL || pTarget->isDead) return v;

	switch(pTarget->GetType())
	{
	case AABB:
		CollideAABBvsAABB(this, (ICol_AABB*) pTarget, v);
		break;
	case Circle:
		CollideCirclevsAABB((ICol_Circle*) pTarget,this, v);
		v*=-1;
		break;
	case Convex:
		this->m_pParent->addAccel(Vector2(1,1));
	break;
	}

	return v;
}


ICol_AABB::ICol_AABB(CEntity *pParent,bool bInheritSize) : ICol(pParent) 
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


void ICol_AABB::SetSize(Vector2 size)
{
	m_h = size.y;
	m_w = size.x;

	m_hx = size.x/2;
	m_hy = size.y/2;
}

// -------------------------------------------------
//   ICol_Circle
// -------------------------------------------------
Vector2 ICol_Circle::Collide(ICol* pTarget)
{
	Vector2 v(0.0f,0.0f);
	switch(pTarget->GetType())
	{
	case AABB:
		CollideCirclevsAABB(this, (ICol_AABB*) pTarget, v);
		break;
	case Circle:
		CollideCirclevsCircle(this, (ICol_Circle*) pTarget, v);
		break;
	}	
	return v;
}


ICol_Circle::ICol_Circle(CEntity *pParent,bool bInheritSize) : ICol(pParent) 
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



void ICol_Circle::SetSize(Vector2 size)
{
	m_h = size.x;
	m_w = size.x;

	m_hx = size.x/2;
	m_hy = size.x/2;

	m_r = size.x/2;
}


// -------------------------------------------------
//   ICol_Convex
// -------------------------------------------------
Vector2 ICol_Convex::Collide(ICol* pTarget)
{
	Vector2 v(0.0f,0.0f);

	switch(pTarget->GetType())
	{
	case AABB:
	//	CollideConvexvsAABB(this, (ICol_AABB*) pTarget, v);
		break;
	case Circle:
	//	CollideConvexvsCircle(this, (ICol_Circle*) pTarget, v);
		break;
	case Convex:
		CollideConvexvsConvex(this, (ICol_Convex*) pTarget, v);
	break;
	}	

	return v;
}


ICol_Convex::ICol_Convex(CEntity *pParent,bool bInheritSize) : ICol(pParent) 
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



void ICol_Convex::SetSize(Vector2 size)
{
	m_h = size.x;
	m_w = size.x;
}

void ICol_Convex::UpdatePos(){

	CConvex* a;
	a = (CConvex*)m_pParent;
	a->CalcMinMax();
	min = a->m_min;
	max = a->m_max;

	m_w = max.x - min.x;
	m_h = max.y - min.y;

	if (Type!=Passive){
		RemoveQT();
		QT = Global->pColMan->getQT()->AddEntity(this);
	}
}

// -------------------------------------------------
//   Collision Manager
// -------------------------------------------------

ColManager::ColManager(){
	ICollist = new LinkedList<ICol>();
	QT = new QuadTree();
	QT->setType(STEM);
	QT->CalcQuadTree(Vector2(0, 0),Vector2(Global->LEVEL_WIDTH,Global->LEVEL_HEIGHT));
}

ColManager::~ColManager(){
	ICollist->clear();
	SAFE_DELETE(ICollist);
}

void ColManager::AddICol(ICol *_icol){
	ICollist->add(_icol);
}

void ColManager::DoCollisions(){
	Vector2 result;
	result *= 0;
	Node<ICol>* pIColIt = ICollist->first();
	ICol* pICol;
	static char buffer[256];

	while(pIColIt!=NULL){
		pICol = pIColIt->object;
		
		if (pICol != NULL && pICol->isDead){
			pIColIt = ICollist->remove(pIColIt);
			QT->RemoveEntity(pICol);
			SAFE_DELETE(pICol);
			pICol = pIColIt->object;
		}
		else
		if (pICol != NULL){
			if (pICol->Type!=Static){
				//std::vector<ICol*> Collider;
				std::set<ICol*> Collider;
				Global->count=0;

				if (pICol->m_pParent->getObjectID()==1000){
				QT->GetEntinCell(pICol,&Collider);

				sprintf(buffer, "Count in this frame: %d", Collider.size());
				Debug->ToConsole(std::wstring(buffer));

				}else
					QT->GetEntinCell(pICol,&Collider);

				FOREACHS(ICol*,Collider,c){

					if ((*c)->isDead == true || (*c)->Type==NoCollide || pICol == (*c) || pICol->m_pParent->m_pParent == (*c)->m_pParent)
						continue;

					
					result = pICol->Collide((*c));
					//ColCheck(pICol,(*c));
					
					//TODO: 1/2 Check on return
					if (result.x!=0 || result.y!=0){
						if ((*c)->GetCollisionType() == Dynamic){
							result /= 2;
							(*c)->Parent()->onCollision(-result,pICol->Parent());
							if( !((*c)->GetCollisionFlags() & NoPush) )
							(*c)->Parent()->addPos(-result);
						}

						((pICol->m_pParent)->*(pICol->Collision)) (result,(*c)->Parent()); //Collision Function
						//TODO: FIX LOL K
						//pICol->Parent()->onCollision(result,(*c)->Parent());

						if( !(pICol->GetCollisionFlags() & NoPush) )
							pICol->Parent()->addPos(result);
					}
				}
			}
			
		}
		pIColIt = pIColIt -> next;
		
	}
}

void ColManager::Clear(){
	ICollist->clear();
	ICollist->Init();
	delete QT;
}

void ColManager::Init(Vector2& Size){
	QT = new QuadTree();
	QT->CalcQuadTree(Vector2(0, 0),Vector2(Size.x,Size.y));
}

void DrawNodes(CGraphicsHandler *g, QuadTree* QT){
	if (QT!=NULL){
		if (QT->getType() == STEM){
			for (int a=0;a<4;a++)
				DrawNodes(g,QT->children[a]);
		}else{
			for (int a=0;a<4;a++)
				QT->DebugDraw(g);
		}
	}
}

void ColManager::DebugDraw(CGraphicsHandler *g){
	
	if(Debug->CheckFlag(CDebug::QuadCells)){
		DrawNodes(g,QT);
	}
	if(Debug->CheckFlag(CDebug::HitBoxes)){
		Node<ICol>* pIColIt = ICollist->first();
		ICol* pICol;
		while(pIColIt!=NULL){
			pICol = pIColIt->object;
			if(pICol){
				pICol->DebugDraw(g);
			}
			pIColIt = pIColIt->next;
		}
	}
	if(Debug->CheckFlag(CDebug::ColLines)){
		Node<ICol>* pIColIt = ICollist->first();
		ICol* pICol;

		while(pIColIt!=NULL){
			pICol = pIColIt->object;
			if(pICol){
				if (pICol->Type!=Static){
					std::set<ICol*> Collider;
					QT->GetEntinCell(pICol,&Collider);
					FOREACHS(ICol*,Collider,c){
						Vector2 result = pICol->Collide((*c));
						if (result.x!=0 || result.y!=0)
							g->DrawLine(Vector2(pICol->min.x+pICol->m_hx,pICol->min.y+pICol->m_hy),Vector2((*c)->min.x+(*c)->m_hx,(*c)->min.y+(*c)->m_hy),D3DCOLOR_RGBA(0,0,255,255));
					}
				}
			}
			pIColIt = pIColIt->next;
		}
	}
}

//********************
// AABB vs AABB
//********************

// Let's try to make this readable this time...

void CollideAABBvsAABB(ICol_AABB* a, ICol_AABB* b, Vector2& result){

		Vector2 pos		= a->Parent()->getPos();
		float A_left	= pos.x - a->m_hx;
		float A_right	= pos.x + a->m_hx;
		float A_top		= pos.y - a->m_hy;
		float A_bottom	= pos.y + a->m_hy;
		float A_hx		= a->m_hx;			// x half width
		float A_hy		= a->m_hy;			// y half width
		float A_hx_pos  = pos.x; 
		float A_hy_pos  = pos.y;		// half width positions

		pos				= b->Parent()->getPos();
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
		
}

//********************
// Circle vs AABB
//********************

void CollideCirclevsAABB(ICol_Circle* a, ICol_AABB* b, Vector2& result){

	result.x = 0;
	result.y = 0;

	Vector2 ColCornerPoint = Vector2(0, 0);

	Vector2 pos		= b->Parent()->getPos();
	float B_left	= pos.x - b->m_hx;
	float B_right	= pos.x + b->m_hx;
	float B_top		= pos.y - b->m_hy;
	float B_bottom	= pos.y + b->m_hy;
	float B_hx		= b->m_hx;
	float B_hy		= b->m_hy;
	float B_hx_pos	= pos.x;
	float B_hy_pos	= pos.y;

	pos				= a->Parent()->getPos();
	float radius	= a->m_r;
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
	
}

//********************
// Circle vS C
//********************

void CollideCirclevsCircle(ICol_Circle* a, ICol_Circle* b, Vector2& result){

	Vector2 posa	= a->Parent()->getPos();
	Vector2 posb    = b->Parent()->getPos();

	float dx,dy; 
	dx = posa.x - posb.x; //Find Delta X
	dy = posa.y - posb.y; //Find Delta Y

	float distance = abs(dx*dx) + abs(dy*dy); //Find Square Distance
	float radius = a->m_r*a->m_r + b->m_r*b->m_r; //Find Square radii (because its alot faster then sqrt the distance)

	if (distance<=radius){
			float dir = atan2(dy,dx);
			
			distance = radius-distance; //Find the penetration amount
			distance = sqrt(distance); //Find the exact penetration length

			result.x = cos(dir) * distance; //Find resultant vectors
			result.y = sin(dir) * distance; //Find resultant vectors

			result /= 6;
	}
}


//********************
// Convex vs Convex
//********************

//Fucking Complicated!!!
void CollideConvexvsConvex(ICol_Convex* a, ICol_Convex* b, Vector2& result){

	
	//TODO: some kind of BSP tree check (or something similer)
	//TODO: AABB Check

	CConvex*	aParent;
	CConvex*	bParent;
	aParent =	(CConvex*)a->m_pParent;
	bParent	=	(CConvex*)a->m_pParent;
	Vector4*	conv;
	conv = NULL;
	Vector2		conversion,axis;
	Vector2		VertexA, VertexB;
	Vector2		minproj, maxproj;

	float		dotmin, dotmax;
	
	
	for (int a=0;a<(aParent->Vertexes.size());a++){
		
		//Find the Seperating Axis
		if (a != aParent->Vertexes.size())
			conv = aParent->Vertexes[a+1];
		else
			conv = aParent->Vertexes[0];
		VertexA = Vector2(conv->x,conv->y);
		conv = aParent->Vertexes[a];
		VertexB = Vector2(conv->x,conv->y);
		axis = VertexA - VertexB;
		
		//Project All Other Points onto that Axis
		FOREACH(Vector4*,bParent->Vertexes, c){
			conv	= (*c);
			conversion = Vector2(conv->x,conv->y);
			minproj = ((conversion.x*axis.x + conversion.y*axis.y) / 
				( axis.x*axis.x + axis.y*axis.y))
				* axis;
			maxproj = minproj;

		}
	}
	
	/*
	FOREACH(Vector4*,aParent->Vertexes, c){
		if ((*c) == aParent->Vertexes[aParent->Vertexes.size-1]){

		}else{
			axis = (*c) - (*(++c));
		}
	}*/
	
	//Loop, Each Seperating Axix

	//Loop, Project Each Vector onto Axis

	//


}

//void CollideAABBvsConvex(ICol_Convex* a, ICol_Convex* b, Vector2& result)
//void CollideCirclevsConvex(ICol_Convex* a, ICol_Convex* b, Vector2& result)