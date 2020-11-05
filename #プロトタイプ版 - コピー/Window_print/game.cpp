/*=====================================================================================================

	�Q�[���̐���[game.cpp]
																author:mgc
																date:2020/06/24
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#include"debug_font.h"
#include"game_player.h"
#include"texture.h"
#include"keylogger.h"
#include"config.h"
#include"game_effect.h"
#include"game_virtual_player.h"

//�Q�[���̏�����
void Game_Initialize(void)
{

	GamePlayer_Initialize();
	VPlayer_Initialize();
	GameEffect_Initialize();

	//�e�N�X�`���̓ǂݍ���
	if (Texture_Load() > 0)
	{
		//�f�o�C�X�̎擾�Ɏ��s
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);

	}
	//Keylogger_RecordStart(600);
	//Keylogger_RecordLoad();
	//Keylogger_RecordPlay();

	//PlaySound(SOUND_LABEL_BGM000);
}

//�Q�[���̍X�V����
void Game_Update(void)
{
	if (Keylogger_Press(KL_UP))
	{
		
	}
	if (Keylogger_Press(KL_DOWN))
	{
		
	}
	if (Keylogger_Press(KL_LEFT))
	{
		GamePlayer_MoveLeft();
	}
	if (Keylogger_Press(KL_RIGHT))
	{
		GamePlayer_MoveRight();
	} 

	if (Keylogger_Press(KL_JUMP))
	{
		GamePlayer_Jump();
	}

	if (Keylogger_Trigger(KL_R))
	{
			
	}

	GamePlayer_Update();
	GameEffect_Update();
	VPlayer_Update();
}

//�Q�[���̕`�揈��
void Game_Draw(void)
{
	GameEffect_Draw();
	GamePlayer_Draw();
	VPlayer_Draw();
}

//�Q�[���̏I������
void Game_Finalize(void)
{
	GamePlayer_Finalize();
	VPlayer_Finalize();
}
