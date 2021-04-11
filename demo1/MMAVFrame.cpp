#include "MMAV.h"


MMAVFrame::MMAVFrame()
{
    frame = av_frame_alloc();

}


MMAVFrame::~MMAVFrame()
{
    if(frame != nullptr)
    {
        av_frame_free(&frame);
        frame = nullptr;
    }
}

int MMAVFrame::VideoPrint()
{
    cout << "Width:" << frame->width << endl;
    cout << "Height:" << frame->height << endl;

    AVPixelFormat format = (AVPixelFormat)frame->format;
    
    char* str = (char*)malloc(128);
    str = av_get_pix_fmt_string(str, 128, format);

    cout << "pixel format:" << str << endl;

    free(str);

    return 0;
}

int MMAVFrame::AudioPrint()
{
    int channel = frame->channels;

    cout << "channel:" << channel << endl;
    cout << "nb_samples:" << frame->nb_samples << endl;
    cout << "sample_rate:" << frame->sample_rate << endl;

    AVSampleFormat format = (AVSampleFormat)(frame->format);

    char* str = (char*)malloc(128);
    str = av_get_sample_fmt_string(str, 128, format);

    cout << "sample format:" << str << endl;

    free(str);
}

int MMAVFrame::GetW()
{
    return frame->width;
}


int MMAVFrame::GetH()
{
    return frame->height;
}


int MMAVFrame::GetY(unsigned char *y)
{
    int width = GetW();
    int height = GetH();

    for(int i = 0; i < height; i++)
    {
        memcpy(y + i * width, frame->data[0] + i * frame->linesize[0], width);
    }

    return 0;
}


int MMAVFrame::GetU(unsigned char *u)
{
    int width = GetW() / 2;
    int height = GetH() / 2;

    for(int i = 0; i < height; i++)
    {
        memcpy(u + i * width, frame->data[1] + i * frame->linesize[1], width);
    }
    return 0;
}


int MMAVFrame::GetV(unsigned char *v)
{
    int width = GetW() / 2;
    int height = GetH() / 2;

    for(int i = 0; i < height; i++)
    {
        memcpy(v + i * width, frame->data[2] + i * frame->linesize[2], width);
    }
    return 0;
}


long long MMAVFrame::GetPts()
{
    return (long long)(ptsSec * 1000);
}