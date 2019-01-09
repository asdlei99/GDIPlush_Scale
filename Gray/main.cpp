/*
说明：图像灰度化
方法：选取RGB三色素的平均值进行赋值.
*/
#include<stdio.h>

void Gray(char *in_chSrc, int in_iW, int in_iH,int in_iB)
{
	int iB = in_iB / 8;
	int itemp = 0;
	int iStride = in_iW *iB;

	for (int i = 0; i < in_iH; i++)
	{
		for (int j = 0; j < in_iW; j++)
		{
			itemp = (in_chSrc[i*iStride + j*iB] + in_chSrc[i*iStride + j*iB + 1] + in_chSrc[i*iStride + j*iB + 2]) / 3;

			in_chSrc[i*iStride + j*iB] = itemp & 0xff;
			in_chSrc[i*iStride + j*iB + 1] = itemp & 0xff;
			in_chSrc[i*iStride + j*iB + 2] = itemp & 0xff;

		}
	}
}
int main(int argc, char *argv[])
{
	FILE *pFile = NULL;
	FILE *pGray = NULL;

	errno_t er = fopen_s(&pFile, "C:\\quanwei\\BMP\\1.bmp", "rb+");
	if (er != 0)
	{
		printf("pFile fopen_s failed\n");
	}
	
	er = fopen_s(&pGray, "c:\\quanwei\\BMP\\Gray.bmp", "wb+");
	if (er != 0)
	{
		printf("pGray fopen_s failed\n");
	}

	char *ch_Head = new char[54];
	fread(ch_Head, 1, 54, pFile);

	fseek(pFile, 54, SEEK_SET);

	char *ch_buf = new char[228 * 141 *4];//32bit

	fread(ch_buf, 1, 228 * 141 * 4, pFile);

	Gray(ch_buf, 228, 141, 32);

	fwrite(ch_Head, 1, 54, pGray);
	fwrite(ch_buf, 1, 228 * 141 * 4, pGray);

	fclose(pFile);
	fclose(pGray);

	delete[] ch_Head;
	delete[] ch_buf;
}