/*=======================================================================================

	texture管理[texture.h]
												author:mgc
												date:2020/06/26
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H
 
#include<d3d9.h>
/*-----------------------------------------------------------------------------------------
   定数
-------------------------------------------------------------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) //テクスチャファイル名最大文字数
#define TEXTURE_INVALID_ID   (-1) //無効なテクスチャ管理番号


//テクスチャ管理モジュールの初期化
void Texture_Initialize(void);

//テクスチャ管理モジュールの終了処理
void Texture_Finalize(void);

//テクスチャファイルの読み込み予約
//
//引数：pFileName ... ファイル名
//
//戻り値:テクスチャ管理番号
//　　　 最大管理数を超えていた場合はINVALID_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName);

//テクスチャの読み込み
//
//予約されているファイルを読み込みます
//
//戻り値:読み込めなかったファイル数
//		 または他の不具合が発生した場合-1
//
int Texture_Load(void);

//テクスチャの部分解放
//
//引数：textureIds[]...解放したいtextureIdが入ったint型配列の先頭アドレス
//		count       ...解放するテクスチャの数
//
void Texture_Release(int textureIds[], int count);

//テクスチャの全解放
void Texture_AllRelease(void);


//テクスチャの幅の取得
//
//引数：テクスチャ番号
//
//戻り値;テクスチャの幅
//
unsigned long Texture_GetTextureWidth(int textureId);

//テクスチャの高さの取得
//
//引数：テクスチャ番号
//
//戻り値;テクスチャの高さ
//
unsigned long Texture_GetTextureHeight(int textureId);

//テクスチャインターフェースポインタの取得
//
//引数：テクスチャ管理番号
//
//戻り値:テクスチャインターフェースポインタ
//	     読み込めていないまたは不正な管理番号だった場合NULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId);


#endif //TEXTURE_H
