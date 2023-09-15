add_executable(
        ffnx_FlowGraphTests
        FlowGraphTest.cpp
        InterfaceTest.cpp)

target_link_libraries(ffnx_FlowGraphTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_FlowGraphTests)
