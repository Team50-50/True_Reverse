/*=======================================================================================

	 �Q�[���̃v���C���[����[game_player.cpp]
												author:mgc
												date:2020/07/17
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include<d3dx9.h>
#include<stdio.h>
#include"texture.h"
#include"sprite.h"
#include"sound.h"


#include"game_player.h"

#include"game_effect.h"
#include"config.h"
#include"keylogger.h"

 /*--------------------------------------------------------------------------------------
   �萔�錾
  ----------------------------------------------------------------------------------------*/
#define PLAYER_WIDTH  (80)
#define PLAYER_HEIGHT (80)
#define JUMP_FORCE (-5.0f)
#define GRAVITY (0.1f)

 /*-----------------------------------------------------------------------------------------
  �O���[�o���ϐ�
 -------------------------------------------------------------------------------------------*/
static int g_TexturePlayer = TEXTURE_INVALID_ID;
Player g_Player;
static Queue g_qCurrent;
static Queue g_qPrev;
static Stack g_sCurrent;
static Stack g_sPrev;


//�Q�[���v���C���[�̏�����
void GamePlayer_Initialize(void)
{
	g_TexturePlayer = Texture_SetTextureLoadFile("asset/player.tga");
	
	g_Player.position = D3DXVECTOR2(32.0f, 300.0f);
	g_Player.direction = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.speed = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.hp = 500.0f;
	g_Player.gravity = 0.0f;
	g_Player.isJump = false;

	//g_qCurrent.recordFrame_head = 0;
	//g_qCurrent.recordFrame_tail = 0;
	//g_qCurrent.recordFrame_head = 0;
	//g_qPrev.recordFrame_tail = 0;
	g_sCurrent.recordFrame = 0;
	g_sPrev.recordFrame = 0;
	
}

//�Q�[���v���C���[�̏I������
void GamePlayer_Finalize(void)
{
	Texture_Release(&g_TexturePlayer, 1);

}

//�Q�[���v���C���[�̍X�V����
void GamePlayer_Update(void) 
{
	//�i�s�����𒷂�1�ɂ���
	D3DXVec2Normalize(&g_Player.direction, &g_Player.direction);
	
	//�v���C���[���W�̍X�V (�ړ������~���x)
	g_Player.position.x += g_Player.direction.x * g_Player.speed.x;
	//g_Player.speed *= 0.85f;
	g_Player.position.y +=  g_Player.speed.y;

	g_Player.speed.y += GRAVITY;

	if (g_Player.position.y >= 600.0f)
	{
		g_Player.position.y = 600.0f;
		g_Player.isJump = false;
	}
	//���̃t���[���̂��߂ɐi�s�������N���A���Ă���
	g_Player.direction = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.prevposition = g_Player.position;

	if (g_Player.speed.x >= 0.1f)
	{
		GameEffect_Spawn(g_Player.position.x + (PLAYER_WIDTH - 80) / 2, g_Player.position.y + (PLAYER_HEIGHT - 80) / 2);

	}


	//================================================================
	//�L���[�\���ŁA�v���C���[��position�f�[�^�̍X�V�������s��
	//================================================================
	//if (Keylogger_Press(KL_J))
	//{
	//	//J�L�[��������������A���ݖ��t���[���̃v���C���[position���L�^����
	//	enqueue(&g_qCurrent, g_Player.position);
	//}
	//if (Keylogger_Press(KL_B))
	//{
	//	//B�L�[��������������A���̃t���[���̃f�[�^�z��̐擪�̃v���C���[position�����o��
	//	//�O�̃t���[���̃v���C���[position�f�[�^�Ƃ��ċL�^����
	//	enqueue(&g_qPrev, dequeue(&g_qCurrent));

	//}

	//================================================================
	//�X�^�b�N�\���ŁA�v���C���[��position�f�[�^�̍X�V�������s��
	//================================================================
	if (Keylogger_Press(KL_J))
	{
		//J�L�[��������������A���ݖ��t���[���̃v���C���[position���L�^����
		push(&g_sCurrent, g_Player.position);
	}
	if (Keylogger_Press(KL_B))
	{
		//B�L�[��������������A���̃t���[���̃f�[�^�z��̖����̃v���C���[position�����o��
		//�O�̃t���[���̃v���C���[position�f�[�^�Ƃ��ċL�^����
		push(&g_sPrev, pop(&g_sCurrent));
	}


}

//�Q�[���v���C���[�̕`�揈��
void GamePlayer_Draw(void)
{
	Sprite_Draw(g_TexturePlayer, g_Player.position.x, g_Player.position.y, PLAYER_WIDTH, PLAYER_HEIGHT, 0, 0, 270, 270);

}

//�Q�[���v���C���[�̈ړ�����
void GamePlayer_MoveLeft(void)
{
	g_Player.direction.x = -1.0f;
	g_Player.speed.x = 3.0f;
}
void GamePlayer_MoveRight(void)
{
	g_Player.direction.x = 1.0f;
	g_Player.speed.x = 3.0f;
}
void GamePlayer_Jump(void)
{
	if (!g_Player.isJump)
	{
		g_Player.speed.y = JUMP_FORCE;
		g_Player.isJump = true;
	}
}


//�v���C���[��Collision�̎擾
CollisionCircle GamePlayer_GetCollision(void)
{
	CollisionCircle c = {
		D3DXVECTOR2(
			g_Player.position.x + PLAYER_WIDTH * 0.5f,
			g_Player.position.y + PLAYER_HEIGHT * 0.5f
		),
		PLAYER_WIDTH * 0.5f
	};

	return c;
}

Player GetPlayer(void)
{
	return g_Player;
}

//�G���L���[...position�f�[�^���L���[�ɒǉ�����A����Ō���Ƀf�[�^��ǉ�����B
void enqueue(Queue* q, D3DXVECTOR2 playerPosition)
{
	if (q->recordFrame_tail > FRAME_MAX)
	{
		return;
	}
	q->position_Data[q->recordFrame_tail] = playerPosition;
	q->recordFrame_tail++;
}

//�f�L���[...position�f�[�^���L���[������o���A����擪�̃f�[�^(��ԌÂ���position�f�[�^)�����o���B
D3DXVECTOR2 dequeue(Queue* q)
{
	D3DXVECTOR2 tmp;
	tmp = q->position_Data[0];
	for (int i = 0; i < q->recordFrame_tail - 1; i++)
	{
		q->position_Data[i] = q->position_Data[i + 1];
	}
	q->position_Data[q->recordFrame_tail - 1] = D3DXVECTOR2(0.0f, 0.0f);
	q->recordFrame_tail--;
	return tmp;
}

Queue GetqCurrent(void)
{
	return g_qCurrent;
}

Queue GetqPrev(void)
{
	return g_qPrev;
}

//PUSH...position�f�[�^���X�^�b�N�ɒǉ�����
void push(Stack* stack, D3DXVECTOR2 playerPosition)
{
	if (stack->recordFrame > FRAME_MAX)
	{
		return;
	}
	stack->position_Data[stack->recordFrame] = playerPosition;

	stack->recordFrame++;
}

//POP...position�f�[�^���X�^�b�N������o���A����Ō� PUSH ���� position�f�[�^��������o��
D3DXVECTOR2 pop(Stack* stack)
{
	D3DXVECTOR2 ret;

	ret = stack->position_Data[stack->recordFrame-1];

	stack->recordFrame--;

	return ret;
}

Stack GetstackPrev(void)
{
	return g_sPrev;
}

Stack GetstackCurrent(void)
{
	return g_sCurrent;
}
