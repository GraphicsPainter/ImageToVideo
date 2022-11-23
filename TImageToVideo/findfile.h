#pragma once
#include <Windows.h>
#include <io.h>		//_finddata_t��ͷ�ļ�
#include <vector>
#include <string>
#include "global_base.h"
#include <fstream>

int DirectoryIsExist(const std::string& filename);
//���Һ�׺ƥ���ļ�
void FindformatFile(const char * path, const char * format, std::vector<IM_FindData> &vec_job);
//��ȡ�ļ����ݵ�string
bool ReadFileToString(const char* path, std::string &s_json);
//Read config json.Don't care key Sensitive.
//If fail,return false;
bool ReadConfigFile(const char* filename, VideoFormat& _videoformat, std::string& outpath);
//�����ļ��б�
void FindDirectoryFileList(const char* directory);

bool WriteDefaultConfigJson(const std::string& filename);

bool WriteFileToPath(const std::string& filepath, const std::string& str);

//��ȡͼƬ�ļ���׺
std::string GetPICSuffix(int i);


//return .mp4/.avi/.mkv
std::string GetVideoPkgSuffixWithPKGModel(int pkgmodel);
//return enum PIC_PkgModel
int GetVideoPKGModelWithSuffix(const std::string& filename);
//Check whether the video encoding matches the encapsulation
bool CompareFourccWithPKG(const int& fourcc, const int& pkg);


