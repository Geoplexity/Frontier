//file: mfa4Grop.cpp

Cluster & oneGroup(jint *inputData, Graph &graph0, int &idx)
{
   Cluster *DR_Tree = new Cluster(), childCluster;

   int i, shapeNum, vName, subGroupNum;
   Vertex childCore;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;

   cout<<"Group="<<inputData[idx]<<endl;
   DR_Tree->setGroup(inputData[idx++]);
   shapeNum=inputData[idx++];
   for(i=0;i<shapeNum;i++)
   {
       vName=inputData[idx++];
       graph0.defrostOneVert(vName);
       childCore=graph0.returnVertByName(vName);
       frontiers.append(vName);
       originalV.append(vName);
       childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
       DR_Tree->children.append(childCluster);
       frontiers.makeEmpty();
       originalV.makeEmpty();
   }

   subGroupNum=inputData[idx++];
   for(i=0;i<subGroupNum;i++)
   {
       DR_Tree->children.append(oneGroup(inputData, graph0, idx));
   }
   return *DR_Tree;
}

void getGroups(jint *inputData, Graph &graph0, int &idx, List<Cluster> &DR_Trees)
{
   int i, vName, subGroupNum;
   Vertex childCore;
   Cluster childCluster;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;
   idx++;  // -1 default group ID
   idx++;  // 0 == number of shapes for group -1

   graph0.freeze();
   subGroupNum=inputData[idx++];
   for(i=0;i<subGroupNum;i++)
   {
       DR_Trees.append(oneGroup(inputData, graph0, idx));
   }
   for(vName=1;vName<=singleVertex;vName++)
   {
      if(graph0.hasVert(vName))
      {
         childCore=graph0.returnVertByName(vName);
         if(childCore.isFrozen()) 
         {
            graph0.defrostOneVert(vName);
            childCore.defrost();
            frontiers.append(vName);
            originalV.append(vName);
            childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
            DR_Trees.append(childCluster);
            frontiers.makeEmpty();
            originalV.makeEmpty();
         }
      }
   }
}
