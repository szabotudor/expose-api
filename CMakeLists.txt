cmake_minimum_required(VERSION 3.20)

project(ExposeApi)

set(CMAKE_CXX_STANDARD 20)

add_executable(ExposeApi example.cpp expose_api.hpp)
