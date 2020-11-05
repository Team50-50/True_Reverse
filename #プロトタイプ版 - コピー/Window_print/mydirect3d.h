/*=======================================================================================

	 Direct3D制御[mydirect3d.h]
												author:mgc
												date:2020/06/17
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef MY_DIRECT3D_H
#define MY_DIRECT3D_H

#include<d3d9.h>
//Direct3D関連の初期化
//
//戻り値:初期化に失敗したらfalse
//
bool MyDirect3D_Initialize(HWND hWnd);

//Direct3D関連の終了処理
//
void MyDirect3D_Finalize(void);

//Direct3Dデバイスの取得
//
//戻り値:Direct3Dデバイスへのポインタ
//
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif // MY_DIRECT3D_H
