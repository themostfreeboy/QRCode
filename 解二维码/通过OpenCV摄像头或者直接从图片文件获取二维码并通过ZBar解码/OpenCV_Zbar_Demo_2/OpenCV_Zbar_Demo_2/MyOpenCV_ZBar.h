#pragma once

#ifndef __MYOPENCV_ZBAR_H
#define __MYOPENCV_ZBAR_H

#include "zbar.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <afx.h>
#include <shlwapi.h>
#include <stdio.h>
#include <stdlib.h>

// ���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨��
// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
// WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��

#include <SDKDDKVer.h>

using namespace std;
using namespace cv;
using namespace zbar;

#define IS_NUMBER(c)        ((c) && (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')))

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shlwapi.lib")

wstring TranslateString(LPCWSTR wszOrig);
int My_QRCode_Decode_By_OpenCV(wstring* result);//ֱ��ͨ��OpenCV��������ͷ��ɨ���ά��ͼƬ�������н�������ά�����ݣ������ݴ浽result��(����ֵ=0ʧ��;����ֵ=1�ɹ�)
int My_QRCode_Decode_By_Image(const char* filename, wstring* result);//ֱ�Ӵ�filename��ά��ͼƬ�н�������ά�����ݣ������ݴ浽result��(����ֵ=0ʧ��;����ֵ=1�ɹ�)

#endif