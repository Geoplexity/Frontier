add_executable(
        ffnx_MappingTests
        HyperToFlowTest.cpp)

target_link_libraries(ffnx_MappingTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_MappingTests)
