/*=======================================================================================

	 collision”»’è[collision.h]
												author:mgc
												date:2020/07/23
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include <d3dx9.h>

//‰~‚Ìcollision
typedef struct CollisionCircle_tag
{
	D3DXVECTOR2 center;
	float radius;

}CollisionCircle;

bool Collision_CircleAndCircleHit(const CollisionCircle* pA, const CollisionCircle* pB);


#endif