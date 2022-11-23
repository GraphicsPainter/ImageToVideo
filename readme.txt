exe需传入两个参数
1、目标图片路径，需以\\结尾
2、输出视频全路径及文件名


配置文件参数说明：
"Fps":"帧率",
"Fourcc":"视频编码格式,1:MPEG_1;2:JPEG;3:MPEG_4_2;4:MPEG_4_3;5:MPEG_4;6:H263;7:H263I;8:FLV1",
"TargetSuffix":"目标图片后缀",
"AutoStretch":"0:默认按照第一张图片拉伸,1:按照参数ResolutionRatio拉伸,2:不改变图片比例以黑色背景进行填充",
"ResolutionRatio":"生成视频帧的宽*高",
"PKGFormat":"输出视频封装格式.mp4;.avi;.mkv",
"OutPath":"输出路径"


示例：
key	value	value-type
"Fps":	1,	（number）
"Fourcc":	6,	（number）（default：6，其他暂未使用）
"TargetSuffix":	".jpg",	（string）
"AutoStretch":	2,	（number）
"ResolutionRatio":	"500*500",	（string）
"PKGFormat":	".mp4",	（string）（暂未使用）
"OutPath":	"",	（string）（暂未使用）