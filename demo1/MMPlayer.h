#pragma once 

#include <string>
#include "MMThread.h"
#include "MMAV.h"
#include "MMQueue.h"
#include <chrono>

using namespace std;

enum MMDecoderType
{
    MMDECODER_TYPE_VIDEO = 0,
    MMDECODER_TYPE_AUDIO = 1
};

enum MMPlayerCtrStatus
{
    MMPLAYER_CTR_STATUS_PLAYING = 0,
    MMPLAYER_CTR_STATUS_PAUSEING = 1
};

class MMPlayerCtr:public MMThread
{
public:
    MMPlayerCtr(string path, double seekTime = 0.0);
    ~MMPlayerCtr();

    void run();

    int GetVideoQueueSize();
    int GetAudioQueueSize();

    int PushFrameToVideoQueue(MMAVFrame* frame);
    int PushFrameToAudioQueue(MMAVFrame* frame);

    int Play();
    int Pause();
private:
    MMQueue<MMAVFrame> videoQueue;
    MMQueue<MMAVFrame> audioQueue;
    string path;
    MMPlayerCtrStatus status = MMPlayerCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
    double seekTime;
};

class MMPlayerReaderThread : public MMThread
{
public:
   MMPlayerReaderThread(string path, MMPlayerCtr* playerCtr, double seekTime = 0.0);
    ~MMPlayerReaderThread();

    void run();

private:
    string path;
    MMPlayerCtr* playerCtr = nullptr;
    double seekTime;
};


class MMPlayerDecoderThread: public MMThread
{
public:
    MMPlayerDecoderThread(MMPlayerCtr* playerCtr, MMDecoderType type);
    ~MMPlayerDecoderThread();

    void run();

    int Init(MMAVStream* avStream);

    int pushPacket(MMAVPacket* pkt);

    int GetPacketQueueSize();

private:
    MMAVDecoder *decoder = nullptr;
    MMQueue<MMAVPacket> packetQueue;
    MMPlayerCtr* playerCtr = nullptr;
    MMDecoderType type;
};


class MMPlayer
{
public:
    MMPlayer(string path);
    ~MMPlayer();

    int Open(double time = 0.0);
    int Stop();

    int Play();
    int Pause();

    int Seek(double time);

private:
    string path;
    MMPlayerReaderThread* readerThread = nullptr;

    MMPlayerCtr* playerCtr = nullptr;
};