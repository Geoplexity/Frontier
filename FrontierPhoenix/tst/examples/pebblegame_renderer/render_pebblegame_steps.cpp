#include <boost/graph/adjacency_list.hpp>
#include <utility>
#include "cluster/Cluster.h"
#include "drplan/plans/canonical_top_down/PebbleGame2D.h"
#include <fstream>
#include <sstream>

struct VertProps {
    std::string label;
    std::string height;
    std::string pos;
    std::string width;
};

struct EdgeProps {
    std::string label;
    std::string pos;
};

using UndirectedGraph = ffnx::graph::UndirectedGraph<VertProps, EdgeProps>;
using PebbleGame = ffnx::pebblegame::PebbleGame2D<UndirectedGraph>;
using PebbleGameGraph = ffnx::pebblegame::PebbleGameGraph<UndirectedGraph>;
using Cluster = ffnx::cluster::Cluster<UndirectedGraph>;

class GraphPropertyWriter {
private:
    int step;

public:
    GraphPropertyWriter(const int& step) : step(step) {

    }

    void operator()(std::ostream& out) const {
        out << R"(graph [pad="0.5", nodesep="1", ranksep="1", label=)" << std::endl;
        out << "\"step " << step << "\"]";
    }
};

class VertEdgeWriter {
private:
    std::shared_ptr<UndirectedGraph> graph;
    std::shared_ptr<Cluster> cluster;
    std::shared_ptr<PebbleGame> game;

public:
    VertEdgeWriter(
            std::shared_ptr<UndirectedGraph> graph,
            std::shared_ptr<Cluster> cluster,
            std::shared_ptr<PebbleGame> game) :
                graph(std::move(graph)),
                cluster(std::move(cluster)),
                game(std::move(game)) {

    }

    void output_label(std::ostream& out, const std::string& label) {
        out << "label=\"" << label << "\"";
    }

    void output_pebbles(std::ostream& out, const std::set<int>& pebbles) {
        out << "xlabel=<";

        std::string open_highlight = R"(<FONT COLOR="RED">)";
        std::string close_highlight = "</FONT>";

        for (const auto &p : pebbles) {
            if (false) {
                out << open_highlight;
                out << " " << p << " ";
                out << close_highlight;
            } else {
                //out << " " << p << " ";
                out << "⬤";
            }
        }

        out << ">";
    }

    void operator()(std::ostream& out, const PebbleGameGraph::InternalGraph::vertex_descriptor & v) {
        auto ext_e = game->get_game_graph().external_vert(v);
        std::string name = (*graph)[ext_e].label;
        out << "[";
        output_label(out, name);
        out << ",height=" << (*graph)[ext_e].height;
        out << ",pos=\"" << (*graph)[ext_e].pos << "\"";
        out << ",width=" << (*graph)[ext_e].width;

        if (cluster->includes_vertex(ext_e)) {
            out << ",";
            output_pebbles(out, game->get_game_graph().get_vert_pebbles(v).pebbles());
            out << "";
        }

        out << "]";
    }

    void operator()(std::ostream& out, const PebbleGameGraph::InternalGraph::edge_descriptor & v) {
        auto ext_e = game->get_game_graph().external_edge(v);

        std::string name = (*graph)[ext_e].label;
        out << "[";
        output_label(out, name);

        if (cluster->includes_edge(ext_e)) {
            out << ",";
            output_pebbles(out, game->get_game_graph().get_edge_pebbles(v).pebbles());
            out << "";
        }

        out << "]";
    }

private:
};

std::unique_ptr<UndirectedGraph> load_graph(const std::string& path) {
    std::cout << "Reading input file: " << path;

    std::ifstream dot_file(path);

    auto result = std::make_unique<UndirectedGraph>();

    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id", boost::get(&VertProps::label, *result));
    dp.property("height", boost::get(&VertProps::height, *result));
    dp.property("pos", boost::get(&VertProps::pos, *result));
    dp.property("width", boost::get(&VertProps::width, *result));

    dp.property("node_id", boost::get(&EdgeProps::label, *result));
    dp.property("pos", boost::get(&EdgeProps::pos, *result));

    boost::read_graphviz(dot_file, *result, dp);

    dot_file.close();

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Expected single argument: path to dot file for graph.");
    }

    std::shared_ptr<UndirectedGraph> graph = load_graph(argv[1]);
    auto cluster = Cluster::Builder::of_graph(graph);

    auto v = *(graph->vertices().begin());
    std::cout << "Vertex attributes for vertex" << std::endl;
    std::map<std::string, std::string> v_attr = boost::get(boost::vertex_attribute_t::vertex_attribute, *graph)[v];
    for (const auto& kv : v_attr) {
        std::cout << kv.first << ", " << kv.second << std::endl;
    }

    auto game = std::make_shared<PebbleGame>(cluster);

    int step = 0;

    game->run([&step, &graph, &cluster, &game](){
        if (step > 10) {
            // prevent runaway loop
            return false;
        }

        std::cout << "STEP" << std::endl;

        std::stringstream ss;
        ss << "g" << (step < 10 ? "0" : "") << step << ".dot";
        std::ofstream graph_file;
        graph_file.open(ss.str());

        auto ve_writer = std::make_shared<VertEdgeWriter>(graph, cluster, game);

        GraphPropertyWriter graph_property_writer(step);


        boost::write_graphviz(graph_file, game->get_game_graph().graph(), *ve_writer, *ve_writer, graph_property_writer);
        graph_file.close();

        step++;
        return true;
    });


}