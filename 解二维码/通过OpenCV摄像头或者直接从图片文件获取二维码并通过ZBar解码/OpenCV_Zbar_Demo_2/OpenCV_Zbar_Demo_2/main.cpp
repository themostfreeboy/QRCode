#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "MyOpenCV_ZBar.h"

int main()
{
	printf("��ΪJXL��д�Ķ�ά��ɨ�������ѡ��ģʽ��\n1��ͨ������ͷɨ���ά�룻\n2��ͨ������ͼƬɨ���ά�롣\n");
	int choice=0;
	scanf("%d",&choice);
	wstring* result = new wstring[10000];
	if(choice==1)//ͨ������ͷɨ���ά��
	{
		My_QRCode_Decode_By_OpenCV(result);
		MessageBoxW(NULL, (*result).c_str(), _T("Decode result"), MB_OK);
	}
	else if(choice==2)//ͨ������ͼƬɨ���ά��
	{
		printf("�뽫ͼƬֱ����ק���˿���̨���ڼ���\n");
		char filename[100];
		scanf_s("%s",filename,100);
		My_QRCode_Decode_By_Image(filename,result);
		MessageBoxW(NULL, (*result).c_str(), _T("Decode result"), MB_OK);
	}
	else//��������
	{
		printf("�����������\n");
	}
	delete[] result;
	system("pause");
	return 0;
}