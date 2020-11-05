/*=======================================================================================

     �Q�[���I�u�W�F�N�g[main.cpp]
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
  �萔��`
 -----------------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"			//�E�C���h�E�N���X�̖��O
#define WINDOW_CAPTION "�E�C���h�E�\������" //�E�C���h�E�̖��O



/*----------------------------------------------------------------------------------------
  �v���g�^�C�v�錾
  ----------------------------------------------------------------------------------------*/
//�E�C���h�E�v���V�[�W��(�S�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�Q�[���̏������֐�
static bool Initialize(HWND hWnd);
//�Q�[���̍X�V�֐�
static void Update(void);
//�Q�[���̕`��֐�
static void Draw(void);
//�Q�[���̏I���֐�
static void Finalize(void);

/*----------------------------------------------------------------------------------------
  �O���[�o���ϐ�
  ----------------------------------------------------------------------------------------*/
static int g_FrameCount = 0;
static int g_BaseFrame = 0;
static double g_BaseTime = 0;
static double g_FPS = 0.0;
static double g_ReserveTime = 0.0;


/*-----------------------------------------------------------------------------------------
 ���C��
-------------------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�g�p���Ȃ�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//�E�C���h�E�N���X�\���̂̐���
	WNDCLASS wc = {};									//������
	wc.lpfnWndProc = WndProc;							//�E�C���h�E�v���V�[�W���̎w��
	wc.lpszClassName = CLASS_NAME;						//�N���X���O�̐ݒ�
	wc.hInstance = hInstance;							//�C���X�^���X�n���h���̎w��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//�}�E�X�J�[�\�����w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);  //�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

	//�N���X�o�^
	RegisterClass(&wc);

	//�E�C���h�E�̃X�^�C��
	const DWORD WINDOW_STYTLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
	//WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX)

	//�s���̗ǂ��E�C���h�E�T�C�Y�̎Z�o
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, WINDOW_STYTLE, FALSE);

	//�E�B���h�E�̋�`���畝�ƍ��������߂�
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//�v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//�f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	//�E�C���h�E�̐���
	HWND hWnd = CreateWindow(
		CLASS_NAME,		//�E�C���h�E�@�N���X
		WINDOW_CAPTION,	//�E�C���h�E�@�e�L�X�g

							// ^ (XOR)
		WINDOW_STYTLE,//�E�C���h�E �X�^�C��

		//	X		Y			��			  ����
		window_x, window_y, window_width, window_height,//�T�C�Y�ƈʒu

		NULL,		//�e�E�C���h�E�n���h��
		NULL,		//���j���[�n���h��
		hInstance,  //�C���X�^���X�n���h��
		NULL		//�ǉ��̃A�v���P�[�V�����f�[�^
	);

	if (hWnd == NULL)
	{
		//�E�C���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
		return 0;
	}

	//�w��̃E�C���h�E�n���h���̃E�C���h�E���w��̕��@�ŕ\��
	ShowWindow(hWnd, nCmdShow);

	//�Q�[���̏�����
	if (!Initialize(hWnd))
	{
		//���b�Z�[�W�{�b�N�X�ŃQ�[���̏����������s������I�����邱�Ƃ�`����
		
		return 0;
	}

	//Windows�Q�[���p���C�����[�v
	MSG msg = {}; //msg.message == WM_NULL
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�Q�[������
			double time = SystemTimer_GetTime();
			if ((time - g_ReserveTime) < 1.0f / 60.f)
			{
				Sleep(0);
				continue;
			}
			g_ReserveTime = time;

			//�X�V
			Update();

			//�`��
			Draw();

		}
	}

	//�Q�[���̏I������
	Finalize();

	return (int)msg.wParam;

}

//�E�C���h�E�v���V�[�W��(�S�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN:
			 if (wParam == VK_ESCAPE)
			 {
				 SendMessage(hWnd, WM_CLOSE, 0, 0);//WM_CLOSE���b�Z�[�W�̑��M
			 }
		case WM_ACTIVATEAPP:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			 Keyboard_ProcessMessage(uMsg, wParam, lParam);
			 break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł���?", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);//�w��̃E�C���h�E��WM_DESTROY���b�Z�[�W�𑗂�
		}
		return 0;//DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���
		//�L�����Z�������Ɋ֌W����__break�Ȃ�E�C���h�E������
	case WM_DESTROY: //�E�C���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0);//WM_QUIT���b�Z�[�W�̑��M
		return 0;
	};
	
	//�ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//������
bool Initialize(HWND hWnd)
{
	//�L�[�{�[�h�̏�����
	Keyboard_Initialize();

	//�L�[���K�[�̏�����
	Keylogger_Initialize();


	//Direct3D�̏�����
	if (!MyDirect3D_Initialize(hWnd))
	{
		MessageBox(NULL, "Direct3D�̏������Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	//�f�o�b�N�}�`�`�惂�W���[���̏�����
	DebugPrimitive_Initialize();

	//�e�N�X�`���Ǘ����W���[���̏�����
	Texture_Initialize();

	//�X�v���C�g���W���[���̏�����
	Sprite_Initialize();

	//�f�o�b�N�t�H���g���W���[���̏�����
	DebugFont_Initialize();

	//�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();

	g_ReserveTime = g_BaseTime = SystemTimer_GetTime();
	g_FrameCount = g_BaseFrame = 0;
	g_FPS = 0.0;
	
	Game_Initialize();

	return true;//����������
}

//�Q�[���̍X�V�֐�
void Update(void)
{
	//�L�[���K�[�̍X�V����
	Keylogger_Update();

	Game_Update();
	
	//�v��
	double time = SystemTimer_GetTime();
	if (time - g_BaseTime > 1.0)
	{
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_BaseTime);
		g_BaseTime = time;
		g_BaseFrame = g_FrameCount;
	}

	g_FrameCount++;
}

//�Q�[���̕`��֐�
void Draw(void)
{
	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	//��ʂ̃N���A
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	//g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	//�`��o�b�`���߂̊J�n
	pDevice->BeginScene();

	Game_Draw();

	//�f�o�b�O�}�`�`�施�߂̎�t�J�n
	DebugPrimitive_BatchBegin();


	//FPS�\��
	char buf[64];
	sprintf(buf,"FPS = %.2f", g_FPS);
	DebugFont_Draw(0.0f, 0.0f, buf);

	Player player = GetPlayer();
	sprintf(buf, "px=%.2f", player.position.x);
	DebugFont_Draw(400.0f, 0.0f, buf);

	sprintf(buf, "py=%.2f", player.position.y);
	DebugFont_Draw(400.0f, 45.0f, buf);

	VPlayer vplayer = GetVPlayer();
	sprintf(buf, "px=%.2f", vplayer.position.x);
	DebugFont_Draw(700.0f, 0.0f, buf);

	sprintf(buf, "py=%.2f", vplayer.position.y);
	DebugFont_Draw(700.0f, 45.0f, buf);

	
	//�f�o�b�O�}�`�`�施�߂̎��s
	DebugPrimitive_BatchRun();

	//�`��o�b�`���߂̏I��
	pDevice->EndScene();

	//�o�b�N�o�b�t�@���t���b�v(�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��)
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//�I������
void Finalize(void)
{
	//�Q�[���̏I������
	Game_Finalize();

	//�f�o�b�N�t�H���g���W���[���̏I������
	DebugFont_Finalize();

	//�X�v���C�g���W���[���̏I������
	Sprite_Finalaize();

	//�e�N�X�`�Ǘ����W���[���̏I������
	Texture_Finalize();


	//Direct3D�I������
	MyDirect3D_Finalize();

	//�L�[���K�[�̏I������
	Keylogger_Finalize();
}