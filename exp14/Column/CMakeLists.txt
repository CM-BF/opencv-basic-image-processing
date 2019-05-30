find_package( OpenCV )
add_library(Column Column.cpp)
target_link_libraries(Column ${OpenCV_LIBS})