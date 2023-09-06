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

TEST(ClusterTests, testFullGraph)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;
    using TV = FlowGraph::vertex_descriptor;
    using TE = FlowGraph::edge_descriptor;
    using TCluster = ffnx::cluster::Cluster<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    TV v0 = graph->add_vertex();
    TV v1 = graph->add_vertex();

    TE e = graph->add_edge(v0, v1);

    auto cluster = TCluster::Builder::of_graph(graph);

    auto cluster_verts = cluster->vertices();
    auto cluster_edges = cluster->edges();

    ASSERT_EQ(2, cluster_verts.size());
    ASSERT_TRUE(cluster_verts.find(v0) != cluster_verts.end());
    ASSERT_TRUE(cluster_verts.find(v1) != cluster_verts.end());

    ASSERT_EQ(1, cluster_edges.size());
    ASSERT_TRUE(cluster_edges.find(e) != cluster_edges.end());
}


TEST(ClusterTests, testPartialGraph)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;
    using TV = FlowGraph::vertex_descriptor;
    using TE = FlowGraph::edge_descriptor;
    using TCluster = ffnx::cluster::Cluster<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    TV v0 = graph->add_vertex();
    TV v1 = graph->add_vertex();
    TV v2 = graph->add_vertex();
    TV v3 = graph->add_vertex();

    TE e0 = graph->add_edge(v0, v1);
    TE e1 = graph->add_edge(v1, v2);
    TE e2 = graph->add_edge(v2, v3);

    auto cluster = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_verts = cluster->vertices();
    auto cluster_edges = cluster->edges();

    ASSERT_EQ(2, cluster_verts.size());
    ASSERT_TRUE(cluster_verts.find(v0) != cluster_verts.end());
    ASSERT_TRUE(cluster_verts.find(v1) != cluster_verts.end());
    ASSERT_TRUE(cluster_verts.find(v2) == cluster_verts.end());
    ASSERT_TRUE(cluster_verts.find(v3) == cluster_verts.end());

    ASSERT_EQ(2, cluster_edges.size());
    ASSERT_TRUE(cluster_edges.find(e0) != cluster_edges.end());
    ASSERT_TRUE(cluster_edges.find(e1) != cluster_edges.end());
    ASSERT_TRUE(cluster_edges.find(e2) == cluster_edges.end());
}


TEST(ClusterTests, testEquivalent)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;
    using TV = FlowGraph::vertex_descriptor;
    using TE = FlowGraph::edge_descriptor;
    using TCluster = ffnx::cluster::Cluster<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    TV v0 = graph->add_vertex();
    TV v1 = graph->add_vertex();
    TV v2 = graph->add_vertex();
    TV v3 = graph->add_vertex();

    TE e0 = graph->add_edge(v0, v1);
    TE e1 = graph->add_edge(v1, v2);
    TE e2 = graph->add_edge(v2, v3);

    auto cluster_a = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_b = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_c = TCluster::Builder(graph)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    ASSERT_TRUE(cluster_a->is_equivalent(*cluster_b));
    ASSERT_TRUE(cluster_b->is_equivalent(*cluster_a));

    ASSERT_TRUE(TCluster::Builder::of_graph(graph)->is_equivalent(*TCluster::Builder::of_graph(graph)));

    ASSERT_FALSE(cluster_a->is_equivalent(*cluster_c));
    ASSERT_FALSE(cluster_c->is_equivalent(*cluster_a));
    ASSERT_FALSE(cluster_b->is_equivalent(*cluster_c));
    ASSERT_FALSE(cluster_c->is_equivalent(*cluster_b));
}


TEST(ClusterTests, testIncludes)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;
    using TV = FlowGraph::vertex_descriptor;
    using TE = FlowGraph::edge_descriptor;
    using TCluster = ffnx::cluster::Cluster<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    TV v0 = graph->add_vertex();
    TV v1 = graph->add_vertex();
    TV v2 = graph->add_vertex();
    TV v3 = graph->add_vertex();

    TE e0 = graph->add_edge(v0, v1);
    TE e1 = graph->add_edge(v1, v2);
    TE e2 = graph->add_edge(v2, v3);

    auto cluster_root = TCluster::Builder::of_graph(graph);

    auto cluster_0 = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_1 = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();


    auto cluster_1_1 = TCluster::Builder(graph)
            .add_vertex(v1)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_1_1_1 = TCluster::Builder(graph)
            .add_vertex(v1)
            .add_edge(e1)
            .build();

    auto cluster_1_1_2 = TCluster::Builder(graph)
            .add_vertex(v1)
            .add_edge(e0)
            .build();


    auto cluster_1_2 = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_edge(e0)
            .add_edge(e1)
            .build();

    auto cluster_1_2_1 = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_edge(e0)
            .build();

    auto cluster_1_2_2 = TCluster::Builder(graph)
            .add_vertex(v0)
            .add_edge(e1)
            .build();

    ASSERT_TRUE(cluster_root->includes(*cluster_0));
    ASSERT_TRUE(cluster_root->includes(*cluster_1));

    ASSERT_TRUE(cluster_1->includes(*cluster_0));
    ASSERT_TRUE(cluster_0->includes(*cluster_1));

    ASSERT_TRUE(cluster_1->includes(*cluster_1));

    ASSERT_TRUE(cluster_1->includes(*cluster_1_1));

    ASSERT_TRUE(cluster_1_1->includes(*cluster_1_1_1));
    ASSERT_TRUE(cluster_1_1->includes(*cluster_1_1_2));

    ASSERT_FALSE(cluster_1_1_1->includes(*cluster_1_1_2));
    ASSERT_FALSE(cluster_1_1_2->includes(*cluster_1_1));

    ASSERT_TRUE(cluster_1_2->includes(*cluster_1_2_1));

    ASSERT_FALSE(cluster_1_2->includes(*cluster_1_1));
}