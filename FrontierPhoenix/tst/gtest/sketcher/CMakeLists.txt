add_executable(
        ffnx_SketcherTests
        SketcherTest.cpp)

target_link_libraries(ffnx_SketcherTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_SketcherTests)
