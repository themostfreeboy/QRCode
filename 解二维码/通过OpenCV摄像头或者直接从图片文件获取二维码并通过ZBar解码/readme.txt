此程序使用OpenCV调用摄像头，通过ZBar处理图片，解析出二维码中的信息。

此程序可以通过OpenCV调用的摄像头扫描获取二维码图片，也可以直接通过拖拽二维码图片到控制台中解析二维码中的信息。

此程序与现成的程序“zbarcam.exe”相比：
优点：有源代码，可以通过封装直接将代码嵌入到其他所需要的程序当中；此程序不仅可以通过摄像头获取二维码，还可以通过直接将二维码图片拖拽获取图片信息，而“zbarcam.exe”只能通过摄像头获取二维码信息。
缺点：编译后的文件大小比“zbarcam.exe”大；而且由于用VS2010编译生成，运行时除了需要ZBar所需要的libiconv-2.dll和libzbar-0.dll，还需要OpenCV所需要的opencv_core2411d.dll、opencv_highgui2411d.dll和opencv_imgproc2411d.dll，还需要kernel32.dll、user32.dll、shlwapi.dllmsvcr100d.dll、oleaut32.dll、msvcr100d.dll和msvcr100d.dll，而现成的程序“zbarcam.exe”中调用摄像头直接通过video4linux(V4L)的内核驱动使用API函数，所以不需要OpenCV所需要的dll，只需要ZBar所需要的libiconv-2.dll和libzbar-0.dll，还有kernel32.dll和msvcrt.dll；除此之外，此程序编译所需OpenCV环境，所以配置起来稍微麻烦些，需要做下列配置：

{

平台对应：
VC8 VS2005
VC9 VS2008
VC10 VS2010
VC11 VS2012
VC12 VS2013


环境变量：
path：
...\build\x86\vc10\bin;
...\build\x64\vc10\bin;


工程属性：
配置属性：
VC++目录：
包含目录：
...\build\include
...\build\include\opencv
...\build\include\opencv2
库目录：
...\build\x86\vc10\lib


工程属性：
配置属性：
链接器：
输入：
附加依赖项：
opencv_calib3d2411d.lib
opencv_contrib2411d.lib
opencv_core2411d.lib
opencv_features2d2411d.lib
opencv_flann2411d.lib
opencv_gpu2411d.lib
opencv_highgui2411d.lib
opencv_imgproc2411d.lib
opencv_legacy2411d.lib
opencv_ml2411d.lib
opencv_nonfree2411d.lib
opencv_objdetect2411d.lib
opencv_ocl2411d.lib
opencv_photo2411d.lib
opencv_stitching2411d.lib
opencv_superres2411d.lib
opencv_ts2411d.lib
opencv_video2411d.lib
opencv_videostab2411d.lib
libzbar-0.lib

}