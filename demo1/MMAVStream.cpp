#include "MMAV.h"

MMAVStream::MMAVStream()
{
    codecParam = avcodec_parameters_alloc();
}



MMAVStream::~MMAVStream()
{
    if(codecParam != nullptr)
    {
        avcodec_parameters_free(&codecParam);
        codecParam = nullptr;
    }
}