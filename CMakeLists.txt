cmake_minimum_required(VERSION 3.5)
project(vofa_bridge LANGUAGES CXX)

add_library(${PROJECT_NAME} OBJECT ./vofa_bridge.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
