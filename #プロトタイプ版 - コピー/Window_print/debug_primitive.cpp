/*=======================================================================================

	 デバック用図形表示モジュール[debug_primitive.cpp]
												author:mgc
												date:2020/09/25
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#if defined (_DEBUG) || defined(DEBUG)

#include<d3dx9.h>
#include<math.h>
#include"mydirect3d.h"

 /*--------------------------------------------------------------------------------------
  定数宣言
  ----------------------------------------------------------------------------------------*/
#define CIRCLE_VERTEX_COUNT (64)
#define CIRCLE_DRAW_MAX     (2048)

 /*-----------------------------------------------------------------------------------------
	構造体宣言
 -------------------------------------------------------------------------------------------*/
//デバック図形描画用頂点宣言
typedef struct DebugVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;

}DebugVertex;
#define FVF_DEBUG_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

/*----------------------------------------------------------------------------------------
   グローバル変数
 ----------------------------------------------------------------------------------------*/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static WORD g_CirleCount = 0;
static DebugVertex* g_pDebugVertex = NULL;
static WORD* g_pDebugVertexIndex = NULL;

#else
#include<windows.h>

#endif

/*----------------------------------------------------------------------------------------
  関数定義
 ----------------------------------------------------------------------------------------*/
void DebugPrimitive_Initialize(void)
{
#if defined (_DEBUG) || defined(DEBUG)

	//各グループ変数の初期化
	g_CirleCount = 0;
	g_pDebugVertex = NULL;
	g_pDebugVertexIndex = NULL;

	//頂点バッファやインデックスバッファの確保
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(DebugVertex) * CIRCLE_VERTEX_COUNT * CIRCLE_DRAW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_DEBUG_VERTEX,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL);

	pDevice->CreateIndexBuffer(
		sizeof(WORD) * CIRCLE_VERTEX_COUNT * 2 * CIRCLE_DRAW_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL);

#endif
}

void DebugPrimitive_Finalize(void)
{
#if defined (_DEBUG) || defined(DEBUG)

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

#endif
}

void DebugPrimitive_BatchBegin(void)
{
#if defined (_DEBUG) || defined(DEBUG)
	//頂点バッファと頂点インデックスバッファをロックする
	g_pVertexBuffer->Lock(0, 0, (void**)&g_pDebugVertex, 0);
	g_pIndexBuffer->Lock(0, 0, (void**)&g_pDebugVertexIndex, 0);

	//丸の描画命令数を初期化
	g_CirleCount = 0;

#endif
}

void DebugPrimitive_BatchDrawCircle(float x, float y, float radius)
{
#if defined (_DEBUG) || defined(DEBUG)
	//指定座標を円の中心とした丸の頂点座標を計算して
	//頂点バッファと頂点インデックスバッファに書き入れる

	//配列のどっから書き入れるか?
//#include<assert.h>
//	assert(CIRCLE_VERTEX_COUNT * g_CirleCount < 65535); //65535が超えたら、プログラムを止める
	WORD n = CIRCLE_VERTEX_COUNT * g_CirleCount;

	//三角形＿一個分の角度(ラジアン角)
	float s = D3DX_PI * 2.0f / CIRCLE_VERTEX_COUNT;

	//頂点バッファとインデックスバッファへ情報を書き込む
	for (WORD i = 0; i < CIRCLE_VERTEX_COUNT; i++)
	{
		g_pDebugVertex[n + i].position.x = cosf(s * i) * radius + x;
		g_pDebugVertex[n + i].position.y = sinf(s * i) * radius + y;
		g_pDebugVertex[n + i].position.z = 1.0f;
		g_pDebugVertex[n + i].position.w = 1.0f;
		g_pDebugVertex[n + i].color = D3DCOLOR_RGBA(0, 255, 0, 255);

		g_pDebugVertexIndex[n * 2 + i * 2] = n + i;
		g_pDebugVertexIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;

	}

	//丸の描画命令数を1増やす
	g_CirleCount++;

#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(radius);

#endif
}

void DebugPrimitive_BatchRun(void)
{
#if defined (_DEBUG) || defined(DEBUG)
	//頂点バッファと頂点インデックスバッファをアンロック
	g_pVertexBuffer->Unlock();
	g_pIndexBuffer->Unlock();

	//丸の描画命令数分の丸を描く
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	int vn = g_CirleCount * CIRCLE_VERTEX_COUNT;

	pDevice->SetFVF(FVF_DEBUG_VERTEX);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(DebugVertex));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, vn, 0, vn);


#endif
}

