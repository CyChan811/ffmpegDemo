#pragma once
#include <iostream>
using namespace std;

extern "C"
{
	#include <avformat.h>
	#include </usr/local/include/libavutil/pixdesc.h>
}

class MMAVStream;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();

	int GetIndex();
public:
	AVPacket* pkt = nullptr;
};

class MMAVFrame
{
public:
	MMAVFrame();
	~MMAVFrame();

	int VideoPrint();
	int AudioPrint();

	int GetW();
	int GetH();

	int GetY(unsigned char *y);
	int GetU(unsigned char *u);
	int GetV(unsigned char *v);

	long long GetPts();
public:
	AVFrame* frame = nullptr;
	double ptsSec = 0.0;
};


class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();

	int Open(const char* path);

	int GetStreamCount();	//获取stream数量
	int GetStream(MMAVStream *avStream, int streamID);	//获取stream

	int GetVideoStreamIndex();
	int GetAudioStreamIndex();

	int Close();

	int Read(MMAVPacket* packet);

	int Seek(double time);

private:
	AVFormatContext* formatCtx = nullptr;
};


class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();

public:
	int streamIndex = -1;	//stream的id
	int timebaseNum = 0;
	int timebaseDen = 0;
	AVCodecParameters *codecParam = nullptr;
};

class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder();

	int Init(MMAVStream* stream);

	int SendPacket(MMAVPacket* avPkt);

	int RecvFrame(MMAVFrame *avFrame);

	int Close();
public:
	AVCodecContext * codecContext = nullptr;
	int timebaseNum = 0;
	int timebaseDen = 0;
};


class MMAVTime
{
public:
	static long long GetTime();
};