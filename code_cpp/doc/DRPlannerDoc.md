Decomposition and Recombination Planner
=======================================

Input
-----
The DR_Planner takes as input a linked list of clusters containing a list of all those clusters to be distributed.  This list at the minimum must consist of all of the vertices in the graph wrapped as singleton cluster objects.  In addition this list should contain, clusters made from each of the groups defined within the sketcher.  These clusters when formed have only their original vertex fields filled with the lists of the original vertices that make up part of the cluster.  No other parts of the cluster need to be filled.  Finally the list should be arranged so that the user made groups occur in the list before the singleton clusters.


Output
------
The DR_Planner outputs a complete decomposition of the input graph into minimal dense subgraphs. These subgraphs are arranged into a forest of direct acyclic graphs through use of the Cluster objects (see the documentation in mfa2Clas.h and the dataStructuresOverview.txt.)  All of the groups defined in the original input will be found in the output decomposition, unless they did not consistute a valid decomposition.  In this case, they will be ignored a correct decomposition will be generated regardless.  Additional clusters might be found between the levels in the input decomposition, but as long as the input is valid, all input decompsitions will be maintained.  Finally, the DR-Planner will correctly fill out all the clusters datastructures in the DR-Tree, even the ones not filled in the input.  Cores, frontiers, interior vertices, etc.  will all be filled at the end of its processing.
