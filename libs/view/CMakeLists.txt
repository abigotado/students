add_library(view STATIC)

target_include_directories(view PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(view PRIVATE src/View.cpp)
 
target_link_libraries(view PUBLIC model) 