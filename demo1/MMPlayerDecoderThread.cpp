#include "MMPlayer.h"

MMPlayerDecoderThread::MMPlayerDecoderThread(MMPlayerCtr* _playerCtr,MMDecoderType _type)
{
    if(decoder == nullptr)
        decoder = new MMAVDecoder();
    playerCtr = _playerCtr;
    type = _type;
}


MMPlayerDecoderThread::~MMPlayerDecoderThread()
{
    if(decoder != nullptr)
    {
        delete decoder;
        decoder = nullptr;
    }
}


int MMPlayerDecoderThread::Init(MMAVStream* avStream)
{
    return decoder->Init(avStream);
}


int MMPlayerDecoderThread::pushPacket(MMAVPacket* pkt)
{
    return packetQueue.Push(pkt);
}


void MMPlayerDecoderThread::run()
{
    //进行解码操作
    while(!stopFlag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if(type == MMDecoderType::MMDECODER_TYPE_VIDEO)
        {
            //防止取太多，内存过大
            if(playerCtr->GetVideoQueueSize() > 10)
            {
                continue;
            }
        }
        else if(type == MMDecoderType::MMDECODER_TYPE_AUDIO)
        {
            if(playerCtr->GetAudioQueueSize() > 20)
            {
                continue;
            }
        }

        MMAVPacket* pkt = nullptr;
        int ret = packetQueue.Pop(&pkt);
        if(ret)
        {
            //队列为空，取不到packet
            continue;
        }

        decoder->SendPacket(pkt);
        while(1)
        {
            MMAVFrame *frame = new (MMAVFrame);
            ret = decoder->RecvFrame(frame);
            if(ret)
            {
                break;
            }
            //cout << "Decoder Success!" << endl;

            //通过playerCtr的句柄，向其塞入frame数据
            if(type == MMDecoderType::MMDECODER_TYPE_VIDEO)
            {
                playerCtr->PushFrameToVideoQueue(frame);
            }
            else if(type == MMDecoderType::MMDECODER_TYPE_AUDIO)
            {
                playerCtr->PushFrameToAudioQueue(frame);
            }
        }
    }
}


int MMPlayerDecoderThread::GetPacketQueueSize()
{
    return packetQueue.Size();
}