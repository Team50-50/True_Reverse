/*=======================================================================================

	texture管理[texture.cpp]
												author:mgc
												date:2020/06/26
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include"texture.h"

#include<d3dx9.h>
#include<string.h>

#include"mydirect3d.h"
#include"debugPrintf.h"

 /*-----------------------------------------------------------------------------------------
   定数
  -------------------------------------------------------------------------------------------*/
#define TEXTURE_MAX          (1024)//テクスチャ管理最大数

 /*-----------------------------------------------------------------------------------------
  構造体宣言
 -------------------------------------------------------------------------------------------*/
//テクスチャ管理テーブル構造体
typedef struct Texture_tag
{
	LPDIRECT3DTEXTURE9 pTexture;
	unsigned long width;
	unsigned long height;
	char filename[TEXTURE_FILENAME_MAX];

}Texture;


/*-----------------------------------------------------------------------------------------
  グローバル変数
 -------------------------------------------------------------------------------------------*/
 //テクスチャ管理テーブル
static Texture g_Textures[TEXTURE_MAX];


//テクスチャ管理モジュールの初期化
void Texture_Initialize(void)
{
	//テクスチャ管理テーブルの初期化
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Textures[i].pTexture = NULL;
		g_Textures[i].width = 0;
		g_Textures[i].height = 0;
		g_Textures[i].filename[0] = 0;
	}
}

//テクスチャ管理モジュールの終了処理
void Texture_Finalize(void)
{
	Texture_AllRelease();
}

//テクスチャファイルの読み込み予約
//
//引数：pFileName ... ファイル名
//
//戻り値:テクスチャ管理番号
//　　　 最大管理数を超えていた場合はINVALID_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが空いていたらスキップ
		if (g_Textures[i].filename[0] == 0)
		{
			continue;
		}
		//すでに指定のファイルが予約されているか?
		if (strcmp(pFileName, g_Textures[i].filename) == 0)
		{
			//予約されている
			return i;
		}
	}

	//新規予約
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが空いていたらスキップ
		if (g_Textures[i].filename[0] != 0)
		{
			continue;
		}

		//ファイル名をテーブルにコピーする
		strcpy(g_Textures[i].filename, pFileName);

		return i;

	}

	return TEXTURE_INVALID_ID;
}

//テクスチャの読み込み
//
//予約されているファイルを読み込みます
//
//戻り値:読み込めなかったファイル数
//
int Texture_Load(void)
{
	int err_count = 0;

	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return -1;
	}
	
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].filename[0] == 0)
		{
			//空のテーブルなのでスキップ
			continue;
		}
		if (g_Textures[i].pTexture != NULL)
		{
			//すでに読み込まれている
			continue;
		}

		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Textures[i].filename, &g_Textures[i].pTexture);
		if (FAILED(hr))
		{
			//テクスチャの読み込みに失敗
			DebugPrintf("読み込めなかったテクスチャファイル:[%s]\n", g_Textures[i].filename);
			err_count++;
		}
		else
		{
			//テクスチャの解像度（幅・高さ）の調査
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Textures[i].filename, &info);
			g_Textures[i].width = info.Width;
			g_Textures[i].height = info.Height;
			DebugPrintf("[%s]が読み込み成功\n", g_Textures[i].filename);
		}
	}

	return err_count; //読み込めなかった数
}

//テクスチャの部分解放
//
//引数：textureIds[]...解放したいtextureIdが入ったint型配列の先頭アドレス
//		count       ...解放するテクスチャの数
//
void Texture_Release(int textureIds[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (g_Textures[textureIds[i]].pTexture)
		{
			g_Textures[textureIds[i]].pTexture->Release();
			g_Textures[textureIds[i]].pTexture = NULL;
		}
		g_Textures[textureIds[i]].filename[0] = 0;
		g_Textures[textureIds[i]].width  = 0;
		g_Textures[textureIds[i]].height = 0;

	}
}

//テクスチャの全解放
void Texture_AllRelease(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture)
		{
			g_Textures[i].pTexture->Release();
			g_Textures[i].pTexture = NULL;
		}
		g_Textures[i].filename[0] = 0;
		g_Textures[i].width = 0;
		g_Textures[i].height = 0;

	}
}


//テクスチャの幅の取得
//
//引数：テクスチャ番号
//
//戻り値;テクスチャの幅
//
unsigned long Texture_GetTextureWidth(int textureId)
{
	return g_Textures[textureId].width;
}

//テクスチャの高さの取得
//
//引数：テクスチャ番号
//
//戻り値;テクスチャの高さ
//
unsigned long Texture_GetTextureHeight(int textureId)
{
	return g_Textures[textureId].height;
}

//テクスチャインターフェースポインタの取得
//
//引数：テクスチャ管理番号
//
//戻り値:テクスチャインターフェースポインタ
//	     読み込めていないまたは不正な管理番号だった場合NULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId)
{
	return g_Textures[textureId].pTexture;
}