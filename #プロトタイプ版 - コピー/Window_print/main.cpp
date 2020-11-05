/*=======================================================================================

     ゲームオブジェクト[main.cpp]
												author:mgc
												date:2020/05/21
 -----------------------------------------------------------------------------------------
     
 =========================================================================================*/
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<stdio.h>

#include"config.h"
#include"mydirect3d.h"
#include"system_timer.h"
#include"texture.h"
#include"sprite.h"
#include"game.h"
#include"keyboard.h"
#include"keylogger.h"
#include"debug_font.h"
#include"debug_primitive.h"
#include"game_player.h"
#include"game_virtual_player.h"

/*----------------------------------------------------------------------------------------
  定数定義
 -----------------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"			//ウインドウクラスの名前
#define WINDOW_CAPTION "ウインドウ表示処理" //ウインドウの名前



/*----------------------------------------------------------------------------------------
  プロトタイプ宣言
  ----------------------------------------------------------------------------------------*/
//ウインドウプロシージャ(ゴールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ゲームの初期化関数
static bool Initialize(HWND hWnd);
//ゲームの更新関数
static void Update(void);
//ゲームの描画関数
static void Draw(void);
//ゲームの終了関数
static void Finalize(void);

/*----------------------------------------------------------------------------------------
  グローバル変数
  ----------------------------------------------------------------------------------------*/
static int g_FrameCount = 0;
static int g_BaseFrame = 0;
static double g_BaseTime = 0;
static double g_FPS = 0.0;
static double g_ReserveTime = 0.0;


