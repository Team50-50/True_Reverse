/*=======================================================================================

	�X�v���C�g����[sprite.cpp]
												author:mgc
												date:2020/07/01
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#include<d3dx9.h>

#include"mydirect3d.h"
#include"texture.h"

 /*-----------------------------------------------------------------------------------------
  �\���̐錾
 -------------------------------------------------------------------------------------------*/
 //���_�\����
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 Position;
	D3DCOLOR	Color;
	D3DXVECTOR2 TexCoord;

}Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//FVF �t���L�V�u���o�[�e�b�N�X�t�H�[�}�b�g
//RHW = 1 ... ���W�ϊ��ςݒ��_

 /*-----------------------------------------------------------------------------------------
  �O���[�o���ϐ�
 -------------------------------------------------------------------------------------------*/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //���_�o�b�t�@�[�C���^�[�t�F�[�X
static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer = NULL;  //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X
static D3DCOLOR g_Color = 0xffffffff;	//�|���S���J���[

 //�X�v���C�g�̏�����
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	pDevice->CreateVertexBuffer(
		sizeof(Vertex2D) * 4, //���_�o�b�t�@�̗�(�o�C�g)
		D3DUSAGE_WRITEONLY,   //�g����
		FVF_VERTEX2D,		  //FVF
		D3DPOOL_MANAGED,	  //�������̊Ǘ��@
		&g_pVertexBuffer,	  //�擾�����C���^�[�t�F�[�X�̃A�h���X���L�^���邽�߂̃|�C���^�ւ̃A�h���X
		NULL
	);

	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,	  
		NULL
	);
}

//�X�v���C�g�̏I������
void Sprite_Finalaize(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}


//�X�v���C�g�|���S���J���[�ݒ�
//
//�����Fcolor
//
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

//�X�v���C�g�̕`��
//*�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
//�����FtextureId...�e�N�X�`���Ǘ��ԍ�
//
//		dx       ...�`����Wx(����w��)
//		dy       ...�`����Wy(����w��)
//
void Sprite_Draw(int textureId, float dx, float dy)
{
	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//�f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//�|���S���̃T�C�Y�̓e�N�X�`���̃T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);


	//���_�f�[�^
	Vertex2D v[] = {  
		{ D3DXVECTOR4(dx     - 0.5f,dy     - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f,dy     - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR4(dx     - 0.5f,dy + h - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR4(dx + w - 0.5f,dy + h - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(1.0f,1.0f) },
	};

	//���_�o�b�t�@�����b�N���ăf�[�^����������
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	//�f�[�^����������
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();
	
	//�C���f�b�N�X�o�b�t�@�����b�N���ăf�[�^����������
	WORD* pI;
	g_pIndexBuffer->Lock(0, 0, (void**)&pI, 0);
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;

	g_pIndexBuffer->Unlock();

	//�f�o�C�X�ɒ��_�o�b�t�@�̗��p��ݒ�
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	//�f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̗��p��ݒ�
	pDevice->SetIndices(g_pIndexBuffer);


	//�|���S���`��(�ȈՔ�)
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
}


//�X�v���C�g�̕`��
//���e�N�X�`���؂���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
//�����FtextureId...�e�N�X�`���Ǘ��ԍ�
//
//		dx       ...�`����Wx(����w��)
//		dy       ...�`����Wy(����w��)
//		tcx      ...�e�N�X�`���؂�����Wx
//		tcy      ...�e�N�X�`���؂�����Wy
//		tcw      ...�e�N�X�`���؂��蕝
//		tch      ...�e�N�X�`���؂��荂��
//
void Sprite_Draw(int textureId, float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//�f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//�|���S���̃T�C�Y�̓e�N�X�`���̃T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//�e�N�X�`���؂���UV���W
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx       - 0.5f,dy       - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dx + tcw - 0.5f,dy       - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(dx       - 0.5f,dy + tch - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dx + tcw - 0.5f,dy + tch - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	//�f�[�^����������
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	//�f�o�C�X�ɗ��p���钸�_�o�b�t�@���w�肷��
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	//�|���S���`��(�ȈՔ�)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}


//�X�v���C�g�̕`��
//���e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
//�����FtextureId...�e�N�X�`���Ǘ��ԍ�
//
//		dx       ...�`����Wx(����w��)
//		dy       ...�`����Wy(����w��)
//		dw		 ...�|���S���̃T�C�Y��
//		dh		 ...�|���S���̃T�C�Y����
//		tcx      ...�e�N�X�`���؂�����Wx
//		tcy      ...�e�N�X�`���؂�����Wy
//		tcw      ...�e�N�X�`���؂��蕝
//		tch      ...�e�N�X�`���؂��荂��
//
void Sprite_Draw(int textureId, float dx, float dy,float dw,float dh, int tcx, int tcy, int tcw, int tch)
{
	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//�f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//�|���S���̃T�C�Y�̓e�N�X�`���̃T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//�e�N�X�`���؂���UV���W
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;


	//���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx      - 0.5f,dy      - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dx + dw - 0.5f,dy      - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(dx      - 0.5f,dy + dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dx + dw - 0.5f,dy + dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	//�|���S���`��(�ȈՔ�)
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}

//�X�v���C�g�̕`��
//����]�@�\�t��
//
//�����FtextureId...�e�N�X�`���Ǘ��ԍ�
//
//		dx       ...�`����Wx(����w��)
//		dy       ...�`����Wy(����w��)
//		dw		 ...�|���S���̃T�C�Y��
//		dh		 ...�|���S���̃T�C�Y����
//		tcx      ...�e�N�X�`���؂�����Wx
//		tcy      ...�e�N�X�`���؂�����Wy
//		tcw      ...�e�N�X�`���؂��蕝
//		tch      ...�e�N�X�`���؂��荂��
//		cx		 ...��]�̒��S���Wx
//		cy		 ...��]�̒��S���Wy
//		angle    ...��]�p�x(���W�A��)
//
void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle)
{
	//Direct3D�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	//�f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//�|���S���̃T�C�Y�̓e�N�X�`���̃T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHeight(textureId);

	//�e�N�X�`���؂���UV���W
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(   - 0.5f,   - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v0) },
		{ D3DXVECTOR4(dw - 0.5f,   - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v0) },
		{ D3DXVECTOR4(   - 0.5f,dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dw - 0.5f,dh - 0.5f,1.0f,1.0f),g_Color, D3DXVECTOR2(u1,v1) },
	};

	//	���s�ړ��s��
	D3DXMATRIX mtxTranslationC;
	D3DXMatrixTranslation(&mtxTranslationC, -cx, -cy, 0.0f);

	D3DXMATRIX mtxTranslationI;
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dy, 0.0f);

	//��]�s��
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation, angle);

	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);


	//�s��̍���
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxRotation * mtxTranslationI;

	//���W�ϊ�
	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].Position, &v[i].Position, &mtxWorld);
	}
		


	//�|���S���`��(�ȈՔ�)
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
	//D3DPT_TRIANGLELIST
	//D3DPT_TRIANGLESTRIP
	//D3DPT_POINTLIST
	//D3DPT_LINELIST
	//D3DPT_LINESTRIP
	//D3DPT_TRIANGLEFAN
}