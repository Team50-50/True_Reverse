/*=======================================================================================

	texture�Ǘ�[texture.cpp]
												author:mgc
												date:2020/06/26
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include"texture.h"

#include<d3dx9.h>
#include<string.h>

#include"mydirect3d.h"
#include"debugPrintf.h"

 /*-----------------------------------------------------------------------------------------
   �萔
  -------------------------------------------------------------------------------------------*/
#define TEXTURE_MAX          (1024)//�e�N�X�`���Ǘ��ő吔

 /*-----------------------------------------------------------------------------------------
  �\���̐錾
 -------------------------------------------------------------------------------------------*/
//�e�N�X�`���Ǘ��e�[�u���\����
typedef struct Texture_tag
{
	LPDIRECT3DTEXTURE9 pTexture;
	unsigned long width;
	unsigned long height;
	char filename[TEXTURE_FILENAME_MAX];

}Texture;


/*-----------------------------------------------------------------------------------------
  �O���[�o���ϐ�
 -------------------------------------------------------------------------------------------*/
 //�e�N�X�`���Ǘ��e�[�u��
static Texture g_Textures[TEXTURE_MAX];


//�e�N�X�`���Ǘ����W���[���̏�����
void Texture_Initialize(void)
{
	//�e�N�X�`���Ǘ��e�[�u���̏�����
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Textures[i].pTexture = NULL;
		g_Textures[i].width = 0;
		g_Textures[i].height = 0;
		g_Textures[i].filename[0] = 0;
	}
}

//�e�N�X�`���Ǘ����W���[���̏I������
void Texture_Finalize(void)
{
	Texture_AllRelease();
}

//�e�N�X�`���t�@�C���̓ǂݍ��ݗ\��
//
//�����FpFileName ... �t�@�C����
//
//�߂�l:�e�N�X�`���Ǘ��ԍ�
//�@�@�@ �ő�Ǘ����𒴂��Ă����ꍇ��INVALID_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//�e�[�u�����󂢂Ă�����X�L�b�v
		if (g_Textures[i].filename[0] == 0)
		{
			continue;
		}
		//���łɎw��̃t�@�C�����\�񂳂�Ă��邩?
		if (strcmp(pFileName, g_Textures[i].filename) == 0)
		{
			//�\�񂳂�Ă���
			return i;
		}
	}

	//�V�K�\��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//�e�[�u�����󂢂Ă�����X�L�b�v
		if (g_Textures[i].filename[0] != 0)
		{
			continue;
		}

		//�t�@�C�������e�[�u���ɃR�s�[����
		strcpy(g_Textures[i].filename, pFileName);

		return i;

	}

	return TEXTURE_INVALID_ID;
}

//�e�N�X�`���̓ǂݍ���
//
//�\�񂳂�Ă���t�@�C����ǂݍ��݂܂�
//
//�߂�l:�ǂݍ��߂Ȃ������t�@�C����
//
int Texture_Load(void)
{
	int err_count = 0;

	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return -1;
	}
	
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].filename[0] == 0)
		{
			//��̃e�[�u���Ȃ̂ŃX�L�b�v
			continue;
		}
		if (g_Textures[i].pTexture != NULL)
		{
			//���łɓǂݍ��܂�Ă���
			continue;
		}

		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Textures[i].filename, &g_Textures[i].pTexture);
		if (FAILED(hr))
		{
			//�e�N�X�`���̓ǂݍ��݂Ɏ��s
			DebugPrintf("�ǂݍ��߂Ȃ������e�N�X�`���t�@�C��:[%s]\n", g_Textures[i].filename);
			err_count++;
		}
		else
		{
			//�e�N�X�`���̉𑜓x�i���E�����j�̒���
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Textures[i].filename, &info);
			g_Textures[i].width = info.Width;
			g_Textures[i].height = info.Height;
			DebugPrintf("[%s]���ǂݍ��ݐ���\n", g_Textures[i].filename);
		}
	}

	return err_count; //�ǂݍ��߂Ȃ�������
}

//�e�N�X�`���̕������
//
//�����FtextureIds[]...���������textureId��������int�^�z��̐擪�A�h���X
//		count       ...�������e�N�X�`���̐�
//
void Texture_Release(int textureIds[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (g_Textures[textureIds[i]].pTexture)
		{
			g_Textures[textureIds[i]].pTexture->Release();
			g_Textures[textureIds[i]].pTexture = NULL;
		}
		g_Textures[textureIds[i]].filename[0] = 0;
		g_Textures[textureIds[i]].width  = 0;
		g_Textures[textureIds[i]].height = 0;

	}
}

//�e�N�X�`���̑S���
void Texture_AllRelease(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture)
		{
			g_Textures[i].pTexture->Release();
			g_Textures[i].pTexture = NULL;
		}
		g_Textures[i].filename[0] = 0;
		g_Textures[i].width = 0;
		g_Textures[i].height = 0;

	}
}


//�e�N�X�`���̕��̎擾
//
//�����F�e�N�X�`���ԍ�
//
//�߂�l;�e�N�X�`���̕�
//
unsigned long Texture_GetTextureWidth(int textureId)
{
	return g_Textures[textureId].width;
}

//�e�N�X�`���̍����̎擾
//
//�����F�e�N�X�`���ԍ�
//
//�߂�l;�e�N�X�`���̍���
//
unsigned long Texture_GetTextureHeight(int textureId)
{
	return g_Textures[textureId].height;
}

//�e�N�X�`���C���^�[�t�F�[�X�|�C���^�̎擾
//
//�����F�e�N�X�`���Ǘ��ԍ�
//
//�߂�l:�e�N�X�`���C���^�[�t�F�[�X�|�C���^
//	     �ǂݍ��߂Ă��Ȃ��܂��͕s���ȊǗ��ԍ��������ꍇNULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId)
{
	return g_Textures[textureId].pTexture;
}