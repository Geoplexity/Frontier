/* The minimal dense algorithm used by the DR-planner. */

#include "Dense_Algo.hpp"

#include "Cluster_Print.hpp"
#include "Dist_Methods.hpp"


// input initialized CQueue--all the single vertices of the original graph

// this is similar to algorithm dense, algorithm dense is described in paper documentation
List<Cluster> & mfaAlgo(Graph &graph1, List<Cluster> &DRTrees, std::ostream &file1, std::ostream &file2)
{
   int i, j, nCluster, extended, numTrees, number, vertexName, pName;
   Cluster A, C;
   Cluster aDRtree;
   Graph F;       //contain distributed vertices & current edges

   F.setDimen(graph1.returnDimen());

   //print the graph1 (simplified from graph0)
   file1<<"---------------------------------------------"<<std::endl;
   file1<<"simpilfied graph (merge the multi-edges and delete zero-weight edges)"<<std::endl;
   graph1.output(file1);

   //print the DR Input Trees
   file1<<"---------------------------------------------"<<std::endl;
   numTrees=DRTrees.returnLen();
   file1<<std::endl<<numTrees<<" DRTrees by design decomposition:"<<std::endl;
   for(i=1;i<=numTrees;i++)
   {
      file1<<"---------"<<std::endl;
      file1<<"DRTree "<<i<<std::endl;
      printTree(DRTrees.retrieve(i), file1, 1);
   }
   file1<<"---------------------------------------------"<<std::endl;
      DRTrees=distributeForest(DRTrees, F, graph1, file1, file2, 0,0);

   //get trivial clusters from the single left vertices, if possible
   getTrivial(DRTrees, graph1);

   //print the DR Output Trees
   file1<<"---------------------------------------------"<<std::endl;
   numTrees=DRTrees.returnLen();
   file1<<std::endl<<numTrees<<" Final DR output Trees"<<std::endl;
   for(i=1;i<=numTrees;i++)
   {
      file1<<"---------"<<std::endl;
      file1<<"DRTree "<<i<<std::endl;
      printTree(DRTrees.retrieve(i), file1, 1);
   }
   file1<<"---------------------------------------------"<<std::endl;
   std::cout<<std::endl<<numTrees<<" Final DR output Trees"<<std::endl;
   for(i=1;i<=numTrees;i++)
   {
      std::cout<<"---------"<<std::endl;
      std::cout<<"DRTree "<<i<<std::endl;
      printTree(DRTrees.retrieve(i), std::cout, 1);
   }
   std::cout<<"---------------------------------------------"<<std::endl;
   return DRTrees;
}

/*********************************************************************\
 *  Note:                                                            *
 *  in F:                                                            *
 *   Label=1 for necessary extended vertices (avoid trivial dense)   *
 *   Label=2,for well constrained extension                          *
 *   Label=3,for over constrained extension                          *
 *   Scan=-1 for inner vertices of new cluster                       *
\*********************************************************************/
