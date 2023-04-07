add_library(frontier-phoenix SHARED
        hypergraph/Annotator.cpp
        hypergraph/Sparse.cpp
        hypergraph/EventDispatching.cpp
        mapping/MappedGraph.cpp
        mapping/SketchMapper.cpp
        sketcher/Constraint.cpp
        sketcher/Shape.cpp
        sketcher/sketcher.cpp)

target_include_directories(frontier-phoenix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(frontier-phoenix ${GINAC_LIBRARIES} boost_regex boost_graph)
