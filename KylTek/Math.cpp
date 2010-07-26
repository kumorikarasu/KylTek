#include "Math.h"
#include <math.h>



inline float Vec2Length( Vector2 v )
{
	return sqrtf( v.x*v.x + v.y*v.y );
}

inline float Vec2SqLength( Vector2 v ){
	return  v.x*v.x + v.y*v.y;
}

// Sets the vectors length to 1, creating a unit vector
// This is accomplished by dividing the vector by its length
inline float Vec2Normalize( Vector2 v )
{
	float len = Vec2Length(v);

	if( len != 0.0f )
		v /= len;
	else
		v.x = v.y = 0.0f;

	return len;
}



inline float Vec2DotProduct(Vector2 a, Vector2 b )
{
	return( a.x*b.x + a.y*b.y ); 
}

//Projects the first vector onto the second vector, and returns the resultant point
inline Vector2 Vec2Project(Vector2 a, Vector2 b,Vector2 c, Vector2 d){
	Vector2 v1,v2;
	v1 = b-a;
	v2 = d-c;

	return (Vec2DotProduct(v1,v2)/Vec2SqLength(v2))*v2;
}

//Projects the first vector onto the second vector, and returns the resultant point
inline Vector2 Vec2Project(Vector2 v1, Vector2 v2){
	return (Vec2DotProduct(v1,v2)/Vec2SqLength(v2))*v2;
}