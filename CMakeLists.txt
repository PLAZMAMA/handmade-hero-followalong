cmake_minimum_required(VERSION 3.28)

project(
    handmade_followalong
    VERSION 0.0.1
    DESCRIPTION "Hand made follow along project with the hopes of spinning this into my own engine which I can build my games on"
    LANGUAGES CXX
)

add_executable(win32 WIN32 src/win32.cpp)
