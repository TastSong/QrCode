// stdafx.cpp : source file that includes just the standard includes
// QRGenerator.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
// #include <string.h>
// #include <errno.h>
// #include <conio.h>
// #include <ctype.h>
// #include <stdio.h>
// #include <stddef.h>
// #include <stdlib.h>
// #include <wchar.h>
#include <windows.h>
#include "qrencode.h"
#include "../qrlib/lib.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#define OUT_FILE					"code.bmp"								// Output file name
#define OUT_FILE_PIXEL_PRESCALER	8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0											// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0

// BMP defines

#define BI_RGB			0L

#pragma pack(push, 2)

#pragma pack(pop)
typedef struct 
{
	BYTE b;
	BYTE g;
	BYTE r;
}RGB;
int QRcodeRest(char* str)
{
	char*			szSourceSring = str;
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*	pRGBData, *pSourceData, *pDestData;
	QRcode*			pQRC;
	FILE*			f;
	// Compute QRCode

	if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

		// Allocate pixels buffer

		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			return 0;
		}

		// Preset to white

		memset(pRGBData, 0xff, unDataBytes);


		// Prepare bmp headers

		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER)+
			sizeof(BITMAPINFOHEADER)+
			unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+
			sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
		kInfoHeader.biHeight = ((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = (kInfoHeader.biWidth*kInfoHeader.biBitCount+31)/32*4*kInfoHeader.biHeight;
		kInfoHeader.biXPelsPerMeter = 4000;
		kInfoHeader.biYPelsPerMeter = 4000;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

		// Convert QrCode bits to bmp pixels

		pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
					{
						for (n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
						{
							*(pDestData + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
						}
					}
				}
				pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
				pSourceData++;
			}
		}
		// Output the bmp file

		if ((!fopen_s(&f, OUT_FILE, "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);

			fclose(f);
		}
		else
		{
			return 0;
		}

		// Free data

		free(pRGBData);
		QRcode_free(pQRC);
	}
	else
	{
		return 0;
	}

	return 0;

}