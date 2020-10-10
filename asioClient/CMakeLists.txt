cmake_minimum_required(VERSION 3.0.0)
project(asioClient VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(asioClient main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT C:/Users/chy/Desktop/boost_1_72_0)
set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    set(Json_INCLUDE_DIRS C:/Users/chy/Desktop/Lot_c)
    include_directories(${Json_INCLUDE_DIRS})
endif()

if(MINGW)
    target_link_libraries(asioClient ws2_32)
endif()

include(CPack)
