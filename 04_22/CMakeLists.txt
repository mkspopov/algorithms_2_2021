get_filename_component(date ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${date}_dijkstra dijkstra.cpp)
