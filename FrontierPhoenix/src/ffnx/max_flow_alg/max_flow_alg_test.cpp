#include "mfa7DRTree.h"

#include <memory>

using namespace ffnx::mfa;

std::unique_ptr<Graph> prepare_test_graph() {
    List<Vertex> v_list;
    List<Edge> e_list;

    auto graph = std::make_unique<Graph>();
    graph->setVlist(v_list);
    graph->setElist(e_list);

    return std::move(graph);
}

int main() {
    auto test_graph = prepare_test_graph();

    //distributeVert();

    return 0;
}
