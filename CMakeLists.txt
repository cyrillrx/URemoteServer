cmake_minimum_required(VERSION 2.8.4)
project(game_lib)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11"
)

set(SOURCE_FILES
    URemoteServer/src/main.cpp
    URemoteServer/src/executor.cpp
    URemoteServer/src/executor.h
    URemoteServer/src/AI.cpp
    URemoteServer/src/AI.h
    URemoteServer/src/ai_config.cpp
    URemoteServer/src/ai_config.h
    URemoteServer/src/authorized_users.cpp
    URemoteServer/src/authorized_users.h
    URemoteServer/src/trad_key.h

    ../cpp_cross_api/src/network_io/server_config.h
    ../cpp_cross_api/src/network_io.cpp
    ../cpp_cross_api/src/network_io.h
    ../cpp_cross_api/src/*.h

    ../cpp_utils/Utils/src/file_handler.cpp
    ../cpp_utils/Utils/src/file_handler.h
    ../cpp_utils/Utils/src/*.h
 )
add_executable(game_lib ${SOURCE_FILES})