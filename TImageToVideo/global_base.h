#pragma once
#include <string>
#include <fstream>

#define Sec2Debug(str) sec2debug_printf(__LINE__, str)

static std::string sec2debug_printf(int line, const char* str)
{
	std::string err(__TIME__);
	err += " -- Error on line: ";
	err += std::to_string(line);
	err += ",Describe: ";
	err += str;
	err += "\n";
	return err;
}


//视频编码格式
enum PIC_FourccModel
{
	FOC_ERROR,
	FOC_MPEG_1 = 1,      //mkv,avi(fps需>5),
	FOC_JPEG,            //avi
	FOC_MPEG_4_2,
	FOC_MPEG_4_3,
	FOC_MPEG_4,         //mp4,mkv,avi
	FOC_H263,           //mp4
	FOC_H263I,          //mp4
	FOC_FLV1            //flv
};

//视频封装格式
enum PIC_PkgModel {
	PKG_ERROR,
	PKG_MP4,
	PKG_AVI,
	PKG_MKV,
	PKG_MOV,
	PKG_FLV
};

//图片后缀
enum PIC_Suffix
{
	P_PNG = 1,
	P_JPG,
	P_JPEG,
	P_BMP
};

//文件类型
enum FILE_Format
{
	F_ERROR,
	F_FILE_OK,
	F_FILE_CAN_NOT_ACCESS,
	F_FILE_NOT_EXIST,
	F_IS_Directory,
};

static std::string GlobalLogFile;

//错误码
static const char* errorcode[20] = {
	"0 Unknown error.",
	"1 Code is OK.",
	"2 File size is too big.",
	"3 Cannot access file.",
	"4 Not is a directory.",
	"5 No corresponding format file found.",
	"6 Number of parameters does not match.",
	"7 Open config.json error.",
	"8 Can`t find config.json.",
	"9 Create video error.",
	"10 OpenCV function error."
};



struct IM_FindData
{
	unsigned attrib;    //attrihute
	__int64 time_create;    // -1 for FAT file systems
	__int64 time_access;    // -1 for FAT file systems
	__int64 time_write;
	size_t size;
	char name[260];
	char path[1024];
	char fullpath[1024];
};


struct VideoFormat {
	int ifps;//帧率
	int ifourcc;//编码格式
	int iwidth;//分辨率，宽
	int iheigh;//分辨率，高
	int iautoStretch;//启用缩放
	int ipkgmdel;//视频封装格式,.mp4,.avi...
	char c_suffix[8];//目标文件后缀
};
























