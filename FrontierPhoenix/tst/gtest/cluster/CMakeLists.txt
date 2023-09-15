add_executable(
        ffnx_ClusterTests
        ClusterTest.cpp)

target_link_libraries(ffnx_ClusterTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_ClusterTests)
