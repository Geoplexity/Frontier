//file: mfa4Grop.cpp

#include "Group.hpp"

#include <fstream>

#include "Cluster_Print.hpp"


Cluster & oneGroup(jint *inputData, Graph &graph0, int &idx)
{
   Cluster *DR_Tree = new Cluster(), childCluster;

   std::ofstream outfile1;
   if (idx < 40)
      outfile1.open("oneGroup.out");
   else
    outfile1.open("oneGroup2.out");

   int i, shapeNum, vName, subGroupNum;
   Vertex childCore;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;

   std::cout<<"Group="<<inputData[idx]<<std::endl;
   outfile1<<"Group(inputData["<<idx<<"]) = "<<inputData[idx]<<std::endl;

   DR_Tree->setGroup(inputData[idx++]);
   shapeNum=inputData[idx++];

   outfile1<<"The original content of DR_Tree is:"<<std::endl;
   outfile1<<"shapeNum(inputData["<<idx-1<<"]) = "<<shapeNum<<std::endl;

   DR_Tree->output(outfile1);

   for(i=0;i<shapeNum;i++)
   {
       vName=inputData[idx++];
       outfile1<<"vName(inputData["<<idx-1<<"]) = " <<vName<<std::endl;
       graph0.defrostOneVert(vName);
       childCore=graph0.returnVertByName(vName);
       frontiers.append(vName);
       originalV.append(vName);
       childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
       childCluster.output(outfile1);

       outfile1<<"The content of DR_Tree is:"<<std::endl;
       DR_Tree->children.append(childCluster);

       DR_Tree->output(outfile1);
       frontiers.makeEmpty();
       originalV.makeEmpty();
   }


   subGroupNum=inputData[idx++];
   outfile1<<"subGroupNum(inputData["<<idx<<"]) = "<<subGroupNum<<std::endl;

   for(i=0;i<subGroupNum;i++)
   {
       outfile1<<"new idx = "<<idx<<std::endl;
       DR_Tree->children.append(oneGroup(inputData, graph0, idx));
       DR_Tree->output(outfile1);
   }

   outfile1.close();
   return *DR_Tree;
}

void getGroups(jint *inputData, Graph &graph0, int &idx, List<Cluster> &DR_Trees)
{
   int i, vName, subGroupNum, len, incidNum, hasAppended;
   Vertex childCore;
   Cluster childCluster, aTree;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;
   idx++;  // -1 default group ID
   idx++;  // 0 == number of shapes for group -1

   std::ofstream outfile1;

   outfile1.open("getgroups.out");

   graph0.output(outfile1);

   graph0.freeze();
   subGroupNum=inputData[idx++];

   outfile1<<"the first idx = "<<idx<<std::endl;
   outfile1<<"subGroupNum = "<<subGroupNum<<std::endl;
   for(i=31;i<46;i++)
   {
       outfile1<<"inputData["<<i<<"] = "<<inputData[i]<<std::endl;
   }
   for(i=0;i<subGroupNum;i++)
   {
       DR_Trees.append(oneGroup(inputData, graph0, idx));

       outfile1<<"idx = "<<idx<<std::endl;

       printForest(DR_Trees, outfile1, 0);
   }

   outfile1<<"singleVertex = "<<singleVertex<<std::endl;

   for(vName=1;vName<=singleVertex;vName++)
   {
      outfile1<<"vName = "<<vName<<std::endl;
      if(graph0.hasVert(vName))
      {
         childCore=graph0.returnVertByName(vName);

         outfile1<<"childCore = "<<childCore<<std::endl;
         if(childCore.isFrozen())
         {
            outfile1<<"childCore is forzen. "<<std::endl;

            hasAppended = 0;

            graph0.defrostOneVert(vName);
            childCore.defrost();
            frontiers.append(vName);
            originalV.append(vName);
            childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);

            childCluster.output(outfile1);

            len=DR_Trees.returnLen();
            for(i=1;i<=len;i++)
            {
            aTree = DR_Trees.pop();

        outfile1<<"the poped tree is:"<<std::endl;
        aTree.output(outfile1);

        int childLen=aTree.children.returnLen();
                outfile1<<"the children number of aTree is : "<<childLen<<std::endl;

                incidNum = 0;
        int v1, edgeName, j;
        Vertex vTemp;
        Edge edge;
        for(j=1;j<=childLen;j++)
        {
                vTemp = aTree.children.retrieve(j).returnCore();
            v1 = vTemp.returnName();
            outfile1<<"the v1 is : "<<v1<<std::endl;
            if (vTemp.returnType()<=6)
            {//vTemp isn't an imaginary object.
                edge = graph0.returnEdgeByEnds(v1, vName);
                outfile1<<"The edge is:"<<edge<<std::endl;
                edgeName = edge.returnName();
                outfile1<<"The edgeName is"<<edgeName<<std::endl;
                    if (edgeName && edge.returnType()==1)
                    incidNum++;
                outfile1<<"incidNum is : "<<incidNum<<std::endl;
            }
        }
        if(incidNum>=2)
        //vName is an imaginary object.
        {
            outfile1<<"add the imaginary object to the cluster."<<std::endl;
            aTree.children.append(childCluster);
            hasAppended = 1;
        }
        DR_Trees.append(aTree);
        }
        if (hasAppended == 0)
            //this object has not been added to the DR_Trees
                DR_Trees.append(childCluster);
            frontiers.makeEmpty();
            originalV.makeEmpty();

        outfile1<<"The current DR_Trees is:"<<std::endl;
        printForest(DR_Trees, outfile1, 0);
         }//if(childCore.isFrozen())
      }//if(graph0.hasVert(vName))
   }//for(vName=1;vName<=singleVertex;vName++)
   graph0.output(outfile1);
   outfile1.close();
}
