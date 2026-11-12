cmake_minimum_required(VERSION 3.22)
project(MojaIgra)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

add_library(mojaigra SHARED main.cpp)
target_link_libraries(mojaigra raylib android log EGL GLESv2 m -Wl,--wrap=fopen)
