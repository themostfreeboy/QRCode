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

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。
// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
// WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。

#include <SDKDDKVer.h>

using namespace std;
using namespace cv;
using namespace zbar;

#define IS_NUMBER(c)        ((c) && (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')))

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shlwapi.lib")

wstring TranslateString(LPCWSTR wszOrig);
int My_QRCode_Decode_By_OpenCV(wstring* result);//直接通过OpenCV调用摄像头，扫描二维码图片，并从中解析出二维码内容，将内容存到result中(返回值=0失败;返回值=1成功)
int My_QRCode_Decode_By_Image(const char* filename, wstring* result);//直接从filename二维码图片中解析出二维码内容，将内容存到result中(返回值=0失败;返回值=1成功)

#endif