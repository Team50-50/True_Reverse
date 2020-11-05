/*=======================================================================================

	スプライト制御[sprite.cpp]
												author:mgc
												date:2020/07/01
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include<d3dx9.h>

#include"mydirect3d.h"
#include"texture.h"

 /*-----------------------------------------------------------------------------------------
  構造体宣言
 -------------------------------------------------------------------------------------------*/
 //頂点構造体
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 Position;
	D3DCOLOR	Color;
	D3DXVECTOR2 TexCoord;

}Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//FVF フレキシブルバーテックスフォーマット
//RHW = 1 ... 座標変換済み頂点

 /*-----------------------------------------------------------------------------------------
  グローバル変数
 -------------------------------------------------------------------------------------------*/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //頂点バッファーインターフェース
static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer = NULL;  //インデックスバッファインターフェース
static D3DCOLOR g_Color = 0xffffffff;	//ポリゴンカラー

 //スプライトの初期化
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	pDevice->CreateVertexBuffer(
		sizeof(Vertex2D) * 4, //頂点バッファの量(バイト)
		D3DUSAGE_WRITEONLY,   //使い方
		FVF_VERTEX2D,		  //FVF
		D3DPOOL_MANAGED,	  //メモリの管理法
		&g_pVertexBuffer,	  //取得したインターフェースのアドレスを記録するためのポインタへのアドレス
		NULL
	);

	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,	  
		NULL
	);
}

//スプライトの終了処理
void Sprite_Finalaize(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}


//スプライトポリゴンカラー設定
//
//引数：color
//
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

//スプライトの描画
//*テクスチャサイズのポリゴンで指定座標に描画
//
//引数：textureId...テクスチャ管理番号
//
//		dx       ...描画座標x(左上指定)
//		dy       ...描画座標y(左上指定)
//
void Sprite_Draw(int textureId, float dx, float dy)
{
	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	//デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//ポリゴンのサイズはテクスチャのサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);


	//頂点データ
	Vertex2D v[] = {  
		{ D3DXVECTOR4(dx     - 0.5f,dy     - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f,dy     - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR4(dx     - 0.5f,dy + h - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f,dy + h - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(1.0f,1.0f) },
	};

	//頂点バッファをロックしてデータを書き込む
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	//データを書き込む
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();
	
	//インデックスバッファをロックしてデータを書き込む
	WORD* pI;
	g_pIndexBuffer->Lock(0, 0, (void**)&pI, 0);
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;

	g_pIndexBuffer->Unlock();

	//デバイスに頂点バッファの利用を設定
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	//デバイスにインデックスバッファの利用を設定
	pDevice->SetIndices(g_pIndexBuffer);


	//ポリゴン描画(簡易版)
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
}


//スプライトの描画
//＊テクスチャ切り取りサイズのポリゴンで指定座標に描画
//
//引数：textureId...テクスチャ管理番号
//
//		dx       ...描画座標x(左上指定)
//		dy       ...描画座標y(左上指定)
//		tcx      ...テクスチャ切り取り座標x
//		tcy      ...テクスチャ切り取り座標y
//		tcw      ...テクスチャ切り取り幅
//		tch      ...テクスチャ切り取り高さ
//
void Sprite_Draw(int textureId, float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	//デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//ポリゴンのサイズはテクスチャのサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx       - 0.5f,dy       - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dx + tcw - 0.5f,dy       - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(dx       - 0.5f,dy + tch - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dx + tcw - 0.5f,dy + tch - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	//データを書き込む
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	//デバイスに利用する頂点バッファを指定する
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	//ポリゴン描画(簡易版)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}


//スプライトの描画
//＊テクスチャサイズのポリゴンで指定座標に描画
//
//引数：textureId...テクスチャ管理番号
//
//		dx       ...描画座標x(左上指定)
//		dy       ...描画座標y(左上指定)
//		dw		 ...ポリゴンのサイズ幅
//		dh		 ...ポリゴンのサイズ高さ
//		tcx      ...テクスチャ切り取り座標x
//		tcy      ...テクスチャ切り取り座標y
//		tcw      ...テクスチャ切り取り幅
//		tch      ...テクスチャ切り取り高さ
//
void Sprite_Draw(int textureId, float dx, float dy,float dw,float dh, int tcx, int tcy, int tcw, int tch)
{
	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	//デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//ポリゴンのサイズはテクスチャのサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;


	//頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx      - 0.5f,dy      - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dx + dw - 0.5f,dy      - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(dx      - 0.5f,dy + dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dx + dw - 0.5f,dy + dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	//ポリゴン描画(簡易版)
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}

//スプライトの描画
//＊回転機能付き
//
//引数：textureId...テクスチャ管理番号
//
//		dx       ...描画座標x(左上指定)
//		dy       ...描画座標y(左上指定)
//		dw		 ...ポリゴンのサイズ幅
//		dh		 ...ポリゴンのサイズ高さ
//		tcx      ...テクスチャ切り取り座標x
//		tcy      ...テクスチャ切り取り座標y
//		tcw      ...テクスチャ切り取り幅
//		tch      ...テクスチャ切り取り高さ
//		cx		 ...回転の中心座標x
//		cy		 ...回転の中心座標y
//		angle    ...回転角度(ラジアン)
//
void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle)
{
	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	//デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//ポリゴンのサイズはテクスチャのサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(   - 0.5f,   - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dw - 0.5f,   - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(   - 0.5f,dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dw - 0.5f,dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	//	平行移動行列
	D3DXMATRIX mtxTranslationC;
	D3DXMatrixTranslation(&mtxTranslationC, -cx, -cy, 0.0f);

	D3DXMATRIX mtxTranslationI;
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dy, 0.0f);

	//回転行列
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation, angle);

	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);


	//行列の合成
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxRotation * mtxTranslationI;

	//座標変換
	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].Position, &v[i].Position, &mtxWorld);
	}
		


	//ポリゴン描画(簡易版)
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}