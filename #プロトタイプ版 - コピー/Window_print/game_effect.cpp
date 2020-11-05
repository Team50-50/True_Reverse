/*=======================================================================================

	 ゲームのエフェクト制御[game_effect.cpp]
												author:mgc
												date:2020/09/10
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include"texture.h"
#include"sprite.h"
#include<d3dx9.h>
#include"mydirect3d.h"

/*------------------------------------------------------------------------------------------
   定数宣言
-------------------------------------------------------------------------------------------*/
#define EFFECT_MAX (2048)

/*------------------------------------------------------------------------------------------
   構造体宣言
-------------------------------------------------------------------------------------------*/
//エフェクト構造体
typedef struct Effect_tag
{
	D3DXVECTOR2 position;
	//色
	//サイズ
	//透明度
	//種類
	//寿命フレーム数
	int birthFrame;
	bool enable;
}Effect;

/*------------------------------------------------------------------------------------------
   グローバル変数
-------------------------------------------------------------------------------------------*/
static int g_TextureId = TEXTURE_INVALID_ID;
static Effect g_Effects[EFFECT_MAX];
static int g_FrameCount = 0;


/*------------------------------------------------------------------------------------------
   関数定義
-------------------------------------------------------------------------------------------*/
 //エフェクトの初期化
void GameEffect_Initialize(void)
{
	g_TextureId = Texture_SetTextureLoadFile("asset/effect000.jpg");

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effects[i].enable = false;
	}

	g_FrameCount = 0;
}

//エフェクトの終了処理
void GameEffect_Finalize(void)
{
	Texture_Release(&g_TextureId, 1);
}

//エフェクトの更新処理
void GameEffect_Update(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (!g_Effects[i].enable) continue;

		int age = g_FrameCount - g_Effects[i].birthFrame;

		if (age >= 60)
		{
			g_Effects[i].enable = false;
		}

	}

	g_FrameCount++;
}

//エフェクトの描画処理
void GameEffect_Draw(void)
{
	//加算合成の設定
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);      //ブレンディング処理(デフォルト)
	//g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			 //αデスティネーションカラーの指定(1.0)

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (!g_Effects[i].enable) continue;

		int age = g_FrameCount - g_Effects[i].birthFrame;

		float a = 1.0f - ((1.0f / 60.0f) * age);

		D3DXCOLOR color(0.5f, 1.0f, 1.0f, a);
		// ↑加算合成の場合 R * a, G * a, B * aされる

		Sprite_SetColor(color);

		Sprite_Draw(g_TextureId, g_Effects[i].position.x, g_Effects[i].position.y);

	}

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//元のαブレンドに戻す
	//g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);      //ブレンディング処理(デフォルト)
	//g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	 //αデスティネーションカラーの指定
}

//エフェクトの生成
void GameEffect_Spawn(float x, float y)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effects[i].enable) continue;

		g_Effects[i].enable = true;
		g_Effects[i].position = D3DXVECTOR2(x, y);
		g_Effects[i].birthFrame = g_FrameCount;


		break;
	}
}

