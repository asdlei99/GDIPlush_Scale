/*
说明：使用GDIPlush实现裁剪.
GdiplusStartup.
GdiplusShutdown.
*/

#include<stdio.h>
#include<Windows.h>
#include <GdiPlus.h>

using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

void GdiPlushScale(char *in_chsrc,int in_iSrcBit, 
	int in_iSrcW,int in_iSrcH,char *in_chDest,int in_iDestBit,int in_iDestW,int in_iDestH)
{
	PixelFormat iSrcFormat;
	
	if (in_iSrcBit == 24)
	{
		iSrcFormat = PixelFormat24bppRGB;
	}
	else
	{
		iSrcFormat = PixelFormat32bppARGB;
	}

	PixelFormat iDestFormat;

	if (in_iDestBit == 24)
	{
		iDestFormat = PixelFormat24bppRGB;
	}
	else 
	{
		iDestFormat = PixelFormat32bppARGB;
	}

	int iStride = in_iSrcW *(in_iSrcBit / 8);

	Bitmap oSrcImage(in_iSrcW, in_iSrcH, iStride, iSrcFormat,(BYTE *)in_chsrc);
	if (oSrcImage.GetWidth() <= 0)
	{
		printf("Src Faile\n");
	}

	Bitmap oDestImage(in_iDestW, in_iDestH, iDestFormat);
	if (oDestImage.GetWidth() <= 0)
	{
		printf("Dest Failed\n");
	}

	Graphics *pGraphics = Graphics::FromImage(&oDestImage);
	if (pGraphics == NULL)
	{
		printf("Graphics Failed\n");
	}

	pGraphics->SetInterpolationMode(InterpolationModeDefault);
	
	pGraphics->DrawImage(&oSrcImage, 0, 0, in_iDestW, in_iDestH);
	delete pGraphics;

	BitmapData data;

	data.Stride = in_iDestW * (in_iDestBit / 8);
	data.Scan0 = (VOID *)in_chDest;

	Rect reRect(0, 0, in_iDestW, in_iDestH);
	
	Status sResult = oDestImage.LockBits(&reRect, ImageLockModeRead | ImageLockModeWrite | ImageLockModeUserInputBuf, iDestFormat, &data);
	if (sResult)
	{
		printf("LockBits Failed\n");
	}

	oDestImage.UnlockBits(&data);

}
//保存当前视频图像为bmp文件
bool SaveBmpFile(short sFrameWidth, short sFrameHeight, short sColorSpace, const char *pBmpData, char* pFileName)
{
	
	FILE* lpFilePtr = NULL;
	//char lszBmpFileName[512+128];
	BITMAPFILEHEADER lBmpFileHeader;
	BITMAPINFOHEADER lBmpInfoHeader;	//绘制视频图像用到的位图信息
	int liBmpFileSize = 0;
	int nSizeImage = sFrameWidth * sFrameHeight * sColorSpace / 8;

	//得到BMP文件大小
	liBmpFileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nSizeImage;

	//填充BMP文件头
	lBmpFileHeader.bfType = 19778;
	lBmpFileHeader.bfSize = liBmpFileSize;
	lBmpFileHeader.bfReserved1 = 0;
	lBmpFileHeader.bfReserved2 = 0;
	lBmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BMP信息头
	lBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	lBmpInfoHeader.biPlanes = 1;
	lBmpInfoHeader.biCompression = BI_RGB;
	lBmpInfoHeader.biXPelsPerMeter = lBmpInfoHeader.biYPelsPerMeter = 0;
	lBmpInfoHeader.biClrImportant = lBmpInfoHeader.biClrUsed = 0;
	lBmpInfoHeader.biSizeImage = nSizeImage;
	lBmpInfoHeader.biWidth = sFrameWidth;
	lBmpInfoHeader.biHeight = sFrameHeight;
	lBmpInfoHeader.biBitCount = sColorSpace;

	//创建、写入和关闭BMP文件
	fopen_s(&lpFilePtr,pFileName, "wb+");
	if (lpFilePtr)
	{

		fwrite(&lBmpFileHeader, sizeof(char), sizeof(BITMAPFILEHEADER), lpFilePtr);
		fwrite(&lBmpInfoHeader, sizeof(char), sizeof(BITMAPINFOHEADER), lpFilePtr);
		fwrite(pBmpData, sizeof(char), nSizeImage, lpFilePtr);
		fclose(lpFilePtr);
	}
	return true;
}
int main(int argc, char *argv[])
{
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	FILE *pFSrc = NULL;
	FILE *pFDest = NULL;

	char *ch_Src = new  char[736 * 416 *3 + 1];
	char *ch_Dest = new char[320 *240 *3 + 1];

	errno_t er = fopen_s(&pFSrc, "c:\\quanwei\\BMP\\123.bmp", "rb+");
	if (er != 0)
	{
		printf("fopen Src Failed\n");
	}
	fseek(pFSrc,54,SEEK_SET);

	fread(ch_Src, 1, 736 * 416 * 3, pFSrc);

	GdiPlushScale(ch_Src,24,736,416,ch_Dest,24,320,240);

	SaveBmpFile(320, 240, 24, ch_Dest, "c:\\quanwei\\BMP\\11.bmp");

	fclose(pFSrc);

	delete[] ch_Src;
	delete[] ch_Dest;

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}