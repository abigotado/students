add_executable(student_app main.cpp)
 
target_link_libraries(student_app PRIVATE controller)

add_executable(benchmark benchmark.cpp)

target_link_libraries(benchmark PRIVATE controller) 