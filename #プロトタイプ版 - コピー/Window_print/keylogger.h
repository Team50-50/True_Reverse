/*=====================================================================================================

	�L�[���͂̋L�^����[keylogger.h]
																author:mgc
																date:2020/07/29
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#ifndef KEYLOGGER_H
#define KEYLOGGER_H


//�L�[���K�[�L�[�񋓌^
typedef enum KeyloggerKey_tag
{
	KL_UP,
	KL_DOWN,
	KL_LEFT,
	KL_RIGHT,
	KL_JUMP,
	KL_B,
	KL_J,
	KL_R,
	KL_MAX

}KeyloggerKey;

//�L�[���K�[���W���[���̏�����
void Keylogger_Initialize(void);

//�L�[���K�[���W���[���̏I������
void Keylogger_Finalize(void);

//�L�[���K�[���W���[���̍X�V����
void Keylogger_Update(void);


//�L�[���͏�Ԃ̎擾
//
//�����F
//
//�߂�l�F������Ă�����true
//
bool Keylogger_Press(KeyloggerKey kl);


//�L�[���͏�Ԃ̎擾(�������u��)
//
//�����F
//
//�߂�l�F�������u�Ԃ�������true
//
bool Keylogger_Trigger(KeyloggerKey kl);


//�L�[���͏�Ԃ̎擾(�������u��)
//
//�����F
//
//�߂�l�F�������u�Ԃ�������true
//
bool Keylogger_Release(KeyloggerKey kl);



void Keylogger_RecordStart(int frame_max);
void Keylogger_RecordEnd(void);
void Keylogger_RecordLoad(void);
void Keylogger_RecordPlay(void);
bool Keylogger_IsRecordPlay(void);
int GetMode(void);

#endif // !KEYLOGGER_H
