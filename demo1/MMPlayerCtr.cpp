#include "MMPlayer.h"

MMPlayerCtr::MMPlayerCtr(string _path, double _seekTime)
{
    path = _path;
    seekTime = _seekTime;
}


MMPlayerCtr::~MMPlayerCtr()
{

}

void MMPlayerCtr::run()
{
    MMPlayerReaderThread readerThread(path, this, seekTime);
    readerThread.Start();
    //获取线程启动时候的时间start_time
    long long startTime = MMAVTime::GetTime();

    MMAVFrame* videoFrame = nullptr;
    MMAVFrame* audioFrame = nullptr;

    //一共sleep了多长时间
    long long sleepCountTime = 0;

    while(!stopFlag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long long sleepTimeStart = MMAVTime::GetTime();

        while(status == MMPlayerCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        long long sleepTimeEnd = MMAVTime::GetTime();
        long long sleepDTime = sleepTimeEnd - sleepTimeStart;
        sleepCountTime += sleepDTime;

        //获取当前时间now_time
        long long nowTime = MMAVTime::GetTime();

        //获取当前时间和开始时间的差值d_time
        long long dTime = nowTime - startTime;

        //暂停了就得调整dTime
        dTime = dTime - sleepCountTime;

        //seek
        dTime = dTime + (long long)(seekTime * 1000);

        // 从视频缓存队列中，获取一帧视频 frame_pts
        if(videoFrame == nullptr)
        {
            videoQueue.Pop(&videoFrame);
        }

        //seek之后，I帧可能不是在seektime时刻，那么就得往前找I帧，但这些帧不需要播放出来
        if(videoFrame != nullptr)
        {
            if(videoFrame->GetPts() < (long long)(seekTime * 1000))
            {
                delete videoFrame;
                videoFrame = nullptr;
            }
        }
        
        if(videoFrame != nullptr)
        {
            // 如果frame_pts <= d_time
            if(videoFrame->GetPts() <= dTime)
            {
                // 这帧视频，应该立即播放出来
                cout << "video frame:" << videoFrame->GetPts() << endl;
                delete videoFrame;
                videoFrame = nullptr;
            }
            // 否则
                // 这帧视频还不到播放的时候，程序进行自旋，或者去处理音频
            else
            {
                
            }
            
            
        }
        


        
        // 从音频缓存队列中，获取一帧音频 frame_pts
        if(audioFrame == nullptr)
        {
            audioQueue.Pop(&audioFrame);
        }

        if(audioFrame != nullptr)
        {
            if(audioFrame->GetPts() < (long long)(seekTime * 1000))
            {
                delete audioFrame;
                audioFrame = nullptr;
            }
        }

        if(audioFrame != nullptr)
        {
            // 如果frame_pts <= d_time
            if(audioFrame->GetPts() <= dTime)
            {
                // 这帧音频，应该立即播放出来
                
                delete audioFrame;
                audioFrame = nullptr;
            }
            // 否则
                // 这帧音频还不到播放的时候，程序进行自旋，或者去处理视频
            else
            {
                
            }
            
        }
        
    }
    readerThread.Stop();
}


int MMPlayerCtr::GetVideoQueueSize()
{
    return videoQueue.Size();
}


int MMPlayerCtr::GetAudioQueueSize()
{
    return audioQueue.Size();
}

int MMPlayerCtr::PushFrameToVideoQueue(MMAVFrame* frame)
{
    return videoQueue.Push(frame);
}


int MMPlayerCtr::PushFrameToAudioQueue(MMAVFrame* frame)
{
    return audioQueue.Push(frame);
}

int MMPlayerCtr::Play()
{
    status = MMPlayerCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
    return 0;
}

int MMPlayerCtr::Pause()
{
    status = MMPlayerCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING;
    return 0;
}