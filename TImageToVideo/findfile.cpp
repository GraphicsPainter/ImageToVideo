#include "findfile.h"
#include <fstream>
#include "cJSON.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

int DirectoryIsExist(const std::string& filename)
{
	struct stat info;
	if (stat(filename.c_str(), &info) == 0) // does not exist
	{  
		return F_FILE_OK;
	}
	else if (info.st_mode & S_IFDIR) // directory
	{         
		return F_IS_Directory;
	}
	else {
		return F_ERROR;
	}
	return true;
}

void FindformatFile(const char * path, const char * format, std::vector<IM_FindData> &vec_job)
{
	char newpath[200];
	strcpy_s(newpath, path);
	strcat_s(newpath, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
	int handle;
	_finddata_t findData;
	IM_FindData ffdata;
	handle = _findfirst(newpath, &findData);
	if (handle == -1)        // 检查是否成功
		return;
	while (_findnext(handle, &findData) == 0)
	{
		if (findData.attrib & _A_SUBDIR) 
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
			{
				continue;
			}
			strcpy_s(newpath, path);
			strcat_s(newpath, "\\");
			strcat_s(newpath, findData.name);
			FindformatFile(newpath, format, vec_job);
		}
		else 
		{
			if (strstr(findData.name, format)) 
			{ 
				//memset(&ffdata, 0, sizeof(IM_FindData));
				sprintf_s(ffdata.name, 128, "%s", findData.name);
				sprintf_s(ffdata.path, 1024, "%s", path);
				ffdata.time_create = findData.time_create;
				ffdata.time_write = findData.time_write;
				ffdata.time_access = findData.time_access;
				ffdata.size = findData.size;
				ffdata.attrib = findData.attrib;
				sprintf_s(ffdata.fullpath, 1024, "%s%s", path, findData.name);
				vec_job.push_back(ffdata);
			}
		}
	}
	_findclose(handle);    // 关闭搜索句柄
}

bool ReadFileToString(const char* path, std::string &s_json)
{
	std::ifstream fr(path);
	if (!fr)
	{
		return false;
	}
	fr.seekg(0, std::ios::end);
	auto size = fr.tellg();
	fr.seekg(0, std::ios::beg);

	char *p = new char[size];
	if (p == NULL)
	{
		return false;
	}
	memset(p, 0, size);
	fr.read(p, size);
	s_json = p;
	delete[]p;
	p = NULL;
	fr.close();
	return true;
}

bool WriteDefaultConfigJson(const std::string& filename)
{
	std::string file;
	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "Fps", 1);
	cJSON_AddNumberToObject(root, "Fourcc", 6);
	cJSON_AddStringToObject(root, "TargetSuffix", ".jpg");
	cJSON_AddNumberToObject(root, "AutoStretch", 1);
	cJSON_AddStringToObject(root, "ResolutionRatio", "1920*1080");
	cJSON_AddStringToObject(root, "PKGFormat", ".mp4");
	cJSON_AddStringToObject(root, "OutPath", "");

	cJSON* Explain = cJSON_CreateObject();
	cJSON_AddStringToObject(Explain, "Fps", "帧率");
	cJSON_AddStringToObject(Explain, "Fourcc", "视频编码格式,1:MPEG_1;2:JPEG;3:MPEG_4_2;4:MPEG_4_3;5:MPEG_4;6:H263;7:H263I;8:FLV1");
	cJSON_AddStringToObject(Explain, "TargetSuffix", "目标图片后缀");
	cJSON_AddStringToObject(Explain, "AutoStretch", "0:默认按照第一张图片拉伸,1:按照参数ResolutionRatio拉伸,2:不改变图片比例以黑色背景进行填充");
	cJSON_AddStringToObject(Explain, "ResolutionRatio", "生成视频帧的宽*高");
	cJSON_AddStringToObject(Explain, "PKGFormat", "输出视频封装格式.mp4;.avi;.mkv");
	cJSON_AddStringToObject(Explain, "OutPath", "输出路径");
	cJSON_AddItemToObject(root, "Parameter_Explain", Explain);
	char* ptr = cJSON_Print(root);
	if (ptr != NULL)
	{
		file = ptr;
		delete ptr;
		ptr = NULL;
	}
	cJSON_Delete(root);
	bool flg = WriteFileToPath(filename, file);
	return flg;
}

