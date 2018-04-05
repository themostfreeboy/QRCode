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

int My_QRCode_Decode_By_OpenCV(wstring* result)//ֱ��ͨ��OpenCV��������ͷ��ɨ���ά��ͼƬ�������н�������ά�����ݣ������ݴ浽result��(����ֵ=0ʧ��;����ֵ=1�ɹ�)
{
	//�ڵ�ǰĿ¼����cache�ļ���
	CString strFolderPath(".\\cache");
	if (!PathIsDirectory(strFolderPath))//�ж�·���Ƿ����
	{

		//��������
		SECURITY_ATTRIBUTES attribute;
		attribute.nLength = sizeof(attribute);
		attribute.lpSecurityDescriptor = NULL;
		attribute.bInheritHandle = FALSE;

		CreateDirectory(strFolderPath, &attribute);
	}

	int n=0;//n=0ʧ��;n=1�ɹ�
	cvNamedWindow("Catching",CV_WINDOW_AUTOSIZE);
	IplImage* pFrame = NULL;//����IplImageָ��
    CvCapture* pCapture = cvCreateCameraCapture(0);//��ȡ����ͷ
    while(n==0)  
    {
		
	    pFrame=cvQueryFrame(pCapture);        
        if(!pFrame)
		{
			RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
			return 0;
		}
	    cvSaveImage(".\\cache\\temp1.jpg",pFrame);
	    IplImage *cv_IplImage_matrix = cvLoadImage(".\\cache\\temp1.jpg",CV_LOAD_IMAGE_COLOR);
	    cvShowImage("Catching",cv_IplImage_matrix);
        cvWaitKey(33); 

		//���".\\cache\\temp1.jpg"�ļ��Ƿ����
	    errno_t err_read;
	    FILE *fp_read = NULL;
	    err_read = fopen_s(&fp_read, ".\\cache\\temp1.jpg", "rb");
	    if (err_read != 0)
	    {
		    continue;//".\\cache\\temp1.jpg"�ļ�������
	    }
	    fclose(fp_read);

	    //".\\cache\\temp1.jpg"�ļ�����
        IplImage* img =cvLoadImage(".\\cache\\temp1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        CvMat *image = cvCreateMat( img->height, img->width, CV_8UC1 );
        cvConvert( img, image );
        IplImage* img2=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
        CvMat *dst = cvCreateMat( img2->height, img2->width, CV_8UC1 );
        cvConvert( img2, dst );
        cvThreshold(image,dst,150,255,CV_THRESH_BINARY);
        cvSaveImage(".\\cache\\temp2.jpg",dst);
	    zbar_image_scanner_t *scanner = zbar_image_scanner_create();
	    zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);//����ZBar
	    int width = 0, height = 0;
	    const void *raw = NULL;
 	    CvMat *cv_matrix = cvLoadImageM(".\\cache\\temp2.jpg",CV_LOAD_IMAGE_GRAYSCALE);//���ʹ�� Magick++ �⣬ֻ���ڽ���ͼƬ���ݵ�λ���е���
	    width = cv_matrix->width;//��ȡͼ����Ϣ
	    height= cv_matrix->height;//��ȡͼ����Ϣ
	    raw = (char*)cv_matrix->data.ptr;
	    Image image_z(width, height, "Y800", raw, width * height);//����ͼ������
	    //��ʼɨ��ͼ���еĶ�ά��
        n = zbar_scan_image(scanner, image_z);//n=0ʧ��;n=1�ɹ�
	    //��ȡ���
	    const zbar_symbol_t *symbol = zbar_image_first_symbol(image_z);
	    for(; symbol; symbol = zbar_symbol_next(symbol)) 
	    {
		    zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		    const char *data = zbar_symbol_get_data(symbol); 
		    CString temp = data;
		    LPCWSTR app = (LPCWSTR)(temp.AllocSysString());
		    *result = TranslateString(app);
        }
        image_z.set_data(NULL, 0);//�������
	    cvReleaseMat(&cv_matrix);//�ͷ���Դ
		cvReleaseImage(&cv_IplImage_matrix);
	}
	cvReleaseCapture(&pCapture);
	cvDestroyWindow("Catching");
	DeleteFile(_T(".\\cache\\temp1.jpg"));//ɾ����ʱ�ļ�temp1.jpg
	DeleteFile(_T(".\\cache\\temp2.jpg"));//ɾ����ʱ�ļ�temp2.jpg
	RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
	return n;//n=0ʧ��;n=1�ɹ�
}   


 
int My_QRCode_Decode_By_Image(const char* filename, wstring* result)//ֱ�Ӵ�filename��ά��ͼƬ�н�������ά�����ݣ������ݴ浽result��(����ֵ=0ʧ��;����ֵ=1�ɹ�)
{
	 //���filename�ļ��Ƿ����
	 errno_t err_read;
	 FILE *fp_read = NULL;
	 err_read = fopen_s(&fp_read, filename, "rb");
	 if (err_read != 0)
	 {
		 return 0;//filename�ļ�������
	 }
	 fclose(fp_read);

	 //filename�ļ�����
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
	 zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);//����ZBar
	 int width = 0, height = 0;
	 const void *raw = NULL;
 	 CvMat *cv_matrix = cvLoadImageM(filename,CV_LOAD_IMAGE_GRAYSCALE);//���ʹ�� Magick++ �⣬ֻ���ڽ���ͼƬ���ݵ�λ���е���
	 width = cv_matrix->width;//��ȡͼ����Ϣ
	 height= cv_matrix->height;//��ȡͼ����Ϣ
	 raw = (char*)cv_matrix->data.ptr;
	 Image image_z(width, height, "Y800", raw, width * height);//����ͼ������
	 //��ʼɨ��ͼ���еĶ�ά��
     int n = zbar_scan_image(scanner, image_z);//n=0ʧ��;n=1�ɹ�
	 //��ȡ���
	 const zbar_symbol_t *symbol = zbar_image_first_symbol(image_z);
	 for(; symbol; symbol = zbar_symbol_next(symbol)) 
	 {
		 zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		 const char *data = zbar_symbol_get_data(symbol); 
		 CString temp = data;
		 LPCWSTR app = (LPCWSTR)(temp.AllocSysString());
		 *result = TranslateString(app);
     }
     image_z.set_data(NULL, 0);//�������
	 cvWaitKey(0);
	 cvReleaseMat(&cv_matrix);//�ͷ���Դ
	 return n;//n=0ʧ��;n=1�ɹ�
}