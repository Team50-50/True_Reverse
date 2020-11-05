/*=====================================================================================================

	�t�s�V�X�e������[game_virtual_player.cpp]
																author:mgc
																date:2020/11/01
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#include <stdio.h>
#include"game_virtual_player.h"
#include"game_player.h"
#include"texture.h"
#include"sprite.h"
#include"keylogger.h"

/*--------------------------------------------------------------------------------------
   �萔�錾
  ----------------------------------------------------------------------------------------*/
#define VPLAYER_WIDTH  (80)
#define VPLAYER_HEIGHT (80)

/*-----------------------------------------------------------------------------------------
  �O���[�o���ϐ�
 -------------------------------------------------------------------------------------------*/
static int g_TextureVPlayer = TEXTURE_INVALID_ID;
//VPlayer g_VPlayer[VPLAYER_MAX];
VPlayer g_VPlayer;



void VPlayer_Initialize(void)
{
	g_TextureVPlayer = Texture_SetTextureLoadFile("asset/player.tga");

}


void VPlayer_Finalize(void)
{
	Texture_Release(&g_TextureVPlayer, 1);
}


void VPlayer_Update(void)
{
	//Queue q = GetqPrev();
	Stack prev_stack = GetstackPrev();

	//���z�v���C���[��position���X�V����
	//�v���C���[�O�̃t���[����position�f�[�^���X�^�b�N�̈悩��擾����
	for (int i = 0; i < prev_stack.recordFrame; i++)
	{
		if (Keylogger_Press(KL_B))
		{
			//B�L�[��������������A�v���C���[�O�̃t���[����position�f�[�^��1�t���[�����A���z�v���C���[�ɑ������
			//B�L�[�𗣂�����A���z�v���C���[�����̏�Ŏ~�܂�
			g_VPlayer.position = prev_stack.position_Data[i];
		}

	}
	
	
}


void VPlayer_Draw(void)
{
	
	Sprite_SetColor(D3DCOLOR_RGBA(255, 0, 255, 255));

	Sprite_Draw(g_TextureVPlayer, g_VPlayer.position.x, g_VPlayer.position.y,
		VPLAYER_WIDTH, VPLAYER_HEIGHT, 0, 0, 270, 270);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}


VPlayer GetVPlayer(void)
{
	return g_VPlayer;
}
