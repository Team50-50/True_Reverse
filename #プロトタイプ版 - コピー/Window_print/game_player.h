/*=======================================================================================

	 ゲームのプレイヤー制御[game_player.h]
												author:mgc
												date:2020/07/17
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include"collision.h"
#define FRAME_MAX (3600)

 /*------------------------------------------------------------------------------------------
	構造体宣言
 -------------------------------------------------------------------------------------------*/
//プレイヤー構造体
typedef struct Player_tag
{
	D3DXVECTOR2 position;
	D3DXVECTOR2 prevposition;
	D3DXVECTOR2 direction;
	D3DXVECTOR2 speed;
	bool  isJump;

}Player;

//キュー構造体
typedef struct
{
	D3DXVECTOR2 position_Data[FRAME_MAX];
	
	int recordFrame_head;
	int recordFrame_tail;
}Queue;

//スタック構造体
typedef struct
{
	D3DXVECTOR2 position_Data[FRAME_MAX];
	int playFrame;
	int recordFrame;
}Stack;

//ゲームプレイヤーの初期化
void GamePlayer_Initialize(void);

//ゲームプレイヤーの終了処理
void GamePlayer_Finalize(void);

//ゲームプレイヤーの更新処理
void GamePlayer_Update(void);

//ゲームプレイヤーの描画処理
void GamePlayer_Draw(void);

//ゲームプレイヤーの移動処理
void GamePlayer_MoveLeft(void);
void GamePlayer_MoveRight(void);
void GamePlayer_Jump(void);


//プレイヤーのCollisionの取得
CollisionCircle GamePlayer_GetCollision(void);

Player GetPlayer(void);

//エンキュー...positionデータをキューに追加する、毎回最後尾にデータを追加する。
void enqueue(Queue* q, D3DXVECTOR2 playerPosition);
//デキュー...positionデータをキューから取り出す、毎回先頭のデータ(一番古いのpositionデータ)を取り出す。
D3DXVECTOR2 dequeue(Queue* q);

Queue GetqCurrent(void);
Queue GetqPrev(void);

//PUSH...positionデータをスタックに追加する
void push(Stack* stack, D3DXVECTOR2 playerPosition);
//POP...positionデータをスタックから取り出す、毎回最後 PUSH した positionデータからを取り出す
D3DXVECTOR2 pop(Stack* stack);

Stack GetstackPrev(void);
Stack GetstackCurrent(void);

#endif //GAME_PLAYER_H