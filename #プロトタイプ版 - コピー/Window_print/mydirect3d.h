/*=======================================================================================

	 Direct3D����[mydirect3d.h]
												author:mgc
												date:2020/06/17
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef MY_DIRECT3D_H
#define MY_DIRECT3D_H

#include<d3d9.h>
//Direct3D�֘A�̏�����
//
//�߂�l:�������Ɏ��s������false
//
bool MyDirect3D_Initialize(HWND hWnd);

//Direct3D�֘A�̏I������
//
void MyDirect3D_Finalize(void);

//Direct3D�f�o�C�X�̎擾
//
//�߂�l:Direct3D�f�o�C�X�ւ̃|�C���^
//
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif // MY_DIRECT3D_H
