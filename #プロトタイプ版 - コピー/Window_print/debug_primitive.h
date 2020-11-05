/*=======================================================================================

	 �f�o�b�N�p�}�`�\�����W���[��[debug_primitive.h]
												author:mgc
												date:2020/09/25
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#ifndef DEBUG_PRIMITIVE_H_
#define DEBUG_PRIMITIVE_H_

void DebugPrimitive_Initialize(void);
void DebugPrimitive_Finalize(void);
void DebugPrimitive_BatchBegin(void);
void DebugPrimitive_BatchDrawCircle(float x, float y, float radius);
void DebugPrimitive_BatchRun(void);

#endif // !DEBUG_PRIMITIVE_H_
