/*=======================================================================================

	 �Q�[���̃v���C���[����[game_player.h]
												author:mgc
												date:2020/07/17
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include"collision.h"
#define FRAME_MAX (3600)

 /*------------------------------------------------------------------------------------------
	�\���̐錾
 -------------------------------------------------------------------------------------------*/
//�v���C���[�\����
typedef struct Player_tag
{
	D3DXVECTOR2 position;
	D3DXVECTOR2 prevposition;
	D3DXVECTOR2 direction;
	D3DXVECTOR2 speed;
	float velocity;
	float hp;
	float gravity;
	bool  isJump;

}Player;

//�L���[�\����
typedef struct
{
	D3DXVECTOR2 position_Data[FRAME_MAX];
	
	int recordFrame_head;
	int recordFrame_tail;
}Queue;

//�X�^�b�N�\����
typedef struct
{
	D3DXVECTOR2 position_Data[FRAME_MAX];
	int playFrame;
	int recordFrame;
}Stack;

//�Q�[���v���C���[�̏�����
void GamePlayer_Initialize(void);

//�Q�[���v���C���[�̏I������
void GamePlayer_Finalize(void);

//�Q�[���v���C���[�̍X�V����
void GamePlayer_Update(void);

//�Q�[���v���C���[�̕`�揈��
void GamePlayer_Draw(void);

//�Q�[���v���C���[�̈ړ�����
void GamePlayer_MoveLeft(void);
void GamePlayer_MoveRight(void);
void GamePlayer_Jump(void);


//�v���C���[��Collision�̎擾
CollisionCircle GamePlayer_GetCollision(void);

Player GetPlayer(void);

//�G���L���[...position�f�[�^���L���[�ɒǉ�����A����Ō���Ƀf�[�^��ǉ�����B
void enqueue(Queue* q, D3DXVECTOR2 playerPosition);
//�f�L���[...position�f�[�^���L���[������o���A����擪�̃f�[�^(��ԌÂ���position�f�[�^)�����o���B
D3DXVECTOR2 dequeue(Queue* q);

Queue GetqCurrent(void);
Queue GetqPrev(void);

//PUSH...position�f�[�^���X�^�b�N�ɒǉ�����
void push(Stack* stack, D3DXVECTOR2 playerPosition);
//POP...position�f�[�^���X�^�b�N������o���A����Ō� PUSH ���� position�f�[�^��������o��
D3DXVECTOR2 pop(Stack* stack);

Stack GetstackPrev(void);
Stack GetstackCurrent(void);

#endif //GAME_PLAYER_H