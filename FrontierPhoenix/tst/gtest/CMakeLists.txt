enable_testing()

# grab google test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


add_executable(ffnx_all_tests
        cluster/ClusterTest.cpp
        drplan/DRPlanTest.cpp
        drplan/PebbleGameTest.cpp
        graph/DirectedGraphTest.cpp
        graph/FlowGraphTest.cpp
        graph/InterfaceTest.cpp
        hypergraph/AnnotatorTest.cpp
        hypergraph/EventDispatchingTest.cpp
        hypergraph/SparseTest.cpp
        mapping/HyperToFlowTest.cpp
        sketcher/SketcherTest.cpp)

target_link_libraries(ffnx_all_tests GTest::gtest_main GTest::gmock frontier-phoenix)

include(GoogleTest)

gtest_discover_tests(ffnx_all_tests)
#add_subdirectory(cluster)
#add_subdirectory(graph)
#add_subdirectory(hypergraph)
#add_subdirectory(mapping)
#add_subdirectory(sketcher)
#add_subdirectory(drplan)

