#include "TImageToVideo.h"
#include "findfile.h"
#include <algorithm>
//1
int main(int argc, char *argv[])
{
	std::string exepath = argv[0];
	auto pos = exepath.find_last_of(".");
	GlobalLogFile = exepath.substr(0, pos);
	GlobalLogFile += ".log";
	if (argc == 3) 
	{
		std::string indir = argv[1];
		std::string outdir = argv[2];
		std::vector<IM_FindData> vec_filelist;
		std::vector<IM_FindData> vec_imagelist;
		FindformatFile(indir.c_str(), ".json", vec_filelist);
		if (vec_filelist.empty())
		{
			std::string defaultjson = indir;
			defaultjson += "videoconfig.json";
			WriteDefaultConfigJson(defaultjson);//写默认配置文件
		}
		else {
			VideoFormat format;//读取配置文件
			std::string outpath;//视频封装格式
			auto config = vec_filelist.at(0).fullpath;
			if ((strlen(config) < 1) || (DirectoryIsExist(config) != F_FILE_OK))
			{
				WriteFileToPath(GlobalLogFile, Sec2Debug("Can`t find config.json."));
				return 0;
			}
			bool rflg = ReadConfigFile(config, format, outpath);
			if (!rflg)
			{
				WriteFileToPath(GlobalLogFile, Sec2Debug("Can`t open config.json."));
				return 0;
			}

			std::string suffix = GetPICSuffix(format.isuffix);
			FindformatFile(indir.c_str(), suffix.c_str(), vec_imagelist);
			if (vec_imagelist.size() > 0)
			{
				std::vector<std::string> imgfilelist;
				for (size_t i = 0; i < vec_imagelist.size(); i++)
				{
					imgfilelist.push_back(vec_imagelist[i].fullpath);
				}
				std::sort(imgfilelist.begin(), imgfilelist.end(), [=](const std::string& s1, const std::string& s2) {
					return s1 < s2 ? true : false;
				});
				if (imgfilelist.size() > 0)
				{
					WriteImageToVideo(outdir, imgfilelist, format.ifps, format.iwidth, format.iheigh, format.iautoStretch);
				}
			}
			else {
				WriteFileToPath(GlobalLogFile, Sec2Debug("No corresponding format file found."));
			}
		}
	}
	else {
		WriteFileToPath(GlobalLogFile, Sec2Debug("Number of parameters does not match."));
	}

	return 1;
}
