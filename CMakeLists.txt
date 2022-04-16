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
        libcocao/fd_manager.cc
        libcocao/fiber.cc
        libcocao/log.cc
        libcocao/iomanager.cc
        libcocao/noncopyable.h
        libcocao/schedule.cc
        libcocao/singleton.h
        libcocao/thread.cc
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


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)