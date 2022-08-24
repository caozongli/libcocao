cmake_minimum_required(VERSION 3.0)
project(libcocao)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

set (LIB_SRC
        libcocao/address.cc
        libcocao/fd_manager.cc
        libcocao/fiber.cc
        libcocao/hook.cc
        libcocao/http/http-parser/http_parser.c
        libcocao/http/http.cc
        libcocao/http/http_server.cc
        libcocao/http/http_session.cc
        libcocao/http/http_parser.cc
        libcocao/http/servlet.cc
        libcocao/log.cc
        libcocao/iomanager.cc
        libcocao/noncopyable.h
        libcocao/schedule.cc
        libcocao/singleton.h
        libcocao/thread.cc
        libcocao/timer.cc
        libcocao/utils.cc
        )

add_library(libcocao SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(libcocao)

set(LIBS
        libcocao
        dl
        pthread
        )

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log libcocao)
force_redefine_file_macro_for_sources(test_log)
target_link_libraries(test_log ${LIBS})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber libcocao)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber ${LIBS})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook libcocao)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook ${LIBS})

add_executable(test_tcpserver tests/test_tcp_server.cc)
add_dependencies(test_tcpserver libcocao)
force_redefine_file_macro_for_sources(test_tcpserver)
target_link_libraries(test_tcpserver ${LIBS})


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)