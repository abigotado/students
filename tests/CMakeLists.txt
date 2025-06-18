include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE GTest::gtest_main model)

include(GoogleTest)
gtest_discover_tests(run_tests) 