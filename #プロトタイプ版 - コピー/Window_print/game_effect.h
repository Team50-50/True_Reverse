/*=======================================================================================

	 �Q�[���̃G�t�F�N�g����[game_effect.h]
												author:mgc
												date:2020/09/10
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef GAME_EFFECT_H_
#define GAME_EFFECT_H_


//�G�t�F�N�g�̏�����
void GameEffect_Initialize(void);

//�G�t�F�N�g�̏I������
void GameEffect_Finalize(void);

//�G�t�F�N�g�̍X�V����
void GameEffect_Update(void);

//�G�t�F�N�g�̕`�揈��
void GameEffect_Draw(void);

//�G�t�F�N�g�̐���
void GameEffect_Spawn(float x, float y);


#endif // !GAME_EFFECT_H_


