cmake_minimum_required(VERSION 2.8)
project(Edge)
find_package( OpenCV )
add_executable(Edge exp3.cpp)
target_link_libraries(Edge ${OpenCV_LIBS})