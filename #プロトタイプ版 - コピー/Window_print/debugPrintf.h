/*================================================================================================

	debugPrintfを作って使おう[debugPrintf.h]
														author:mgc
														date:2020/05/20
 -------------------------------------------------------------------------------------------------

 ===============================================================================================*/
#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#include<Windows.h>
#include<stdio.h>

//デバッグ用 printf...VisualStudioの出力ウィンドに出力される
inline void DebugPrintf(const char* pFormat, ...)
{
	
#if defined(_DEBUG)||defined(DEBUG)
		va_list argp;
		char buf[256];//バッファは大きめに取りましょう
		va_start(argp, pFormat);
		vsprintf_s(buf, 256, pFormat, argp); // バッファ量と第２引数を合わせること
		va_end(argp);
		// VisualStudioの出力ウィンドウへ文字列を表示
		OutputDebugStringA(buf);
#else 
	UNREFERENCED_PARAMETER(pFormat);
#endif // _DEBUG || DEBUG
}

#endif // _DEBUG_PRINTF_H_