#pragma once
#include <string>

#if _DEBUG
#pragma comment(lib, "./lib_d/IlmImfd.lib")
#pragma comment(lib, "./lib_d/ippicvmt.lib")
#pragma comment(lib, "./lib_d/ippiwd.lib")
#pragma comment(lib, "./lib_d/ittnotifyd.lib")
#pragma comment(lib, "./lib_d/libjasperd.lib")
#pragma comment(lib, "./lib_d/libjpeg-turbod.lib")
#pragma comment(lib, "./lib_d/libpngd.lib")
#pragma comment(lib, "./lib_d/libprotobufd.lib")
#pragma comment(lib, "./lib_d/libtiffd.lib")
#pragma comment(lib, "./lib_d/libwebpd.lib")
#pragma comment(lib, "./lib_d/opencv_core3412d.lib")
#pragma comment(lib, "./lib_d/opencv_features2d3412d.lib")
#pragma comment(lib, "./lib_d/opencv_highgui3412d.lib")
#pragma comment(lib, "./lib_d/opencv_imgcodecs3412d.lib")
#pragma comment(lib, "./lib_d/opencv_imgproc3412d.lib")
#pragma comment(lib, "./lib_d/opencv_video3412d.lib")
#pragma comment(lib, "./lib_d/opencv_videoio3412d.lib")
#pragma comment(lib, "./lib_d/opencv_videostab3412d.lib")
#pragma comment(lib, "./lib_d/zlibd.lib")

#else

#pragma comment(lib, "./lib/IlmImf.lib")
#pragma comment(lib, "./lib/ippicvmt.lib")
#pragma comment(lib, "./lib/ippiw.lib")
#pragma comment(lib, "./lib/ittnotify.lib")
#pragma comment(lib, "./lib/libjasper.lib")
#pragma comment(lib, "./lib/libjpeg-turbo.lib")
#pragma comment(lib, "./lib/libpng.lib")
#pragma comment(lib, "./lib/libprotobuf.lib")
#pragma comment(lib, "./lib/libtiff.lib")
#pragma comment(lib, "./lib/libwebp.lib")
#pragma comment(lib, "./lib/opencv_core3412.lib")
#pragma comment(lib, "./lib/opencv_features2d3412.lib")
#pragma comment(lib, "./lib/opencv_highgui3412.lib")
#pragma comment(lib, "./lib/opencv_imgcodecs3412.lib")
#pragma comment(lib, "./lib/opencv_imgproc3412.lib")
#pragma comment(lib, "./lib/opencv_video3412.lib")
#pragma comment(lib, "./lib/opencv_videoio3412.lib")
#pragma comment(lib, "./lib/opencv_videostab3412.lib")
#pragma comment(lib, "./lib/zlib.lib")

#endif
#define OPENCV_3_4_12
#include "include/opencv2/opencv.hpp"
#include "include/opencv2/highgui.hpp"
#include "include/opencv2/videoio.hpp"

#include <string>
#include "global_base.h"

//default fourcc = CV_FOURCC('U', '2', '6', '3');
int GetFourccModel(int ifourcc);

//OutFile:   path\\file.mp4
//filelist:  input photo.png list
int WriteImageToVideo(const std::string& OutFile, std::vector<std::string>& filelist, int fps, int width, int heigh, int autoStretch, int fourcc = FOC_H263);

/*
		CV_FOURCC('P', 'I', 'M', '1') = MPEG - 1 codec
		CV_FOURCC('M', 'J', 'P', 'G') = motion - jpeg codec    avi
		CV_FOURCC('M', 'P', '4', '2') = MPEG - 4.2 codec
		CV_FOURCC('D', 'I', 'V', '3') = MPEG - 4.3 codec
		CV_FOURCC('D', 'I', 'V', 'X') = MPEG - 4 codec
		CV_FOURCC('U', '2', '6', '3') = H263 codec            mp4
		CV_FOURCC('I', '2', '6', '3') = H263I codec
		CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec
*/