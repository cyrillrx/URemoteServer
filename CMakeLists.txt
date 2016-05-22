PROJECT( cpp_utils )
cmake_minimum_required(VERSION 3.0)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_library(utils
        src/listeners/ConsoleListener.cpp
        src/listeners/ConsoleListener.h
        src/listeners/Listener.cpp
        src/listeners/Listener.h
        src/listeners/URemoteListener.cpp
        src/listeners/URemoteListener.h
        src/listeners/VoiceListener.cpp
        src/listeners/VoiceListener.h
        src/logger/file_logger.cpp
        src/logger/file_logger.h
        src/modules/App.cpp
        src/modules/App.h
        src/modules/file_manager.cpp
        src/modules/file_manager.h
        src/modules/keyboard.cpp
        src/modules/keyboard.h
        src/modules/master_volume.cpp
        src/modules/master_volume.h
        src/modules/MonUtils.cpp
        src/modules/MonUtils.h
        src/AI.cpp
        src/AI.h
        src/ai_config.cpp
        src/ai_config.h
        src/authorized_users.cpp
        src/authorized_users.h
        src/executor.cpp
        src/executor.h
        src/main.cpp
        src/trad_key.h)
target_link_libraries(utils ${CONAN_LIBS})

IF(BUILD_TEST)
    add_executable(test_utils
            test/test_suites.cpp)
    target_link_libraries(test_utils utils ${CONAN_LIBS})
ENDIF()