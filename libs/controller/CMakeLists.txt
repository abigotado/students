add_library(controller STATIC)

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(controller PRIVATE src/Controller.cpp)
 
target_link_libraries(controller PUBLIC view) 