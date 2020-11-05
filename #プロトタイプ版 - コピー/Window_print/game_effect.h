/*=======================================================================================

	 ゲームのエフェクト制御[game_effect.h]
												author:mgc
												date:2020/09/10
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef GAME_EFFECT_H_
#define GAME_EFFECT_H_


//エフェクトの初期化
void GameEffect_Initialize(void);

//エフェクトの終了処理
void GameEffect_Finalize(void);

//エフェクトの更新処理
void GameEffect_Update(void);

//エフェクトの描画処理
void GameEffect_Draw(void);

//エフェクトの生成
void GameEffect_Spawn(float x, float y);


#endif // !GAME_EFFECT_H_


