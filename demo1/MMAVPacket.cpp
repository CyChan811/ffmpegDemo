#include "MMAV.h"

MMAVPacket::MMAVPacket()
{
	pkt = av_packet_alloc();
}

MMAVPacket::~MMAVPacket()
{
	if(pkt != nullptr)
	{
		av_packet_free(&pkt);
		pkt = nullptr;
	}
}


int MMAVPacket::GetIndex()
{
	return pkt->stream_index;
}