add_executable(
        ffnx_DRPlanTests
        PebbleGameTest.cpp
        DRPlanTest.cpp)

target_link_libraries(ffnx_DRPlanTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_DRPlanTests)
