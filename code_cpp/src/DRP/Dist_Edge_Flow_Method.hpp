//file: mfa6Dist.cpp
/*Contains the distribute-edge flow method  and the pushoutside method
used by the Distribute-forest, Distribute-cluster methods,
and all the copying methods associated with it. */


#ifndef DIST_EDGE_FLOW_METHOD_HPP
#define DIST_EDGE_FLOW_METHOD_HPP

#include <iostream>

#include "../GDS/GDS.hpp"



//determines if cluster is a special case for pushing more weight outside.
bool specialPushCase(Graph &graph0, Cluster &C);


//copies only thet labels from g0 to g1
void copyLabel(Graph &g0, Graph &g1);

//delete inner vertices and edges of newTree from F
void delInnerVer(Graph &F, Graph &graph0);

// distribute edge when K!=0
int distributEdge(Edge &edge, Graph &F, std::ostream &file2);

//remove one flow which is into C. Return the edge name
int removeFlow(Cluster &C, Graph &F, Graph &graph0);


//see attached paper documentation for description of push outside
int pushOutside(Cluster &C, Graph &F, Graph &graph0, std::ostream &file1,
  std::ostream &file2);

//only for trivial dense subgraph
int pushOutside0(Graph &F, std::ostream &file2);

//distribute a single vertex
// this is similar to 'for every edge e incident to v1 do'
Cluster * distributeVert(Vertex &v1, Graph &F, Graph &graph0,
  std::ostream &file1, std::ostream &file2);

// distribute Cluster
Cluster & distributeCl(Cluster &C, Graph &F, Graph &graph0, std::ostream &file1,
  std::ostream &file2);





#endif
