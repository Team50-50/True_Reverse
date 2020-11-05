/*=======================================================================================

	texture�Ǘ�[texture.h]
												author:mgc
												date:2020/06/26
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H
 
#include<d3d9.h>
/*-----------------------------------------------------------------------------------------
   �萔
-------------------------------------------------------------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) //�e�N�X�`���t�@�C�����ő啶����
#define TEXTURE_INVALID_ID   (-1) //�����ȃe�N�X�`���Ǘ��ԍ�


//�e�N�X�`���Ǘ����W���[���̏�����
void Texture_Initialize(void);

//�e�N�X�`���Ǘ����W���[���̏I������
void Texture_Finalize(void);

//�e�N�X�`���t�@�C���̓ǂݍ��ݗ\��
//
//�����FpFileName ... �t�@�C����
//
//�߂�l:�e�N�X�`���Ǘ��ԍ�
//�@�@�@ �ő�Ǘ����𒴂��Ă����ꍇ��INVALID_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName);

//�e�N�X�`���̓ǂݍ���
//
//�\�񂳂�Ă���t�@�C����ǂݍ��݂܂�
//
//�߂�l:�ǂݍ��߂Ȃ������t�@�C����
//		 �܂��͑��̕s������������ꍇ-1
//
int Texture_Load(void);

//�e�N�X�`���̕������
//
//�����FtextureIds[]...���������textureId��������int�^�z��̐擪�A�h���X
//		count       ...�������e�N�X�`���̐�
//
void Texture_Release(int textureIds[], int count);

//�e�N�X�`���̑S���
void Texture_AllRelease(void);


//�e�N�X�`���̕��̎擾
//
//�����F�e�N�X�`���ԍ�
//
//�߂�l;�e�N�X�`���̕�
//
unsigned long Texture_GetTextureWidth(int textureId);

//�e�N�X�`���̍����̎擾
//
//�����F�e�N�X�`���ԍ�
//
//�߂�l;�e�N�X�`���̍���
//
unsigned long Texture_GetTextureHeight(int textureId);

//�e�N�X�`���C���^�[�t�F�[�X�|�C���^�̎擾
//
//�����F�e�N�X�`���Ǘ��ԍ�
//
//�߂�l:�e�N�X�`���C���^�[�t�F�[�X�|�C���^
//	     �ǂݍ��߂Ă��Ȃ��܂��͕s���ȊǗ��ԍ��������ꍇNULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId);


#endif //TEXTURE_H
