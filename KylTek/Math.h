#ifndef EXPLODER_MATH_H
#define EXPLODER_MATH_H

// Maths

#include "Main.h" 

#define PI		(3.141592653589793f)
#define DegreeToRadian( degree ) ((degree) * (PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / PI))

inline float Vec2Length( Vector2 v );
inline float Vec2SqLength( Vector2 v );
inline float Vec2Normalize( Vector2 v );
inline float Vec2DotProduct( Vector2 a, Vector2 b );
inline Vector2 Vec2Project(Vector2 a, Vector2 b,Vector2 c, Vector2 d);
inline Vector2 Vec2Project(Vector2 v1, Vector2 v2);

#endif