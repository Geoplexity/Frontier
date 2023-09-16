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

    void output_pebbles(std::ostream& out, const std::vector<int>& pebbles) {
        out << "xlabel=<";

        std::string open_highlight = R"(<FONT COLOR="RED">)";
        std::string close_highlight = "</FONT>";

        for (const auto &p : pebbles) {
            if (pebble_applies_to_move(p)) {
                out << open_highlight;
                out << " " << p << " ";
                out << close_highlight;
            } else {
                out << " " << p << " ";
                //out << "⬤";
            }
        }

        out << ">";
    }

    void operator()(std::ostream& out, const FlowGraph::vertex_descriptor & v) {
        std::string name = (*graph)[v];
        out << "[";
        output_label(out, name);

        if (tracker != nullptr && cluster->includes_vertex(v)) {
            out << ",";
            std::vector<int> pebbles;
            tracker->pebbles_on_vertex(v, pebbles);
            output_pebbles(out, pebbles);
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
            std::vector<int> pebbles;
            tracker->pebbles_on_edge(v, pebbles);
            output_pebbles(out, pebbles);
            out << ", color=blue";
        }

        out << "]";
    }

private:
    [[nodiscard]] bool pebble_applies_to_move(const int& pebble_id) const {
        for (const auto &m : *moves) {
            if (m.pebble_id() == pebble_id) {
                return true;
            }
        }

        return false;
    }
};

std::unique_ptr<FlowGraph> load_graph(const std::string& path) {
    std::cout << "Reading input file: " << path;

    std::ifstream dot_file(path);

    auto result = std::make_unique<FlowGraph>();

    boost::dynamic_properties dp(boost::ignore_other_properties);

    boost::read_graphviz(dot_file, *result, dp);

    dot_file.close();

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Expected single argument: path to dot file for graph.");
    }

    std::shared_ptr<FlowGraph> graph = load_graph(argv[1]);

    auto cluster = Cluster::Builder::of_graph(graph);

    auto game = std::make_shared<PebbleGame>(cluster);

    int step = 0;

    GraphPropertyWriter graph_property_writer;

    auto result = game->run([&graph_property_writer, &step, &graph, &cluster](const auto &evt, const auto& tracker){

        std::cout << evt.size() << std::endl;

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