//file: mfa5GetC.cpp
/*contains two methods: copying  graph and sequential extensions for
  the DR-planner*/


#ifndef CLUSTER_GET_HPP
#define CLUSTER_GET_HPP

#include <iostream>

#include "../GDS/GDS.hpp"


// moved into GDS/Graph.hpp as a method
// // copy a graph and return K value
// int copyG(Graph &g0, Graph &g1);



//checks for special cases that should be extended
bool checkSpecialExtensionCase(Graph &F, std::ostream &out);



// //check whether the current labeld vertices in F are able to be extended
// bool canExtend(Graph &F, Graph &graph0, List<Cluster> &DR_Trees, int groupID, List<int> &groupV);



//performs a sequential extension on the labeled section of F, sequential extensions are described in the
//attached paper documentation
int extension(Graph &F, Graph &graph0, std::ostream &file1, std::ostream &file2);



int extension(Graph &F, Graph &graph0, bool firstTime, int groupID,
 	std::ostream &file1, std::ostream &file2);



// After distribute has been run, Graph F contains vertices labeled for the
// creation of a new cluster this method isolates those vertices, performs any
// reductions to the interior vertices and edges as necessary, and creates a
// new Cluster object, returning a pointer to it for more on the specifics of
// his reduction process, see attached paper documentation
Cluster * getCluster(Graph &F, Graph &graph0);


#endif
