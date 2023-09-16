#include <boost/graph/adjacency_list.hpp>
#include <utility>
#include "cluster/Cluster.h"
#include "drplan/plans/canonical_top_down/PebbleGame2D.h"
#include <fstream>
#include <sstream>

using PebbleGame = ffnx::pebblegame::PebbleGame2D<std::string, std::string>;
using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;
using Cluster = ffnx::cluster::Cluster<FlowGraph>;
using PebbleTracker = ffnx::pebblegame::PebbleTracker<FlowGraph::vertex_descriptor, FlowGraph::edge_descriptor>;

class GraphPropertyWriter {
public:
    void operator()(std::ostream& out) const {
        out << R"(graph [pad="0.5", nodesep="1", ranksep="1"])" << std::endl;
    }
};

class VertEdgeWriter {
private:
    std::shared_ptr<FlowGraph> graph;
    std::shared_ptr<Cluster> cluster;
    const PebbleTracker* tracker;
    const std::vector<PebbleTracker::PebbleMovement>* moves;

public:
    VertEdgeWriter(
            std::shared_ptr<FlowGraph> graph,
            std::shared_ptr<Cluster> cluster,
            const PebbleTracker* tracker,
            const std::vector<PebbleTracker::PebbleMovement>* moves) :
                graph(std::move(graph)),
                cluster(cluster),
                tracker(tracker),
                moves(moves) {

    }

    void output_label(std::ostream& out, const std::string& label) {
        out << "label=\"" << label << "\"";
    }

    void output_pebbles(std::ostream& out, const int& pebble_count) {
        out << "xlabel=\"";

        for (int i = 0; i < pebble_count; i++) {
            out << "⬤";
        }

        out << "\"";
    }

    void operator()(std::ostream& out, const FlowGraph::vertex_descriptor & v) {
        std::string name = (*graph)[v];
        out << "[";
        output_label(out, name);

        if (tracker != nullptr && cluster->includes_vertex(v)) {
            out << ",";
            output_pebbles(out, tracker->pebbles_on_vertex(v));
            out << ", color=blue";
        }

        out << "]";
    }

    void operator()(std::ostream& out, const FlowGraph::edge_descriptor & v) {
        std::string name = (*graph)[v];
        out << "[";
        output_label(out, name);

        if (tracker != nullptr && cluster->includes_edge(v)) {
            out << ",";
            output_pebbles(out, tracker->pebbles_on_edge(v));
            out << ", color=blue";
        }

        out << "]";
    }
};

int main() {

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto v2 = graph->add_vertex();
    auto v3 = graph->add_vertex();
    auto v4 = graph->add_vertex();
    auto v5 = graph->add_vertex();


    auto e = graph->add_edge(v0, v1);
    graph->add_edge(v1, v2);
    graph->add_edge(v1, v3);

    graph->add_edge(v3, v4);
    graph->add_edge(v3, v5);

    //std::shared_ptr<ffnx::cluster::Cluster<std::string, std::string>> cluster = ffnx::cluster::Cluster<std::string, std::string>::Builder(graph)
    //        .add_vertex(v0)
    //        .add_vertex(v1)
    //        .add_edge(e)
    //        .build();
    auto cluster = Cluster::Builder::of_graph(graph)->get_vert_filtered_cluster([&v0](const auto &v){ return v != v0; });

    auto game = std::make_shared<PebbleGame>(cluster);

    int step = 0;

    GraphPropertyWriter graph_property_writer;

    auto result = game->run([&graph_property_writer, &step, &graph, &cluster](const auto &evt, const auto& tracker){
        std::stringstream ss;
        ss << "g" << step << ".dot";
        std::ofstream graph_file;
        graph_file.open(ss.str());

        auto ve_writer = std::make_shared<VertEdgeWriter>(graph, cluster, &tracker, &evt);

        boost::write_graphviz(graph_file, *graph, *ve_writer, *ve_writer, graph_property_writer);
        graph_file.close();

        step++;
    });


}