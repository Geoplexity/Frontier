#include "mfa7DRTree.h"
#include "mfa8Algo.h"

#include <memory>

using namespace ffnx::mfa;

std::unique_ptr<Graph> prepare_test_graph() {
    List<Vertex> v_list;
    List<Edge> e_list;

    Vertex v0(0, VertexType::point);
    v0.initialValue(static_cast<int>(DegValuesIndexResolver::Point::X), 0.0f, FreeDegsValues::NOT_FREE);
    v0.initialValue(static_cast<int>(DegValuesIndexResolver::Point::Y), 0.0f, FreeDegsValues::NOT_FREE);

    Vertex v1(1, VertexType::point);
    v1.initialValue(static_cast<int>(DegValuesIndexResolver::Point::X), 1.0f, FreeDegsValues::FREE);
    v1.initialValue(static_cast<int>(DegValuesIndexResolver::Point::Y), 1.0f, FreeDegsValues::FREE);

    v_list.append(v0);
    v_list.append(v1);

    Edge e;
    e.setEnd(0, 0);
    e.setEnd(1, 1);
    e.setType(EdgeType::distance);
    e.setValue(1);

    e_list.append(e);

    auto graph = std::make_unique<Graph>();
    graph->setVlist(v_list);
    graph->setElist(e_list);

    return std::move(graph);
}

std::unique_ptr<List<Cluster>> get_initial_cluster_list(Graph& graph) {
    auto result = std::make_unique<List<Cluster>>();

    Cluster dr_tree_root;

    for (int i = 0; i < graph.returnNumVer(); i++) {
        Cluster cluster;
        List<int> frontiers;
        List<int> originalV;
        List<Edge> innerE, outerE;
        cluster.formCl(graph.returnVertByIndex(i), frontiers, innerE, outerE, originalV);

        dr_tree_root.children.append(cluster);
    }

    result->append(dr_tree_root);

    return std::move(result);
}

int main() {
    auto test_graph = prepare_test_graph();

    auto dr_trees = get_initial_cluster_list(*test_graph);

    mfaAlgo(
            *test_graph,
            *dr_trees,
            std::cout,
            std::cout);

    return 0;
}
