/*=====================================================================================================

	�f�o�b�N�t�H���g���W���[��[debug_font.h]
																author:mgc
																date:2020/07/03
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H


//�f�o�b�N�t�H���g���W���[���̏�����
void DebugFont_Initialize(void);

//�f�o�b�N�t�H���g���W���[���̏I������
void DebugFont_Finalize(void);

//�f�o�b�N�t�H���g�̕\��
//
//�����Fdx,dy    ...�f�o�b�N������̕`����W
//		pString  ...�f�o�b�N������ւ̃|�C���^
//
void DebugFont_Draw(float dx, float dy, const char* pString);

#endif // !DEBUG_FONT_H

