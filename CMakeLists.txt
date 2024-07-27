set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDES src/*.cpp)
add_library(vofa_bridge OBJECT ${srcs})
target_include_directories(vofa_bridge PUBLIC include)

find_package(fmt)
target_link_libraries(vofa_bridge PRIVATE fmt)

add_subdirectory(player)
add_subdirectory(example)
if (vlog_BUILD_EXAMPLE)
    
endif ()
