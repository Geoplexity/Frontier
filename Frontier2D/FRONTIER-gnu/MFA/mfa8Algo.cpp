//file: mfa8Algo.cpp

/*The minimal dense algorithm used  
by the DR-planner.
*/

/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


//input initialized CQueue--all the single vertices of the original graph

// this is similar to algorithm dense, algorithm dense is described in paper documentation
List<Cluster> & mfaAlgo(Graph &graph1, List<Cluster> &DRTrees,
ostream &file1, ostream &file2)
{
   int i, j, nCluster, extended, numTrees, number, vertexName, pName;
   Cluster A, C;
   Cluster aDRtree;
   Graph F;       //contain distributed vertices & current edges

   //print the graph1 (simplified from graph0)
   file1<<"---------------------------------------------"<<endl;
   file1<<"simpilfied graph (merge the multi-edges and delete zero-weight edges)"<<endl;
   graph1.output(file1);

   //print the DR Input Trees
   file1<<"---------------------------------------------"<<endl;
   numTrees=DRTrees.returnLen();
   file1<<endl<<numTrees<<" DRTrees by design decomposition:"<<endl;
   for(i=1;i<=numTrees;i++)
   {
      file1<<"---------"<<endl;
      file1<<"DRTree "<<i<<endl;
      printTree(DRTrees.retrieve(i), file1, 1);
   }
   file1<<"---------------------------------------------"<<endl;
      DRTrees=distributeForest(DRTrees, F, graph1, file1, file2, 0,0);

   //get trivial clusters from the single left vertices, if possible
   getTrivial(DRTrees, graph1);

   //print the DR Output Trees
   file1<<"---------------------------------------------"<<endl;
   numTrees=DRTrees.returnLen();
   file1<<endl<<numTrees<<" Final DR output Trees"<<endl;
   for(i=1;i<=numTrees;i++)
   {
      file1<<"---------"<<endl;
      file1<<"DRTree "<<i<<endl;
      printTree(DRTrees.retrieve(i), file1, 1);
   }
   file1<<"---------------------------------------------"<<endl;
   cout<<endl<<numTrees<<" Final DR output Trees"<<endl;
   for(i=1;i<=numTrees;i++)
   {
      cout<<"---------"<<endl;
      cout<<"DRTree "<<i<<endl;
      printTree(DRTrees.retrieve(i), cout, 1);
   }
   cout<<"---------------------------------------------"<<endl;
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
