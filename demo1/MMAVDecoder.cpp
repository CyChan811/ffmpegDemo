#include "MMAV.h"

MMAVDecoder::MMAVDecoder()
{
    codecContext = avcodec_alloc_context3(nullptr); //开辟context内存
}


MMAVDecoder::~MMAVDecoder()
{
    if(codecContext != nullptr)
    {
        avcodec_free_context(&codecContext);    //释放内存
        codecContext =nullptr;
    }
}


int MMAVDecoder::Init(MMAVStream* stream)
{
    timebaseNum = stream->timebaseNum;
	timebaseDen = stream->timebaseDen;
    //配置参数
    avcodec_parameters_to_context(codecContext, stream->codecParam);
    //获取解码器
    AVCodec* avCodec = avcodec_find_decoder(codecContext->codec_id);
    //
    int ret = avcodec_open2(codecContext, avCodec, nullptr);
    if(ret)
    {
        cout << "avcodec_open2 fail!!" << endl;
        return -1;
    }

    return 0;
}


int MMAVDecoder::SendPacket(MMAVPacket* avPkt)
{
    int ret = 0;
    
    if(avPkt == nullptr)
    {
        ret = avcodec_send_packet(codecContext, nullptr);
    }
    else
    {
        ret = avcodec_send_packet(codecContext, avPkt->pkt);
    }
    
    return ret;
}


int MMAVDecoder::RecvFrame(MMAVFrame *avFrame)
{
    int ret = avcodec_receive_frame(codecContext, avFrame->frame);
    if(!ret)
    {
        // 将秒级时间戳计算并且赋值
        avFrame->ptsSec = avFrame->frame->pts * 1.0 * timebaseNum / timebaseDen;
    }
    return ret;
}

int MMAVDecoder::Close()
{
    avcodec_close(codecContext);
    return 0;
}