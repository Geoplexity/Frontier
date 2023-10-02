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

add_subdirectory(cluster)
add_subdirectory(graph)
add_subdirectory(hypergraph)
add_subdirectory(mapping)
add_subdirectory(sketcher)
add_subdirectory(drplan)

