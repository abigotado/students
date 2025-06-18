include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE GTest::gtest_main model)

# Отключаем ворнинги для сторонних библиотек (GoogleTest/GoogleMock)
target_compile_options(run_tests PRIVATE 
    $<$<CXX_COMPILER_ID:Clang>:-isystem ${googletest_SOURCE_DIR}/googletest/include>
    $<$<CXX_COMPILER_ID:Clang>:-isystem ${googletest_SOURCE_DIR}/googlemock/include>
    $<$<CXX_COMPILER_ID:GNU>:-isystem ${googletest_SOURCE_DIR}/googletest/include>
    $<$<CXX_COMPILER_ID:GNU>:-isystem ${googletest_SOURCE_DIR}/googlemock/include>
)

include(GoogleTest)
gtest_discover_tests(run_tests) 