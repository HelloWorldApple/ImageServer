project(Server)
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

aux_source_directory(. SRC_LIST)
include_directories(../Jnetlib)##include extern dir
aux_source_directory(../Jnetlib SRC_LIST)

find_package(Threads)
find_package( OpenCV REQUIRED )

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
