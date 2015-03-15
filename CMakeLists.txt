cmake_minimum_required(VERSION 2.8.4)
project(eternity)

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

    libraries/lrx_cross_api/src/exception/socket_exception.cpp
    libraries/lrx_cross_api/src/exception/socket_exception.h
    libraries/lrx_cross_api/src/exception/tts_exception.cpp
    libraries/lrx_cross_api/src/exception/tts_exception.h
    libraries/lrx_cross_api/src/exception/vr_exception.cpp
    libraries/lrx_cross_api/src/exception/vr_exception.h

    libraries/lrx_cross_api/src/network_io/io_service_pool.cpp
    libraries/lrx_cross_api/src/network_io/io_service_pool.h
    libraries/lrx_cross_api/src/network_io/remote_command.pb.cc
    libraries/lrx_cross_api/src/network_io/remote_command.pb.h
    libraries/lrx_cross_api/src/network_io/request_handler.h
    libraries/lrx_cross_api/src/network_io/serialized_message.cpp
    libraries/lrx_cross_api/src/network_io/serialized_message.h
    libraries/lrx_cross_api/src/network_io/server_config.cpp
    libraries/lrx_cross_api/src/network_io/server_config.h
    libraries/lrx_cross_api/src/network_io/tcp_connection.cpp
    libraries/lrx_cross_api/src/network_io/tcp_connection.h
    libraries/lrx_cross_api/src/network_io/tcp_server.cpp
    libraries/lrx_cross_api/src/network_io/tcp_server.h

    libraries/lrx_cross_api/src/network_io.cpp
    libraries/lrx_cross_api/src/network_io.h
    libraries/lrx_cross_api/src/text_to_speech.h
    libraries/lrx_cross_api/src/voice.cpp
    libraries/lrx_cross_api/src/voice.h
    libraries/lrx_cross_api/src/voice_recognition.h

    libraries/lrx_core/Utils/src/exception/Exception.cpp
    libraries/lrx_core/Utils/src/exception/Exception.h
    libraries/lrx_core/Utils/src/file_handler.cpp
    libraries/lrx_core/Utils/src/file_handler.h
    libraries/lrx_core/Utils/src/fs_utils.cpp
    libraries/lrx_core/Utils/src/fs_utils.h
    libraries/lrx_core/Utils/src/lexicon.cpp
    libraries/lrx_core/Utils/src/lexicon.h
    libraries/lrx_core/Utils/src/lexicon_manager.cpp
    libraries/lrx_core/Utils/src/lexicon_manager.h
    libraries/lrx_core/Utils/src/logger.cpp
    libraries/lrx_core/Utils/src/logger.h
    libraries/lrx_core/Utils/src/platform_config.h
    libraries/lrx_core/Utils/src/properties.cpp
    libraries/lrx_core/Utils/src/properties.h
    libraries/lrx_core/Utils/src/string_utils.cpp
    libraries/lrx_core/Utils/src/string_utils.h
)

add_executable(eternity ${SOURCE_FILES})
add_definitions(-std=c++11)
