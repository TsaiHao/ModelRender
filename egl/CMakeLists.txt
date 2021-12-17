cmake_minimum_required(VERSION 3.6)

project("nativeapplacation")

add_subdirectory(../glbase)

add_library(native-lib SHARED native-lib.cpp GLRender.cpp)

find_library(log-lib log)

target_link_libraries(native-lib ${log-lib} EGL GLESv3 android glbase)