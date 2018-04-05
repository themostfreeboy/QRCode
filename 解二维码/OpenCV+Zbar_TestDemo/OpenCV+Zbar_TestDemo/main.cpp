#include <opencv2/opencv.hpp>
#include "zbar.h"
#include <iostream>
#include <afx.h>
#include <shlwapi.h>
//#include <stdio.h>
//#include <stdlib.h>

 
#pragma once

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。

// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
// WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。

#include <SDKDDKVer.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shlwapi.lib")
 
 
#define IS_NUMBER(c)        ((c) && (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')))
 
using namespace std;
using namespace cv;
using namespace zbar;

wstring TranslateString(LPCWSTR wszOrig);
 

   
int main( int argc, char** argv )  
{
	//声明IplImage指针
    IplImage* pFrame = NULL;

    //获取摄像头
    CvCapture* pCapture = cvCreateCameraCapture(0);  

    pFrame=cvQueryFrame(pCapture);
    cvSaveImage("D:\\二维码\\摄像头.jpg",pFrame);

    //显示视屏  
    while(1)  
    {
	    pFrame=cvQueryFrame(pCapture);        
        if(!pFrame) break;
	    cvSaveImage("D:\\二维码\\摄像头.jpg",pFrame);
	    cvNamedWindow("Example",CV_WINDOW_AUTOSIZE);
	    IplImage *cv_matrix = cvLoadImage("D:\\二维码\\摄像头.jpg",CV_LOAD_IMAGE_COLOR);
	    cvShowImage("Example",cv_matrix);
        char KeyValue=cvWaitKey(33);  
        if(KeyValue==13) break;//回车ENTER键
	    cvReleaseImage(&cv_matrix);
     }
     cvReleaseCapture(&pCapture);  
     cvDestroyWindow("Example");

     IplImage* img =cvLoadImage("D:\\二维码\\摄像头.jpg",CV_LOAD_IMAGE_GRAYSCALE);
     CvMat *image = cvCreateMat( img->height, img->width, CV_8UC1 );
     cvConvert( img, image );
     IplImage* img2=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
     CvMat *dst = cvCreateMat( img2->height, img2->width, CV_8UC1 );
     cvConvert( img2, dst );
     cvThreshold(image,dst,150,255,CV_THRESH_BINARY);
     cvSaveImage("D:\\二维码\\摄像头2.jpg",dst);
     cvWaitKey(0);

	 char * str;
	 str="D:\\二维码\\摄像头2.jpg";
	 zbar_image_scanner_t *scanner = zbar_image_scanner_create();

   	/* configure the reader */
	 zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	 /* obtain image data */
	 int width = 0, height = 0;
	 const void *raw = NULL;

     //如果使用 Magick++ 库，只是在解析图片数据的位置有点差别
	 CvMat *cv_matrix = cvLoadImageM(str,CV_LOAD_IMAGE_GRAYSCALE);
	 cvNamedWindow("Example1",CV_WINDOW_AUTOSIZE);
	 cvShowImage("Example1",cv_matrix);

	 width = cv_matrix->width;
	 height= cv_matrix->height;
	 raw = (char*)cv_matrix->data.ptr;

	 /* wrap image data */
	
	 Image image_z(width, height, "Y800", raw, width * height);

	 /*zbar_image_t *image = zbar_image_create();
	 zbar_image_set_format(image, *(int*)"Y800");
	 zbar_image_set_size(image, width, height);
	 zbar_image_set_data(image, raw, width * height, zbar_image_free_data);*/

	 /* scan the image for barcodes */
     int n = zbar_scan_image(scanner, image_z); //n == 0 is failed

	 /* extract results */
	 const zbar_symbol_t *symbol = zbar_image_first_symbol(image_z); 
	 for(; symbol; symbol = zbar_symbol_next(symbol))//Image::SymbolIterator symbol = image.symbol_begin();symbol != image.symbol_end();++symbol) 
	{
		// do something useful with results
		/*cout << "decoded " << symbol->get_type_name()
				 << " symbol \"" <<  symbol->get_data() << '"' << endl;*/
		zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		const char *data = zbar_symbol_get_data(symbol); 
		
		cout << data <<endl;

		CString temp = data;
		LPCWSTR app = (LPCWSTR)(temp.AllocSysString());
		cout << "decoded " << typ << endl;
		wstring wstr = TranslateString(app).c_str();
		wcout.imbue(locale("chs"));
		wcout << wstr << endl;
		MessageBoxW(NULL, TranslateString(app).c_str(), _T("Decode result"), MB_OK);

		//const std::string& result = symbol->get_data();
        }
        // clean up
        image_z.set_data(NULL, 0);

	    /*WCHAR *d;
	    int nd = 0;
	    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
	    for(; symbol; symbol = zbar_symbol_next(symbol)) 
	    {
		      /* do something useful with results */
	          /*	zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		      memcpy(info.type,zbar_get_symbol_name(typ),sizeof(info.type) - 1);

		      const char *data = zbar_symbol_get_data(symbol);
		      unsigned int len = zbar_symbol_get_data_length(symbol);
		      printf("decoded %s symbol \"%s\"\n", zbar_get_symbol_name(typ), data);
		      nd = utf8_to_utf16((BYTE *)data,len,NULL,0); //utf-8 转成 unicode 需要多少字节
		      d = (WCHAR *)malloc((nd * 2) + 2);
		      utf8_to_utf16((BYTE *)data,len,d,nd); //utf-8 转成 unicode
		      memset(info.data,0,sizeof(info.data));
		      WideCharToMultiByte(CP_ACP,0,d,nd,(CHAR *)info.data,sizeof(info.data) - 1,NULL,NULL); //unicode 转 ascii
		      free(d);
	      }
	      //return n;*/
	      cout<< n <<endl;

	      cvWaitKey(0);
	      cvReleaseMat(&cv_matrix);
	      cvDestroyWindow("Example1");
	      return(0);
}   


 
wstring TranslateString(LPCWSTR wszOrig)
{
    wstring ret;
    LPCWSTR curChar = wszOrig;
 
    while (*curChar)
    {
        if (curChar[0] == L'\\' &&
            (curChar[1] == L'u' || curChar[1] == L'U') &&
            IS_NUMBER(curChar[2]) &&
            IS_NUMBER(curChar[3]) &&
            IS_NUMBER(curChar[4]) &&
            IS_NUMBER(curChar[5]))
        {
            wchar_t hex[8] = {'0', 'x'};
            hex[2] = curChar[2];
            hex[3] = curChar[3];
            hex[4] = curChar[4];
            hex[5] = curChar[5];
 
            int i;
            StrToIntExW(hex, STIF_SUPPORT_HEX, &i);
            ret += (wchar_t)i;
 
            curChar += 6;
        }
        else
        {
            ret += (wchar_t)*curChar;
            curChar++;
        }
    }
    return ret;
}