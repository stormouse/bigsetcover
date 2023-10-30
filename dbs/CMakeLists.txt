add_library(dbs dbs.cpp)
target_include_directories(dbs 
    PRIVATE include
    PUBLIC /usr/include
    PUBLIC ${Boost_INCLUDE_DIRS})