add_executable(
        ffnx_GraphTests
        FlowGraphTest.cpp
        DirectedGraphTest.cpp
        InterfaceTest.cpp)

target_link_libraries(ffnx_GraphTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_GraphTests)
