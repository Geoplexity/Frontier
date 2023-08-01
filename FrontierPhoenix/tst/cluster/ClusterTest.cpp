#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "cluster/Cluster.h"

TEST(ClusterTests, testCreate)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;
    using TV = FlowGraph::vertex_descriptor;
    using TE = FlowGraph::edge_descriptor;
    using TCluster = ffnx::cluster::Cluster<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    TV v0 = graph->add_vertex();
    TV v1 = graph->add_vertex();

    TE e = graph->add_edge(v0, v1);

    auto cluster = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto cluster_verts = cluster->vertices();
    auto cluster_edges = cluster->edges();

    ASSERT_EQ(2, cluster_verts.size());
    ASSERT_TRUE(cluster_verts.find(v0) != cluster_verts.end());
    ASSERT_TRUE(cluster_verts.find(v1) != cluster_verts.end());

    ASSERT_EQ(1, cluster_edges.size());
    ASSERT_TRUE(cluster_edges.find(e) != cluster_edges.end());
}
