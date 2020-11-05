/*=====================================================================================================

	逆行システム制御[game_virtual_player.cpp]
																author:mgc
																date:2020/11/01
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#include <stdio.h>
#include"game_virtual_player.h"
#include"game_player.h"
#include"texture.h"
#include"sprite.h"

/*--------------------------------------------------------------------------------------
   定数宣言
  ----------------------------------------------------------------------------------------*/
#define VPLAYER_WIDTH  (80)
#define VPLAYER_HEIGHT (80)

/*-----------------------------------------------------------------------------------------
  グローバル変数
 -------------------------------------------------------------------------------------------*/
static int g_TextureVPlayer = TEXTURE_INVALID_ID;
//VPlayer g_VPlayer[VPLAYER_MAX];
VPlayer g_VPlayer;



void VPlayer_Initialize(void)
{
	g_TextureVPlayer = Texture_SetTextureLoadFile("asset/player.tga");

}


void VPlayer_Finalize(void)
{
	Texture_Release(&g_TextureVPlayer, 1);
}


void VPlayer_Update(void)
{
	//Queue q = GetqPrev();
	Stack pstack = GetstackPrev();
	Stack cstack = GetstackCurrent();

	for (int i = 0; i < pstack.recordFrame; i++)
	{
		g_VPlayer.position = pstack.position_Data[i];

	}
	
	
}


void VPlayer_Draw(void)
{
	
	Sprite_SetColor(D3DCOLOR_RGBA(255, 0, 255, 255));

	Sprite_Draw(g_TextureVPlayer, g_VPlayer.position.x, g_VPlayer.position.y,
		VPLAYER_WIDTH, VPLAYER_HEIGHT, 0, 0, 270, 270);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}


VPlayer GetVPlayer(void)
{
	return g_VPlayer;
}
