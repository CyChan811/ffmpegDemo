#include "MMAV.h"

MMAVReader::MMAVReader()
{
	formatCtx = avformat_alloc_context();
}


MMAVReader::~MMAVReader()
{
	if(formatCtx != nullptr)
	{
		avformat_free_context(formatCtx);
		formatCtx = nullptr;
	}
}


int MMAVReader::Read(MMAVPacket* packet)
{
	if(formatCtx == nullptr)
	{
		return -1;
	}

	int ret = av_read_frame(formatCtx, packet->pkt);

	return ret;
}

int MMAVReader::Open(const char* path)
{
	if(path == nullptr)
	{
		return -1;
	}

	if(formatCtx == nullptr)
	{
		return -1;
	}

	//0代表成功
	int ret = avformat_open_input(&formatCtx, path, nullptr, nullptr);

	if(!ret)
	{
		//获取流信息
		avformat_find_stream_info(formatCtx, nullptr);
	}

	return ret;
}

int MMAVReader::Close()
{
	if(formatCtx == nullptr)
	{
		return -1;
	}
	avformat_close_input(&formatCtx);
	return 0;
}


//获取stream数量
int MMAVReader::GetStreamCount()
{
	return formatCtx->nb_streams;
}



int MMAVReader::GetStream(MMAVStream *avStream, int streamID)
{
	AVStream* ffmpegStream = formatCtx->streams[streamID];

	avStream->timebaseNum = ffmpegStream->time_base.num;
	avStream->timebaseDen = ffmpegStream->time_base.den;

	avStream->streamIndex = ffmpegStream->index;
	avcodec_parameters_copy(avStream->codecParam, ffmpegStream->codecpar);	//从ffmpegStream拷贝参数到avStream

	return 0;
}

//获取视频流id
int MMAVReader::GetVideoStreamIndex()
{

	return av_find_best_stream(formatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
}


//获取音频流id
int MMAVReader::GetAudioStreamIndex()
{
	return av_find_best_stream(formatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

}

int MMAVReader::Seek(double time)
{
	if(formatCtx == nullptr)
	{
		return -1;
	}

	int64_t timestamp = (int64_t)(time * AV_TIME_BASE);
	av_seek_frame(formatCtx, -1, timestamp, AVSEEK_FLAG_BACKWARD);
	return 0;
}
