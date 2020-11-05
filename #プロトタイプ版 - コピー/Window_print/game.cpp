/*=====================================================================================================

	ゲームの制御[game.cpp]
																author:mgc
																date:2020/06/24
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#include"debug_font.h"
#include"game_player.h"
#include"texture.h"
#include"keylogger.h"
#include"config.h"
#include"game_effect.h"
#include"game_virtual_player.h"

//ゲームの初期化
void Game_Initialize(void)
{

	GamePlayer_Initialize();
	VPlayer_Initialize();
	GameEffect_Initialize();

	//テクスチャの読み込み
	if (Texture_Load() > 0)
	{
		//デバイスの取得に失敗
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);

	}
	//Keylogger_RecordStart(600);
	//Keylogger_RecordLoad();
	//Keylogger_RecordPlay();

	//PlaySound(SOUND_LABEL_BGM000);
}

//ゲームの更新処理
void Game_Update(void)
{
	if (Keylogger_Press(KL_UP))
	{
		
	}
	if (Keylogger_Press(KL_DOWN))
	{
		
	}
	if (Keylogger_Press(KL_LEFT))
	{
		GamePlayer_MoveLeft();
	}
	if (Keylogger_Press(KL_RIGHT))
	{
		GamePlayer_MoveRight();
	} 

	if (Keylogger_Press(KL_JUMP))
	{
		GamePlayer_Jump();
	}

	if (Keylogger_Trigger(KL_R))
	{
			
	}

	GamePlayer_Update();
	GameEffect_Update();
	VPlayer_Update();
}

//ゲームの描画処理
void Game_Draw(void)
{
	GameEffect_Draw();
	GamePlayer_Draw();
	VPlayer_Draw();
}

//ゲームの終了処理
void Game_Finalize(void)
{
	GamePlayer_Finalize();
	VPlayer_Finalize();
}
