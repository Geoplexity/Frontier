add_library(frontier-phoenix SHARED
        entities/Constraint.cpp
        entities/MappedGraph.cpp
        entities/Shape.cpp
        entities/Sketch.cpp
        entities/SketchMapper.cpp
        sketcher/sketcher.cpp)

target_link_libraries(frontier-phoenix ${GINAC_LIBRARIES} boost_regex boost_graph)

add_executable(run main.cpp)
target_link_libraries(run frontier-phoenix)
