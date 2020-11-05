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
#include"keylogger.h"

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
	Stack prev_stack = GetstackPrev();

	//仮想プレイヤーのpositionを更新する
	//プレイヤー前のフレームのpositionデータをスタック領域から取得する
	for (int i = 0; i < prev_stack.recordFrame; i++)
	{
		if (Keylogger_Press(KL_B))
		{
			//Bキーを押し続けたら、プレイヤー前のフレームのpositionデータを1フレームずつ、仮想プレイヤーに代入する
			//Bキーを離したら、仮想プレイヤーがその場で止まる
			g_VPlayer.position = prev_stack.position_Data[i];
		}

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
