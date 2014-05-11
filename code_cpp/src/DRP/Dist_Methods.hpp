//file: mfa7DRTree.cpp
/*Methods for distribution including distribute-forest,
distribute-cluster etc., including all the defrost methods
for incorporating  input groups */



#ifndef DIST_METHODS_HPP
#define DIST_METHODS_HPP



#include <iostream>
// #include <fstream>


#include "../GDS/GDS.hpp"


//recursively defrosts and entire forest, used with defrostTree

void defrostTree(Cluster &DR_Tree, Graph &F);

void defrostForest(List<Cluster> &DR_Trees, Graph &F);


//pops the minimal depth cluster in DR_Trees and returns a pointer to it
Cluster &popMinDepth(List<Cluster> &DR_Trees);

//pops the maximum depth cluster in DR_Trees and returns a pointer to it
Cluster &popMaxDepth(List<Cluster> &DR_Trees);

/*Distribute all the clsuters in DR_Trees assuring that the groups set by the user and found in the output
DR_Dag.  Both distributeForest and distributeCluster work recursively together to perform this process
*/

List<Cluster> &distributeForest(List<Cluster> &DRtrees, Graph &F,
  Graph &graph0, std::ostream &file1,std::ostream &file2, int groupID, int indentation);

Cluster &distributeTree(Cluster &aDRtree, Graph &F,
  Graph &graph0, std::ostream &file1, std::ostream &file2, int indentation);

void explicitChild(Cluster &newTree, List<int> &nameList,List<Cluster> &forest);

// int getClusterType(Graph &graph0, Cluster &theCluster);

int getClusterType(Graph &graph0, Cluster &theCluster);

void getTrivial(List<Cluster> &DRTrees, Graph &graph0);

//the cluster is
bool specialCluster(Graph &graph0, Cluster cl);

// a cluster is a sub cluster of another if all of its original vertices are original vertices in the
// larger cluster
bool canCombine (Cluster &c, Cluster &cl0, Graph &F, Graph &graph0);  // subset operator

void implicitChild(Cluster &newTree, List<int> &nameList,
    List<Cluster> &forest, Graph &F, Graph &graph0);

void addChild(Cluster &newTree, List<Cluster> &popedTrees,
    List<Cluster> &DR_Trees, Graph &F, Graph &graph0);

int checkClusterForBananas(Graph &graph0, Cluster &theCluster);




void setAlias(Graph &graph0, Cluster &theCluster);

bool isDense(Cluster &C1, Cluster &C2, Graph &graph0);

// Important note: THIS METHOD WILL NOT CORRECTLY ADD A CLUSTER.  TO BE USED
// ONLY FOR CERTAIN VALIDATION CHECKS ABOUT A CLUSTERS STRUCTURE.
void pseudoAppendChild(Cluster &theCluster, Cluster &newCluster);

void pseudoMinDense(Cluster &theCluster, Graph &graph0);


bool canMerge(Graph &graph0, Cluster &child1, Cluster &child2);

//merge clusters
void mergeCluster(Graph &graph0, int coreName, Cluster &parent,
    Cluster child1, Cluster child2);

//change the depth of the cluster based the depths of its children.
void changeDepth(Cluster &newC);

//modify tree which has 3 or more children if possible.
int modifyTree(Graph &graph0, Cluster &newC);

int setConstrain(Graph &G, Cluster &theCluster);

List<Cluster> &distributeForest(List<Cluster> &DR_Trees, Graph &F,
  Graph &graph0, std::ostream &file1, std::ostream &file2,
  int groupID, int indentation);

// get trivial clusters from the single left vertices, after main distribution
void getTrivial(List<Cluster> &DRTrees, Graph &graph0);





// similar to getEdgePlanner in ESM/ESM.cpp maybe could be consolidated
// into a single method in cluster?

// When the DRP needs to know whether an edge is between or within a cluster,
// the case where it is within must take precedence over the case where it is
// between.  Of course, the solver needs the opposite, thus two methods.
int getEdgeCodeDRP(Edge &theEdge, Cluster &theCluster);


#endif
