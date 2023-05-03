# grab google test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(cluster)
add_subdirectory(flowgraph)
add_subdirectory(hypergraph)
add_subdirectory(mapping)
add_subdirectory(sketcher)
