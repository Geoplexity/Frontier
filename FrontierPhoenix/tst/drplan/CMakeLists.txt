add_executable(
        ffnx_PebbleGameTests
        PebbleTrackerTest.cpp
        PebbleGameTest.cpp)

target_link_libraries(ffnx_PebbleGameTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_PebbleGameTests)
