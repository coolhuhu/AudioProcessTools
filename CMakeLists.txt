cmake_minimum_required(VERSION 3.14)
project(AudioProcessTools)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(LIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib")
set(INCLUDE "${CMAKE_SOURCE_DIR}/ffmpeg/include")

include_directories(${INCLUDE})
link_directories(${LIB})

add_executable(CalSpeechDuration CalSpeechDuration.cpp)
target_link_libraries(
    CalSpeechDuration 
    avformat
)
