#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "MyOpenCV_ZBar.h"

int main()
{
	printf("此为JXL编写的二维码扫描程序，请选择模式：\n1、通过摄像头扫描二维码；\n2、通过现有图片扫描二维码。\n");
	int choice=0;
	scanf("%d",&choice);
	wstring* result = new wstring[10000];
	if(choice==1)//通过摄像头扫描二维码
	{
		My_QRCode_Decode_By_OpenCV(result);
		MessageBoxW(NULL, (*result).c_str(), _T("Decode result"), MB_OK);
	}
	else if(choice==2)//通过现有图片扫描二维码
	{
		printf("请将图片直接推拽到此控制台框内即可\n");
		char filename[100];
		scanf_s("%s",filename,100);
		My_QRCode_Decode_By_Image(filename,result);
		MessageBoxW(NULL, (*result).c_str(), _T("Decode result"), MB_OK);
	}
	else//输入有误
	{
		printf("你的输入有误。\n");
	}
	delete[] result;
	system("pause");
	return 0;
}