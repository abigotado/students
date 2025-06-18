add_library(model STATIC)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(model PRIVATE 
    src/Student.cpp
    src/JuniorStudent.cpp
    src/SeniorStudent.cpp
    src/GraduateStudent.cpp
) 