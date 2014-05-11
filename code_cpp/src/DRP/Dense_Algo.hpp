/* The minimal dense algorithm used by the DR-planner. */


#ifndef DENSE_ALGO_HPP
#define DENSE_ALGO_HPP


#include <iostream>

#include "../GDS/GDS.hpp"


// input initialized CQueue--all the single vertices of the original graph

// this is similar to algorithm dense, algorithm dense is described in paper documentation
List<Cluster> & mfaAlgo(Graph &graph1, List<Cluster> &DRTrees,
   std::ostream &file1, std::ostream &file2);

#endif
