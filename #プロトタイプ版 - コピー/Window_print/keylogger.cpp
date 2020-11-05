/*=====================================================================================================

	キー入力の記録制御[keylogger.cpp]
																author:mgc
																date:2020/07/29
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#include "keylogger.h"
#include"keyboard.h"

/*-----------------------------------------------------------------------------------------
 グローバル変数
------------------------------------------------------------------------------------------*/
typedef unsigned short KeyloggerType;
static KeyloggerType g_CurrentKeyState = 0; //そのフレームでのキーの状態保持用
static KeyloggerType g_PrevKeyState = 0;	//前フレームでのキーの状態保持用
static KeyloggerType g_TriggerKeyState = 0; //トリガー状態の確認用
static KeyloggerType g_ReleaseKeyState = 0;  //リリース状態の確認用

static int g_KeyloggerMode = 0; // 0 - 通常  1 - 記録  2 - 再生  3 - 逆再生
static KeyloggerType* g_pRecordCurrentData = NULL;
static int g_RecordFrame = 0;
static int g_RecordPlayFrame = 0;
static int g_RecordFrameMax = 0;

/*-----------------------------------------------------------------------------------------
   関数定義
 -------------------------------------------------------------------------------------------*/
 //キーロガーキーからキーボードキーへの変換表
static const Keyboard_Keys g_KKs[KL_MAX] = {
	KK_W,
	KK_S,
	KK_A,
	KK_D,
	KK_SPACE,
	KK_B,
	KK_J,
	KK_R
};


//キーロガーモジュールの初期化
void Keylogger_Initialize(void)
{
	g_CurrentKeyState = 0;
	g_PrevKeyState = 0;
	g_TriggerKeyState = 0;
	g_ReleaseKeyState = 0;

	g_KeyloggerMode = 0;
	g_pRecordCurrentData = NULL;
	g_RecordFrame = 0;
	g_RecordPlayFrame = 0;
	g_RecordFrameMax = 0;

}

//キーロガーモジュールの終了処理
void Keylogger_Finalize(void)
{
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
		g_pRecordCurrentData = NULL;
	}
}

//キーロガーモジュールの更新処理
void Keylogger_Update(void)
{
	//前フレームのキー状態を保存しておく
	g_PrevKeyState = g_CurrentKeyState;
	g_CurrentKeyState = 0;

	//キーロガーモードによって動作変更
	if (g_KeyloggerMode <= 1)
	{
		g_CurrentKeyState = 0;

		//キーロガーキーの状態取得
		for (int i = 0; i < KL_MAX; i++)
		{
			if (Keyboard_IsKeyDown(g_KKs[i]))
			{
				g_CurrentKeyState |= 1u << i;
			}
		}

		//キー記録モード
		if (g_KeyloggerMode == 1)
		{
			if (g_RecordFrame < g_RecordFrameMax)
			{
				g_pRecordCurrentData[g_RecordFrame++] = g_CurrentKeyState;
			}
		}
	}

	if (g_KeyloggerMode == 2)
	{
		//再生モード

		//安全処置(記録フレーム数よりも多く再生しようとした)
		if (g_RecordPlayFrame < g_RecordFrame)
		{
			g_CurrentKeyState = g_pRecordCurrentData[g_RecordPlayFrame++];
		}
		else
		{
			g_CurrentKeyState = 0;
			g_KeyloggerMode = 0;
		}
		//g_CurrentKeyState = g_RecordPlayFrame < g_RecordFrame ? g_pRecordCurrentData[g_RecordPlayFrame++] : 0;
	}

	if(g_KeyloggerMode==3)
	{
	}

	g_TriggerKeyState = (g_PrevKeyState ^ g_CurrentKeyState) & g_CurrentKeyState;
	g_ReleaseKeyState = (g_PrevKeyState ^ g_CurrentKeyState) & g_PrevKeyState;
}


//キー入力状態の取得
//
//引数：
//
//戻り値：押されていたらtrue
//
bool Keylogger_Press(KeyloggerKey kl)
{
	return g_CurrentKeyState & (1u << (int)kl);
}


//キー入力状態の取得(押した瞬間)
//
//引数：
//
//戻り値：押した瞬間だったらtrue
//
bool Keylogger_Trigger(KeyloggerKey kl)
{
	return g_TriggerKeyState & (1u << (int)kl);
}


//キー入力状態の取得(離した瞬間)
//
//引数：
//
//戻り値：離した瞬間だったらtrue
//
bool Keylogger_Release(KeyloggerKey kl)
{
	return g_ReleaseKeyState & (1u << (int)kl);
}


void Keylogger_RecordStart(int frame_max)
{
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
	}

	g_pRecordCurrentData = (KeyloggerType*)malloc(sizeof(KeyloggerType) * frame_max);
	g_KeyloggerMode = 1;
	g_RecordFrame = 0;
	g_RecordFrameMax = frame_max;
}

void Keylogger_RecordEnd(void)
{
	if (!g_pRecordCurrentData)
	{
		return;
	}
	FILE* fp = NULL;
	fp = fopen("keylogger.dat", "wb");
	//バイナリの先頭にデータ量を記録
	fwrite(&g_RecordFrame, sizeof(g_RecordFrame), 1, fp);
	//ファイルへデータを保存
	fwrite(g_pRecordCurrentData, sizeof(KeyloggerType), g_RecordFrame, fp);
	fclose(fp);

	g_KeyloggerMode = 0; //普通モード
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
		g_pRecordCurrentData = NULL;
	}
}

void Keylogger_RecordLoad(void)
{
	FILE* fp = NULL;
	fp = fopen("keylogger.dat", "rb");

	fread(&g_RecordFrame, sizeof(g_RecordFrame), 1, fp);
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
	}

	g_pRecordCurrentData = (KeyloggerType*)malloc(sizeof(KeyloggerType) * g_RecordFrame);

	fread(g_pRecordCurrentData, sizeof(KeyloggerType), g_RecordFrame, fp);
	fclose(fp);

}

void Keylogger_RecordPlay(void)
{
	g_RecordPlayFrame = 0;
	g_KeyloggerMode = 2; //再生モード
}

bool Keylogger_IsRecordPlay(void)
{
	if (g_KeyloggerMode == 2)
	{
		return true;
	}


	return false;
}

int GetMode(void)
{
	return g_KeyloggerMode;
}

