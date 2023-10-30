cmake_minimum_required(VERSION 3.21)
project(set_cover)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost)
add_subdirectory(dbs)
add_executable(set_cover setcover.cpp)

target_include_directories(set_cover PUBLIC
    /usr/include
    dbs/include
    ${Boost_INCLUDE_DIRS})
target_link_libraries(set_cover PUBLIC dbs)