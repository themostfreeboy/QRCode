#include "MyOpenCV_ZBar.h"

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

int My_QRCode_Decode_By_OpenCV(wstring* result)//直接通过OpenCV调用摄像头，扫描二维码图片，并从中解析出二维码内容，将内容存到result中(返回值=0失败;返回值=1成功)
{
	//在当前目录创建cache文件夹
	CString strFolderPath(".\\cache");
	if (!PathIsDirectory(strFolderPath))//判断路径是否存在
	{

		//设置属性
		SECURITY_ATTRIBUTES attribute;
		attribute.nLength = sizeof(attribute);
		attribute.lpSecurityDescriptor = NULL;
		attribute.bInheritHandle = FALSE;

		CreateDirectory(strFolderPath, &attribute);
	}

	int n=0;//n=0失败;n=1成功
	cvNamedWindow("Catching",CV_WINDOW_AUTOSIZE);
	IplImage* pFrame = NULL;//声明IplImage指针
    CvCapture* pCapture = cvCreateCameraCapture(0);//获取摄像头
    while(n==0)  
    {
		
	    pFrame=cvQueryFrame(pCapture);        
        if(!pFrame)
		{
			RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
			return 0;
		}
	    cvSaveImage(".\\cache\\temp1.jpg",pFrame);
	    IplImage *cv_IplImage_matrix = cvLoadImage(".\\cache\\temp1.jpg",CV_LOAD_IMAGE_COLOR);
	    cvShowImage("Catching",cv_IplImage_matrix);
        cvWaitKey(33); 

		//检测".\\cache\\temp1.jpg"文件是否存在
	    errno_t err_read;
	    FILE *fp_read = NULL;
	    err_read = fopen_s(&fp_read, ".\\cache\\temp1.jpg", "rb");
	    if (err_read != 0)
	    {
		    continue;//".\\cache\\temp1.jpg"文件不存在
	    }
	    fclose(fp_read);

	    //".\\cache\\temp1.jpg"文件存在
        IplImage* img =cvLoadImage(".\\cache\\temp1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        CvMat *image = cvCreateMat( img->height, img->width, CV_8UC1 );
        cvConvert( img, image );
        IplImage* img2=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
        CvMat *dst = cvCreateMat( img2->height, img2->width, CV_8UC1 );
        cvConvert( img2, dst );
        cvThreshold(image,dst,150,255,CV_THRESH_BINARY);
        cvSaveImage(".\\cache\\temp2.jpg",dst);
	    zbar_image_scanner_t *scanner = zbar_image_scanner_create();
	    zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);//配置ZBar
	    int width = 0, height = 0;
	    const void *raw = NULL;
 	    CvMat *cv_matrix = cvLoadImageM(".\\cache\\temp2.jpg",CV_LOAD_IMAGE_GRAYSCALE);//如果使用 Magick++ 库，只是在解析图片数据的位置有点差别
	    width = cv_matrix->width;//获取图像信息
	    height= cv_matrix->height;//获取图像信息
	    raw = (char*)cv_matrix->data.ptr;
	    Image image_z(width, height, "Y800", raw, width * height);//处理图像数据
	    //开始扫面图像中的二维码
        n = zbar_scan_image(scanner, image_z);//n=0失败;n=1成功
	    //提取结果
	    const zbar_symbol_t *symbol = zbar_image_first_symbol(image_z);
	    for(; symbol; symbol = zbar_symbol_next(symbol)) 
	    {
		    zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		    const char *data = zbar_symbol_get_data(symbol); 
		    CString temp = data;
		    LPCWSTR app = (LPCWSTR)(temp.AllocSysString());
		    *result = TranslateString(app);
        }
        image_z.set_data(NULL, 0);//清除数据
	    cvReleaseMat(&cv_matrix);//释放资源
		cvReleaseImage(&cv_IplImage_matrix);
	}
	cvReleaseCapture(&pCapture);
	cvDestroyWindow("Catching");
	DeleteFile(_T(".\\cache\\temp1.jpg"));//删除临时文件temp1.jpg
	DeleteFile(_T(".\\cache\\temp2.jpg"));//删除临时文件temp2.jpg
	RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
	return n;//n=0失败;n=1成功
}   


 
int My_QRCode_Decode_By_Image(const char* filename, wstring* result)//直接从filename二维码图片中解析出二维码内容，将内容存到result中(返回值=0失败;返回值=1成功)
{
	 //检测filename文件是否存在
	 errno_t err_read;
	 FILE *fp_read = NULL;
	 err_read = fopen_s(&fp_read, filename, "rb");
	 if (err_read != 0)
	 {
		 return 0;//filename文件不存在
	 }
	 fclose(fp_read);

	 //filename文件存在
     IplImage* img =cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);
     CvMat *image = cvCreateMat( img->height, img->width, CV_8UC1 );
     cvConvert( img, image );
     IplImage* img2=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
     CvMat *dst = cvCreateMat( img2->height, img2->width, CV_8UC1 );
     cvConvert( img2, dst );
     cvThreshold(image,dst,150,255,CV_THRESH_BINARY);
     cvSaveImage(filename,dst);
     cvWaitKey(0);
	 zbar_image_scanner_t *scanner = zbar_image_scanner_create();
	 zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);//配置ZBar
	 int width = 0, height = 0;
	 const void *raw = NULL;
 	 CvMat *cv_matrix = cvLoadImageM(filename,CV_LOAD_IMAGE_GRAYSCALE);//如果使用 Magick++ 库，只是在解析图片数据的位置有点差别
	 width = cv_matrix->width;//获取图像信息
	 height= cv_matrix->height;//获取图像信息
	 raw = (char*)cv_matrix->data.ptr;
	 Image image_z(width, height, "Y800", raw, width * height);//处理图像数据
	 //开始扫面图像中的二维码
     int n = zbar_scan_image(scanner, image_z);//n=0失败;n=1成功
	 //提取结果
	 const zbar_symbol_t *symbol = zbar_image_first_symbol(image_z);
	 for(; symbol; symbol = zbar_symbol_next(symbol)) 
	 {
		 zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		 const char *data = zbar_symbol_get_data(symbol); 
		 CString temp = data;
		 LPCWSTR app = (LPCWSTR)(temp.AllocSysString());
		 *result = TranslateString(app);
     }
     image_z.set_data(NULL, 0);//清除数据
	 cvWaitKey(0);
	 cvReleaseMat(&cv_matrix);//释放资源
	 return n;//n=0失败;n=1成功
}