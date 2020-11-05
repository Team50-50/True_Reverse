/*=====================================================================================================

	ÉQÅ[ÉÄÇÃãLò^êßå‰[game_virtual_player.h]
																author:mgc
																date:2020/11/01
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#ifndef GAME_VIRTUAL_PLAYER_H_
#define GAME_VIRTUAL_PLAYER_H_

#include<d3dx9.h>
#include"collision.h"
#include"game_player.h"
#define VPLAYER_MAX (3)

typedef struct Virtual_player_tag
{
	D3DXVECTOR2 position;
	int frame;

}VPlayer;



void VPlayer_Initialize(void);


void VPlayer_Finalize(void);


void VPlayer_Update(void);


void VPlayer_Draw(void);

VPlayer GetVPlayer(void);



#endif // !GAME_VIRTUAL_PLAYER_H_


