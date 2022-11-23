#include <iostream>
#include <fstream>
#include "TImageToVideo.h"
#include "global_base.h"
#include "findfile.h"

#ifdef OPENCV_3_4_12
int GetFourccModel(int ifourcc)
{
	int fourcc = 0;
	switch (ifourcc)
	{
	case FOC_MPEG_1: fourcc = CV_FOURCC('P', 'I', 'M', '1');
		break;
	case FOC_JPEG: fourcc = CV_FOURCC('M', 'J', 'P', 'G');
		break;
	case FOC_MPEG_4_2: fourcc = CV_FOURCC('M', 'P', '4', '2');
		break;
	case FOC_MPEG_4_3: fourcc = CV_FOURCC('D', 'I', 'V', '3');
		break;
	case FOC_MPEG_4: fourcc = CV_FOURCC('D', 'I', 'V', 'X');
		break;
	case FOC_H263: fourcc = CV_FOURCC('U', '2', '6', '3');
		break;
	case FOC_H263I: fourcc = CV_FOURCC('I', '2', '6', '3');
		break;
	case FOC_FLV1: fourcc = CV_FOURCC('F', 'L', 'V', '1');
		break;
	default:
		fourcc = CV_FOURCC('U', '2', '6', '3');
		break;
	}
	return fourcc;
}
#endif

#ifdef OPENCV_4_5_5
int GetFourccModel(int ifourcc)
{
	int fourcc = 0;
	switch (ifourcc)
	{
	case FOC_MPEG_1: fourcc = cv::VideoWriter::fourcc('P', 'I', 'M', '1');
		break;
	case FOC_JPEG: fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
		break;
	case FOC_MPEG_4_2: fourcc = cv::VideoWriter::fourcc('M', 'P', '4', '2');
		break;
	case FOC_MPEG_4_3: fourcc = cv::VideoWriter::fourcc('D', 'I', 'V', '3');
		break;
	case FOC_MPEG_4: fourcc = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
		break;
	case FOC_H263: fourcc = cv::VideoWriter::fourcc('U', '2', '6', '3');
		break;
	case FOC_H263I: fourcc = cv::VideoWriter::fourcc('I', '2', '6', '3');
		break;
	case FOC_FLV1: fourcc = cv::VideoWriter::fourcc('F', 'L', 'V', '1');
		break;
	default:
		fourcc = cv::VideoWriter::fourcc('U', '2', '6', '3');
		break;
	}
	return fourcc;
}
#endif



int WriteImageToVideo(const std::string& OutFile, std::vector<std::string>& filelist, int fps, int width, int heigh, int autoStretch, int fourcc)
{
	try {
		fourcc = GetFourccModel(fourcc);
		if (filelist.size() <= 0) 
			return -1;//No corresponding format file found
		cv::VideoWriter writer;
		int errorcode = 0;
		if (1 == autoStretch)//按照设置参数拉伸图像比例
		{
			cv::Size frameSize(width, heigh);
			bool openflg = writer.open(OutFile, fourcc, fps, frameSize, true);
			if (!openflg)
			{
				throw "Output video path error.";
			}
			for (size_t i = 0; i < filelist.size(); i++)
			{
				cv::Mat img;
				img = cv::imread(filelist[i]);
				cv::Size imgsize = img.size();
				if (img.empty())
				{
					continue;
				}
				if ((imgsize.height != frameSize.height)
					&& (imgsize.width != frameSize.width))
				{
					cv::resize(img, img, frameSize);
				}
				writer.write(img);
			}
		}
		else if (2 == autoStretch)//不改变原始比例,添加一张纯黑色底图
		{
			cv::Size frameSize(width, heigh);
			bool isresize = false;
			bool openflg = writer.open(OutFile, fourcc, fps, frameSize, true);
			if (!openflg)
			{
				throw "Output video path error.";
			}
			for (size_t i = 0; i < filelist.size(); i++)
			{
				cv::Mat img;
				img = cv::imread(filelist[i]);
				cv::Size imgsize = img.size();
				if (img.empty())
				{
					continue;
				}

				if ((imgsize.height == frameSize.height)
					&& (imgsize.width == frameSize.width))
				{
					writer.write(img);
				}
				else if ((imgsize.height < frameSize.height)
					&& (imgsize.width < frameSize.width))
				{
					cv::Mat tmpimg(frameSize, img.type(), cv::Scalar(0, 0, 0));
					int offsety = (frameSize.height - imgsize.height) / 2;
					int offsetx = (frameSize.width - imgsize.width) / 2;
					cv::Rect rect(cv::Point(offsetx, offsety), cv::Point(offsetx + img.rows, offsety + img.cols));
					img.copyTo(tmpimg(rect));
					writer.write(tmpimg);
				}
				else
				{
					if (imgsize.width > frameSize.width)
					{
						isresize = true;
						imgsize.width = frameSize.width;
						double multi = (double)frameSize.width / (double)img.size().width;
						double heighttmp = multi * imgsize.height;
						imgsize.height = (int)floor(heighttmp);
					}
					if (imgsize.height > frameSize.height)
					{
						isresize = true;
						double multi2 = (double)imgsize.height / (double)frameSize.height;
						double widthtmp = multi2 * imgsize.width;
						imgsize.width = (int)floor(widthtmp);
						imgsize.height = frameSize.height;
					}
					if (isresize)
					{
						cv::resize(img, img, imgsize);
					}
					int offsety = (frameSize.height - imgsize.height) / 2;
					int offsetx = (frameSize.width - imgsize.width) / 2;
					if (offsety >= 0 || offsetx >= 0)
					{
						/*if (offsetx == 0)
						{
							offsetx = 1;
						}
						if (offsety == 0)
						{
							offsety = 1;
						}*/
						cv::Mat tmpimg(frameSize, img.type(), cv::Scalar(0, 0, 0));
						cv::Rect rect(cv::Point(offsetx, offsety), cv::Point(offsetx + imgsize.width, offsety + imgsize.height));
						img.copyTo(tmpimg(rect));
						writer.write(tmpimg);
					}
					else
					{
						writer.write(img);
					}
				}
			}
		}
		else//设置第一张图尺寸为默认尺寸，后面所有图片缩放至默认大小
		{
			cv::Size frameSize;
			for (size_t i = 0; i < filelist.size(); ++i)
			{
				cv::Mat img;
				img = cv::imread(filelist[i]);
				if (img.empty())
				{
					continue;
				}
				if (!writer.isOpened())//default with the first picture size for video.
				{
					frameSize.width = width;
					frameSize.height = heigh;
					if (!writer.open(OutFile, fourcc, fps, frameSize, true))
					{
						throw "open video writer error...";
					}
				}
				else
				{
					if ((img.size().height != frameSize.height)
						&& (img.size().width != frameSize.width))
					{
						cv::resize(img, img, frameSize);
					}
				}
				writer.write(img);
			}
		}
		writer.release();
	}
	catch (const char* e)
	{
		WriteFileToPath(GlobalLogFile, Sec2Debug(e));
		return 10;
	}
	return 1;
}
