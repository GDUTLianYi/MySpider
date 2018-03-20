cmake_minimum_required(VERSION 3.8)
project(myspider4)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        CSocket.cpp
        CSocket.h
        DNS.cpp
        DNS.h
        FileHelper.cpp
        FileHelper.h
        Http.cpp
        Http.h
        Log.cpp
        Log.h
        main.cpp
        Rio.cpp
        Rio.h
        TaskQueue.cpp
        TaskQueue.h
        TaskQueueSelect.cpp
        TaskQueueSelect.h
        UrlParse.cpp
        UrlParse.h MutexRAII.cpp MutexRAII.h ThreadPool.cpp ThreadPool.h)

add_executable(myspider4 ${SOURCE_FILES})
target_link_libraries(myspider4 /usr/lib/x86_64-linux-gnu/libevent.so)
find_package(Threads REQUIRED)
target_link_libraries(myspider4 Threads::Threads)
target_link_libraries(myspider4 /usr/lib/libboost_regex.so)
target_link_libraries(myspider4 /usr/lib/x86_64-linux-gnu/libz.so)
target_link_libraries(myspider4 /usr/lib/libboost_iostreams.so.1.66.0)