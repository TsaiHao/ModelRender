include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(cg_test simple_test.cpp)

target_link_libraries(cg_test PUBLIC 
    ${LIB_NAME} 
    GTest::gtest_main
    )

target_include_directories(cg_test PUBLIC
    ..
    )

include(GoogleTest)
gtest_discover_tests(cg_test)