/*-----------------------------------------------------------------------------------------
 メイン
-------------------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//使用しない
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//ウインドウクラス構造体の説明
	WNDCLASS wc = {};									//初期化
	wc.lpfnWndProc = WndProc;							//ウインドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME;						//クラス名前の設定
	wc.hInstance = hInstance;							//インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);  //ウインドウのクライアント領域の背景色を設定

	//クラス登録
	RegisterClass(&wc);

	//ウインドウのスタイル
	const DWORD WINDOW_STYTLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
	//WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX)

	//都合の良いウインドウサイズの算出
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, WINDOW_STYTLE, FALSE);

	//ウィンドウの矩形から幅と高さを求める
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//デスクトップの真ん中にウィンドウが生成されるように座標を計算
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	//ウインドウの生成
	HWND hWnd = CreateWindow(
		CLASS_NAME,		//ウインドウ　クラス
		WINDOW_CAPTION,	//ウインドウ　テキスト

							// ^ (XOR)
		WINDOW_STYTLE,//ウインドウ スタイル

		//	X		Y			幅			  高さ
		window_x, window_y, window_width, window_height,//サイズと位置

		NULL,		//親ウインドウハンドル
		NULL,		//メニューハンドル
		hInstance,  //インスタンスハンドル
		NULL		//追加のアプリケーションデータ
	);

	if (hWnd == NULL)
	{
		//ウインドウハンドルが何らかの理由で生成出来なかった
		return 0;
	}

	//指定のウインドウハンドルのウインドウを指定の方法で表示
	ShowWindow(hWnd, nCmdShow);

	//ゲームの初期化
	if (!Initialize(hWnd))
	{
		//メッセージボックスでゲームの初期化が失敗したら終了することを伝える
		
		return 0;
	}

	//Windowsゲーム用メインループ
	MSG msg = {}; //msg.message == WM_NULL
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//ゲーム処理
			double time = SystemTimer_GetTime();
			if ((time - g_ReserveTime) < 1.0f / 60.f)
			{
				Sleep(0);
				continue;
			}
			g_ReserveTime = time;

			//更新
			Update();

			//描画
			Draw();

		}
	}

	//ゲームの終了処理
	Finalize();

	return (int)msg.wParam;

}

//ウインドウプロシージャ(ゴールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN:
			 if (wParam == VK_ESCAPE)
			 {
				 SendMessage(hWnd, WM_CLOSE, 0, 0);//WM_CLOSEメッセージの送信
			 }
		case WM_ACTIVATEAPP:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			 Keyboard_ProcessMessage(uMsg, wParam, lParam);
			 break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか?", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);//指定のウインドウにWM_DESTROYメッセージを送る
		}
		return 0;//DefWindowProc関数にメッセージを流さず終了することによって何もなっかたことにする
		//キャンセルおすに関係ある__breakならウインドウも閉じる
	case WM_DESTROY: //ウインドウの破棄メッセージ
		PostQuitMessage(0);//WM_QUITメッセージの送信
		return 0;
	};
	
	//通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//初期化
bool Initialize(HWND hWnd)
{
	//キーボードの初期化
	Keyboard_Initialize();

	//キーロガーの初期化
	Keylogger_Initialize();


	//Direct3Dの初期化
	if (!MyDirect3D_Initialize(hWnd))
	{
		MessageBox(NULL, "Direct3Dの初期化に失敗しました", "エラー", MB_OK);
		return false;
	}

	//デバック図形描画モジュールの初期化
	DebugPrimitive_Initialize();

	//テクスチャ管理モジュールの初期化
	Texture_Initialize();

	//スプライトモジュールの初期化
	Sprite_Initialize();

	//デバックフォントモジュールの初期化
	DebugFont_Initialize();

	//システムタイマーの初期化
	SystemTimer_Initialize();

	g_ReserveTime = g_BaseTime = SystemTimer_GetTime();
	g_FrameCount = g_BaseFrame = 0;
	g_FPS = 0.0;
	
	Game_Initialize();

	return true;//初期化成功
}

//ゲームの更新関数
void Update(void)
{
	//キーロガーの更新処理
	Keylogger_Update();

	Game_Update();
	
	//計測
	double time = SystemTimer_GetTime();
	if (time - g_BaseTime > 1.0)
	{
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_BaseTime);
		g_BaseTime = time;
		g_BaseFrame = g_FrameCount;
	}

	g_FrameCount++;
}

//ゲームの描画関数
void Draw(void)
{
	//Direct3Dデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	//画面のクリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	//g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	//描画バッチ命令の開始
	pDevice->BeginScene();

	Game_Draw();

	//デバッグ図形描画命令の受付開始
	DebugPrimitive_BatchBegin();


	//FPS表示
	char buf[64];
	sprintf(buf,"FPS = %.2f", g_FPS);
	DebugFont_Draw(0.0f, 0.0f, buf);

	Player player = GetPlayer();
	sprintf(buf, "px=%.2f", player.position.x);
	DebugFont_Draw(400.0f, 0.0f, buf);

	sprintf(buf, "py=%.2f", player.position.y);
	DebugFont_Draw(400.0f, 45.0f, buf);

	VPlayer vplayer = GetVPlayer();
	sprintf(buf, "Vpx=%.2f", vplayer.position.x);
	DebugFont_Draw(900.0f, 0.0f, buf);

	sprintf(buf, "Vpy=%.2f", vplayer.position.y);
	DebugFont_Draw(900.0f, 45.0f, buf);
	
	//デバッグ図形描画命令の実行
	DebugPrimitive_BatchRun();

	//描画バッチ命令の終了
	pDevice->EndScene();

	//バックバッファをフリップ(タイミングはD3DPRESENT_PARAMETERSの設定による)
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//終了処理
void Finalize(void)
{
	//ゲームの終了処理
	Game_Finalize();

	//デバックフォントモジュールの終了処理
	DebugFont_Finalize();

	//スプライトモジュールの終了処理
	Sprite_Finalaize();

	//テクスチ管理モジュールの終了処理
	Texture_Finalize();


	//Direct3D終了処理
	MyDirect3D_Finalize();

	//キーロガーの終了処理
	Keylogger_Finalize();
}