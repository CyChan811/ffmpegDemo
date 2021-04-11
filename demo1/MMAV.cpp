// #include <iostream>
// #include "MMAV.h"
// #include <vector>
// #include "MMQueue.h"

// int main()
// {
//     MMQueue<MMAVPacket> packetQueue;
//     MMAVReader reader;

//     int ret = reader.Open("../../test.mp4");

//     if(ret)
//     {
//         printf("Open File Fail!!!!\n");
//         return -1;
//     }

//     int videoStreamIndex = reader.GetVideoStreamIndex();

//     int audioStreamIndex = reader.GetAudioStreamIndex();

//     vector<MMAVDecoder*> decoderList;

//     int streamCount = reader.GetStreamCount();
//     for(int i = 0;i < streamCount; i++)
//     {   
//         MMAVStream avStream;
//         reader.GetStream(&avStream, i);

//         cout << "StreamIndex:" << avStream.streamIndex << endl;

//         MMAVDecoder* decoder = new MMAVDecoder();
//         int ret = decoder->Init(&avStream);
//         if(ret)
//         {
//             cout << "Init decoder falil!!" << endl;
//         }

//         decoderList.push_back(decoder);
//     }

//     FILE *f = fopen("./test.yuv", "wb");

//     while(1)
//     {
//         MMAVPacket *pkt = new MMAVPacket();
//         ret = reader.Read(pkt);
//         if(ret)
//         {
//             break;
//         }
//         //printf("Read Packet Success %d");

//         packetQueue.Push(pkt);

//         int streamIndex = pkt->GetIndex();

//         MMAVDecoder* decoder = decoderList[streamIndex];

//         ret = decoder->SendPacket(pkt);
//         if(ret)
//         {
//             continue;
//         }

//         //循环recv 由于存在B帧
//         while(1)
//         {
//             MMAVFrame frame;
//             ret = decoder->RecvFrame(&frame);
//             if(ret)
//             {
//                 break;
//             }
//             //recv success
//             if(streamIndex == videoStreamIndex)
//             {
//                 frame.VideoPrint();

//                 int width = frame.GetW();
//                 int height = frame.GetH();

//                 unsigned char* y = (unsigned char*)malloc(width * height);
//                 unsigned char* u = (unsigned char*)malloc(width / 2 * height / 2);
//                 unsigned char* v = (unsigned char*)malloc(width / 2* height / 2);

//                 frame.GetY(y);
//                 frame.GetU(u);
//                 frame.GetV(v);

//                 fwrite(y, width * height, 1, f);
//                 fwrite(u, width / 2* height / 2, 1, f);
//                 fwrite(v, width / 2* height / 2, 1, f);

//                 free(y);
//                 free(u);
//                 free(v);
//             }
//             else if(streamIndex == audioStreamIndex)
//             {
//                 frame.AudioPrint();
//             }


            
//         }
//     }

//     //读取失败之后还需要再读取一次缓存帧
//     for(int i = 0; i< decoderList.size(); i++)
//     {
//         MMAVDecoder* decoder = decoderList[i];

//         ret = decoder->SendPacket(nullptr); //传nullptr
//         if(ret)
//         {
//             continue;
//         }

//         //循环recv 由于存在B帧
//         while(1)
//         {
//             MMAVFrame frame;
//             ret = decoder->RecvFrame(&frame);
//             if(ret)
//             {
//                 break;
//             }

//             //recv success
//         }
//     }

//     while(packetQueue.Size() > 0)
//     {
//         MMAVPacket *pkt = nullptr;
//         packetQueue.Pop(&pkt);
//         cout << "packetSize:" << packetQueue.Size() << endl;

//         if(pkt == nullptr)
//         {
//             delete(pkt);
//         }
//     }

//     reader.Close();

//     //释放内存
//     for(int i = 0; i < decoderList.size(); i++)
//     {
//         MMAVDecoder* decoder = decoderList[i];
//         decoder->Close();
//         delete decoder;
//     }

//     decoderList.clear();

//     fclose(f);
//     return 0;
// }
