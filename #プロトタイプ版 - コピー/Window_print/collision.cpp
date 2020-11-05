/*=======================================================================================

	 collision”»’è[collision.cpp]
												author:mgc
												date:2020/07/23
 -----------------------------------------------------------------------------------------

 =========================================================================================*/

#include "collision.h"
#include <d3dx9.h>


bool Collision_CircleAndCircleHit(const CollisionCircle* pA, const CollisionCircle* pB)
{
	D3DXVECTOR2 vec = pA->center - pB->center;

	return D3DXVec2LengthSq(&vec) < (pA->radius + pB->radius) * (pA->radius + pB->radius);
}
