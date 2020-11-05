/*================================================================================================

	debugPrintf������Ďg����[debugPrintf.h]
														author:mgc
														date:2020/05/20
 -------------------------------------------------------------------------------------------------

 ===============================================================================================*/
#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#include<Windows.h>
#include<stdio.h>

//�f�o�b�O�p printf...VisualStudio�̏o�̓E�B���h�ɏo�͂����
inline void DebugPrintf(const char* pFormat, ...)
{
	
#if defined(_DEBUG)||defined(DEBUG)
		va_list argp;
		char buf[256];//�o�b�t�@�͑傫�߂Ɏ��܂��傤
		va_start(argp, pFormat);
		vsprintf_s(buf, 256, pFormat, argp); // �o�b�t�@�ʂƑ�Q���������킹�邱��
		va_end(argp);
		// VisualStudio�̏o�̓E�B���h�E�֕������\��
		OutputDebugStringA(buf);
#else 
	UNREFERENCED_PARAMETER(pFormat);
#endif // _DEBUG || DEBUG
}

#endif // _DEBUG_PRINTF_H_