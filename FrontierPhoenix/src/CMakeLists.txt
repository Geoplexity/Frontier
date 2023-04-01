add_library(frontier-phoenix SHARED
        entities/Constraint.cpp
        entities/MappedGraph.cpp
        entities/Shape.cpp
        entities/Sketch.cpp
        entities/SketchMapper.cpp)

add_executable(boost_graph_test main.cpp)
target_link_libraries(boost_graph_test frontier-phoenix boost_regex boost_graph)