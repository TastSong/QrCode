// QrCodeGeneration.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "./qrlib/lib.h"
#include <windows.h>
#pragma comment(lib,"./Debug/qrlib.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	printf("%d", QRcodeRest("TastSong"));
	HBITMAP  hb;
	hb = (HBITMAP)LoadImageA(NULL, "code.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hb == 0)
	{
		printf("error\n");
	}
	system("pause");
	return 0;
}