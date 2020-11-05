/*=====================================================================================================

	キー入力の記録制御[keylogger.h]
																author:mgc
																date:2020/07/29
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#ifndef KEYLOGGER_H
#define KEYLOGGER_H


//キーロガーキー列挙型
typedef enum KeyloggerKey_tag
{
	KL_UP,
	KL_DOWN,
	KL_LEFT,
	KL_RIGHT,
	KL_JUMP,
	KL_B,
	KL_J,
	KL_R,
	KL_MAX

}KeyloggerKey;

//キーロガーモジュールの初期化
void Keylogger_Initialize(void);

//キーロガーモジュールの終了処理
void Keylogger_Finalize(void);

//キーロガーモジュールの更新処理
void Keylogger_Update(void);


//キー入力状態の取得
//
//引数：
//
//戻り値：押されていたらtrue
//
bool Keylogger_Press(KeyloggerKey kl);


//キー入力状態の取得(押した瞬間)
//
//引数：
//
//戻り値：押した瞬間だったらtrue
//
bool Keylogger_Trigger(KeyloggerKey kl);


//キー入力状態の取得(離した瞬間)
//
//引数：
//
//戻り値：離した瞬間だったらtrue
//
bool Keylogger_Release(KeyloggerKey kl);



void Keylogger_RecordStart(int frame_max);
void Keylogger_RecordEnd(void);
void Keylogger_RecordLoad(void);
void Keylogger_RecordPlay(void);
bool Keylogger_IsRecordPlay(void);
int GetMode(void);

#endif // !KEYLOGGER_H
