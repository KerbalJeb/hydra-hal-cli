add_library(cli INTERFACE)
target_include_directories(cli INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(test)