bool ReadConfigFile(const char* filename, VideoFormat& _videoformat, std::string& outpath)
{
	bool flg = true;
	std::string file;
	bool openfile = ReadFileToString(filename, file);
	if (!openfile)
	{
		return false;
	}
	cJSON* root = cJSON_Parse(file.c_str());
	if (root)
	{
		cJSON* FPS = cJSON_GetObjectItem(root, "Fps");//帧率
		if (cJSON_IsNumber(FPS))
		{
			_videoformat.ifps = FPS->valueint;
		}

		cJSON* Fourcc = cJSON_GetObjectItem(root, "Fourcc");//编码格式
		if (cJSON_IsNumber(Fourcc))
		{
			_videoformat.ifourcc = Fourcc->valueint;
		}

		cJSON* Suffix = cJSON_GetObjectItem(root, "TargetSuffix");//目标后缀
		if (cJSON_IsString(Suffix))
		{
			std::string suf = Suffix->valuestring;
			if (_strnicmp(suf.c_str(), ".png", 4) == 0)
			{
				_videoformat.isuffix = P_PNG;
			}
			else if (_strnicmp(suf.c_str(), ".jpg", 4) == 0)
			{
				_videoformat.isuffix = P_JPG;
			}
			else if (_strnicmp(suf.c_str(), ".jpeg", 5) == 0)
			{
				_videoformat.isuffix = P_JPEG;
			}
			else if (_strnicmp(suf.c_str(), ".bmp", 4) == 0)
			{
				_videoformat.isuffix = P_BMP;
			}
			else {
				_videoformat.isuffix = P_PNG;
			}
		}
		
		cJSON* PATH = cJSON_GetObjectItem(root, "OutFile");//输出路径
		if (cJSON_IsString(PATH))
		{
			outpath = PATH->valuestring;
		}
		
		cJSON* PKGModel = cJSON_GetObjectItem(root, "PKGFormat");//输出封装格式
		if (cJSON_IsString(PKGModel))
		{
			int model = GetVideoPKGModelWithSuffix(PKGModel->valuestring);
			if (model != PKG_ERROR)
			{
				_videoformat.ipkgmdel = model;
			}
			else {
				_videoformat.ipkgmdel = PKG_ERROR;
			}
		}

		cJSON* RESOLUT = cJSON_GetObjectItem(root, "ResolutionRatio");//分辨率
		if (cJSON_IsString(RESOLUT))
		{
			std::string str = RESOLUT->valuestring;
			size_t pos = str.find("*");
			if (pos != std::string::npos)
			_videoformat.iwidth = atoi(str.substr(0, pos).c_str());
			_videoformat.iheigh = atoi(str.substr(pos + 1, str.size() - pos - 1).c_str());
		}
		
		cJSON* Stretch = cJSON_GetObjectItem(root, "AutoStretch");//自动缩放
		if (cJSON_IsNumber(Stretch))
		{
			_videoformat.iautoStretch = Stretch->valueint;
		}
	}
	else
	{
		flg = false;
	}
	cJSON_Delete(root);
	return flg;
}

void FindDirectoryFileList(const char* directory)
{

}

bool WriteFileToPath(const std::string& filepath, const std::string& str)
{
	bool flg = true;
	std::fstream fw(filepath, std::ios::out);
	if (fw.is_open())
	{
		fw << str;
	}
	else {
		flg = false;
	}
	fw.close();
	return flg;
}

std::string GetPICSuffix(int suffix)
{
	std::string sufix;
	switch (suffix)
	{
	case P_PNG: sufix = ".png";
		break;
	case P_JPG: sufix = ".jpg";
		break;
	case P_JPEG: sufix = ".jpeg";
		break;
	case P_BMP: sufix = ".bmp";
		break;
	default: sufix = ".png";
		break;
	}
	return sufix;
}

std::string GetVideoPkgSuffixWithPKGModel(int pkgmodel)
{
	std::string videopkg;
	switch (pkgmodel)
	{
	case PKG_MP4: videopkg = ".mp4";
		break;
	case PKG_AVI: videopkg = ".avi";
		break;
	case PKG_MKV: videopkg = ".mkv";
		break;
	case PKG_MOV: videopkg = ".mov";
		break;
	case PKG_FLV: videopkg = ".flv";
		break;
	default: videopkg = ".mp4";
		break;
	}
	return videopkg;
}

//return model
int GetVideoPKGModelWithSuffix(const std::string& filename)
{
	std::string tmpfile = filename;
	auto notfind = std::string::npos;
	std::transform(tmpfile.begin(), tmpfile.end(), tmpfile.begin(), tolower);
	if (tmpfile.find("mp4") != notfind)
	{
		return PKG_MP4;
	}
	else if (tmpfile.find("avi") != notfind)
	{
		return PKG_AVI;
	}
	else if (tmpfile.find("mkv") != notfind)
	{
		return PKG_MKV;
	}
	else if (tmpfile.find("mov") != notfind)
	{
		return PKG_MOV;
	}
	else if (tmpfile.find("flv") != notfind)
	{
		return PKG_FLV;
	}
	else {
		return PKG_ERROR;
	}
	return PKG_ERROR;
}

//对比视频编码与封装格式是否匹配
bool CompareFourccWithPKG(const int& fourcc, const int& pkg)
{
	if (fourcc == FOC_MPEG_1 || fourcc == FOC_MPEG_4 || fourcc == FOC_H263)
	{
		return pkg == PKG_MP4 ? true : false;
	}
	return false;
}

