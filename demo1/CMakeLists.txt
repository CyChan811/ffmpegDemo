cmake_minimum_required(VERSION 3.8)

project("Main")

#链接库的目录
link_directories(/usr/local/lib)

#头文件目录
include_directories(/usr/local/include/libavformat)
#include_directories(/usr/local/include/libavutil)

#设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

add_executable(
    Main 
    Main.cpp
    MMAV.cpp 
    MMAVPacket.cpp 
    MMAVReader.cpp
    MMAVDecoder.cpp
    MMAVStream.cpp
    MMAVFrame.cpp
    MMThread.cpp
    MMPlayer.cpp
    MMPlayerReaderThread.cpp
    MMPlayerDecoderThread.cpp
    MMPlayerCtr.cpp
    MMAVTime.cpp
)

#目标需要链接的库
target_link_libraries(Main avcodec)
target_link_libraries(Main avdevice)
target_link_libraries(Main avformat)
target_link_libraries(Main avutil)
target_link_libraries(Main swresample)
target_link_libraries(Main swscale)