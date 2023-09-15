add_executable(
        ffnx_HyperGraphTests
        AnnotatorTest.cpp
        SparseTest.cpp
        EventDispatchingTest.cpp)

target_link_libraries(ffnx_HyperGraphTests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)
gtest_discover_tests(ffnx_HyperGraphTests)
