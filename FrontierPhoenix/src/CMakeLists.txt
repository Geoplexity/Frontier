add_library(frontier-phoenix SHARED
        ffnx/hypergraph/Annotator.cpp
        ffnx/hypergraph/sparse/Sparse.cpp
        ffnx/hypergraph/sparse/Edge.cpp
        ffnx/hypergraph/EventDispatching.cpp
        ffnx/mapping/hyper_to_flow/SketchMapper.cpp
        ffnx/sketcher/Constraint.cpp
        ffnx/sketcher/Shape.cpp
        ffnx/sketcher/sketcher.cpp
        ffnx/cluster/Cluster.h
        ffnx/cluster/commands/Condensation.h
        ffnx/cluster/commands/Condensation.cpp
        ffnx/drplan/DRPlanner.h
        ffnx/drplan/DRPlan.h
        ffnx/drplan/plans/canonical_top_down/CanonicalTopDown.h
        ffnx/tree/TreeNode.h)

target_include_directories(frontier-phoenix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(frontier-phoenix ${GINAC_LIBRARIES} boost_regex boost_graph)

install(TARGETS frontier-phoenix)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ffnx
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*_p.h" EXCLUDE
        )
