add_library(frontier-phoenix SHARED
        hypergraph/Annotator.cpp
        hypergraph/sparse/Sparse.cpp
        hypergraph/sparse/Edge.cpp
        hypergraph/EventDispatching.cpp
        mapping/hyper_to_flow/SketchMapper.cpp
        sketcher/Constraint.cpp
        sketcher/Shape.cpp
        sketcher/sketcher.cpp)

target_include_directories(frontier-phoenix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(frontier-phoenix ${GINAC_LIBRARIES} boost_regex boost_graph)
