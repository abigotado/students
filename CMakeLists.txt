cmake_minimum_required(VERSION 3.20)
project(StudentRegistry)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Static analysis settings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang static analyzer
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic")
    
    # Thread sanitizer for multithreading analysis
    option(ENABLE_THREAD_SANITIZER "Enable Thread Sanitizer" OFF)
    if(ENABLE_THREAD_SANITIZER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O1")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
    endif()
    
    # Address sanitizer for memory leak detection
    option(ENABLE_ADDRESS_SANITIZER "Enable Address Sanitizer" OFF)
    if(ENABLE_ADDRESS_SANITIZER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC static analysis
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
    
    # Thread sanitizer for multithreading analysis
    option(ENABLE_THREAD_SANITIZER "Enable Thread Sanitizer" OFF)
    if(ENABLE_THREAD_SANITIZER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O1")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
    endif()
    
    # Address sanitizer for memory leak detection
    option(ENABLE_ADDRESS_SANITIZER "Enable Address Sanitizer" OFF)
    if(ENABLE_ADDRESS_SANITIZER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # MSVC static analysis
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /analyze")
endif()

# Add subdirectories for libraries and the main application
add_subdirectory(libs/model)
add_subdirectory(libs/view)
add_subdirectory(libs/controller)
add_subdirectory(src)
add_subdirectory(tests) 