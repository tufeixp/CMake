cmake_minimum_required(VERSION 3.0)

add_executable(TextClang main.cpp)
set_property(TARGET TextClang PROPERTY VS_PLATFORM_TOOLSET_OVERRIDE "v140_clang_3_7")

add_executable(TextVS main.cpp)

