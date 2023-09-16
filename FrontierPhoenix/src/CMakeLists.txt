add_library(frontier-phoenix SHARED
        hypergraph/Annotator.cpp
        hypergraph/sparse/Sparse.cpp
        hypergraph/sparse/Edge.cpp
        hypergraph/EventDispatching.cpp
        mapping/hyper_to_flow/SketchMapper.cpp
        sketcher/Constraint.cpp
        sketcher/Shape.cpp
        sketcher/sketcher.cpp
        cluster/Cluster.h
        cluster/commands/Condensation.h
        cluster/commands/Condensation.cpp
        drplan/DRPlanner.h
        drplan/DRPlan.h
        drplan/plans/canonical_top_down/CanonicalTopDown.h
        tree/TreeNode.h)

target_include_directories(frontier-phoenix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(frontier-phoenix ${GINAC_LIBRARIES} boost_regex boost_graph)
