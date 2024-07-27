set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDES src/*.cpp)
add_library(vofa_bridge OBJECT ${srcs})
target_include_directories(vofa_bridge PUBLIC include)

add_subdirectory(intermediary)

if (vlog_BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()
