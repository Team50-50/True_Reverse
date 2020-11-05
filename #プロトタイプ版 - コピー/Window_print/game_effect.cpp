/*=======================================================================================

	 �Q�[���̃G�t�F�N�g����[game_effect.cpp]
												author:mgc
												date:2020/09/10
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include"texture.h"
#include"sprite.h"
#include<d3dx9.h>
#include"mydirect3d.h"

/*------------------------------------------------------------------------------------------
   �萔�錾
-------------------------------------------------------------------------------------------*/
#define EFFECT_MAX (2048)

/*------------------------------------------------------------------------------------------
   �\���̐錾
-------------------------------------------------------------------------------------------*/
//�G�t�F�N�g�\����
typedef struct Effect_tag
{
	D3DXVECTOR2 position;
	//�F
	//�T�C�Y
	//�����x
	//���
	//�����t���[����
	int birthFrame;
	bool enable;
}Effect;

/*------------------------------------------------------------------------------------------
   �O���[�o���ϐ�
-------------------------------------------------------------------------------------------*/
static int g_TextureId = TEXTURE_INVALID_ID;
static Effect g_Effects[EFFECT_MAX];
static int g_FrameCount = 0;


/*------------------------------------------------------------------------------------------
   �֐���`
-------------------------------------------------------------------------------------------*/
 //�G�t�F�N�g�̏�����
void GameEffect_Initialize(void)
{
	g_TextureId = Texture_SetTextureLoadFile("asset/effect000.jpg");

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effects[i].enable = false;
	}

	g_FrameCount = 0;
}

//�G�t�F�N�g�̏I������
void GameEffect_Finalize(void)
{
	Texture_Release(&g_TextureId, 1);
}

//�G�t�F�N�g�̍X�V����
void GameEffect_Update(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (!g_Effects[i].enable) continue;

		int age = g_FrameCount - g_Effects[i].birthFrame;

		if (age >= 60)
		{
			g_Effects[i].enable = false;
		}

	}

	g_FrameCount++;
}

//�G�t�F�N�g�̕`�揈��
void GameEffect_Draw(void)
{
	//���Z�����̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);      //�u�����f�B���O����(�f�t�H���g)
	//g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			 //���f�X�e�B�l�[�V�����J���[�̎w��(1.0)

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (!g_Effects[i].enable) continue;

		int age = g_FrameCount - g_Effects[i].birthFrame;

		float a = 1.0f - ((1.0f / 60.0f) * age);

		D3DXCOLOR color(0.5f, 1.0f, 1.0f, a);
		// �����Z�����̏ꍇ R * a, G * a, B * a�����

		Sprite_SetColor(color);

		Sprite_Draw(g_TextureId, g_Effects[i].position.x, g_Effects[i].position.y);

	}

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//���̃��u�����h�ɖ߂�
	//g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);      //�u�����f�B���O����(�f�t�H���g)
	//g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	 //���f�X�e�B�l�[�V�����J���[�̎w��
}

//�G�t�F�N�g�̐���
void GameEffect_Spawn(float x, float y)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effects[i].enable) continue;

		g_Effects[i].enable = true;
		g_Effects[i].position = D3DXVECTOR2(x, y);
		g_Effects[i].birthFrame = g_FrameCount;


		break;
	}
}

