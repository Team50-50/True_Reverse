/*=====================================================================================================

	デバックフォントモジュール[debug_font.cpp]
																author:mgc
																date:2020/07/03
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#if defined (_DEBUG) || defined(DEBUG)

#include"texture.h"
#include"sprite.h"
#include<string.h>

#define DEBUG_FONT_WIDTH  (32)
#define DEBUG_FONT_HEIGHT (64)
#define DEBUG_FONT_START_CHAR_COOD (32)
#define DEBUG_FONT_ONE_LINE_CHAR_COUNT (16)

static int g_TextureId = TEXTURE_INVALID_ID;

//デバックフォントモジュールの初期化
void DebugFont_Initialize(void)
{
	g_TextureId = Texture_SetTextureLoadFile("asset/debug_font_32x64.png");

	//テクスチャの読み込み
	if (Texture_Load() > 0)
	{
		//デバイスの取得に失敗
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);

	}
}

//デバックフォントモジュールの終了処理
void DebugFont_Finalize(void)
{
	Texture_Release(&g_TextureId, 1);
}

//デバックフォントの表示
//
//引数：dx,dy    ...デバック文字列の描画座標
//		pString  ...デバック文字列へのポインタ
//
void DebugFont_Draw(float dx, float dy, const char* pString)
{
	int a = strlen(pString);

	for (int i = 0; i < a; i++)
	{
		int offset = pString[i] - ' ';
		int tcx = offset % DEBUG_FONT_ONE_LINE_CHAR_COUNT * DEBUG_FONT_WIDTH;
		int tcy = offset / DEBUG_FONT_ONE_LINE_CHAR_COUNT * DEBUG_FONT_HEIGHT;

		Sprite_Draw(g_TextureId, dx + i * DEBUG_FONT_WIDTH, dy,tcx, tcy, DEBUG_FONT_WIDTH, DEBUG_FONT_HEIGHT);
	}
	
}
#endif