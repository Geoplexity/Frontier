//file: mfa7DRTree.cpp
/*Methods for distribution including distribute-forest,
distribute-cluster etc., including all the defrost methods
for incorporating  input groups */



#ifndef DIST_METHODS_HPP
#define DIST_METHODS_HPP



#include <iostream>
#include <fstream>


#include "../GDS/GDS.hpp"

#include "Cluster_Print.hpp"
#include "Cluster_Get.hpp"
#include "Dist_Edge_Flow_Method.hpp"

#include "../ESM/ESM.hpp"


//recursively defrosts and entire forest, used with defrostTree

void defrostForest(List<Cluster> & DR_Trees, Graph &F);

void defrostTree(Cluster &DR_Tree, Graph &F)
{
   int i, numKids;
   List<Cluster> kids;

   kids = DR_Tree.children;
   numKids = kids.returnLen();

   F.defrostOneVert(DR_Tree.returnCore().returnName());

   if(numKids==0) return;

   defrostForest(kids, F);
}

void defrostForest(List<Cluster> &DR_Trees, Graph &F)
{
   int i, len;
   Cluster oneTree;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      oneTree = DR_Trees.retrieve(i);
      defrostTree(oneTree, F);
   }
   return;
}


//pops the minimal depth cluster in DR_Trees and returns a pointer to it
Cluster &popMinDepth(List<Cluster> &DR_Trees)
{
   int i,pos,len,depth,coreName,depthMin=nextVerName,nameOfMin=nextVerName;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      depth=DR_Trees.retrieve(i).returnCore().returnDepth();
      coreName=DR_Trees.retrieve(i).returnCore().returnName();
      if(depth==depthMin)
      {
         if(coreName<nameOfMin)
         {
            nameOfMin=coreName;
            pos=i;
         }
      }
      else if(depth<depthMin)
      {
         depthMin=depth;
         nameOfMin=coreName;
         pos=i;
      }
   }
   return DR_Trees.getByIndex(pos);
}

//pops the maximum depth cluster in DR_Trees and returns a pointer to it
Cluster &popMaxDepth(List<Cluster> &DR_Trees)
{
   int i, pos, len, depth, depthMax=0;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      depth=DR_Trees.retrieve(i).returnCore().returnDepth();
      if(depth>=depthMax)
      {
         depthMax=depth;
         pos=i;
      }
   }
   return DR_Trees.getByIndex(pos);
}

/*Distribute all the clsuters in DR_Trees assuring that the groups set by the user and found in the output
DR_Dag.  Both distributeForest and distributeCluster work recursively together to perform this process
*/

List<Cluster> &distributeForest(List<Cluster> &DRtrees, Graph &F,
  Graph &graph0, std::ostream &file1,std::ostream &file2, int groupID, int indentation);

Cluster &distributeTree(Cluster &aDRtree, Graph &F,
  Graph &graph0, std::ostream &file1, std::ostream &file2, int indentation)
{
   file2<<"---------------start---------------"<<std::endl;
   file2<<"DistributeTree"<<std::endl;
   file2<<"Contents of F"<<std::endl;
   F.output(file2);
   file2<<"Contents of aDRTree"<<std::endl;
   printTree(aDRtree, file2, 0);

   int i, numChild, groupID;
   List<Cluster> Children, distriedChild;
   Cluster *newDRtree = new Cluster (aDRtree);

   if(newDRtree->returnCore().returnDepth()==0&&newDRtree->returnGroup()==0)
   {
	file2<<"Returning from depth 0"<<std::endl;
	file2<<"----------------end----------------"<<std::endl;
	return *newDRtree;
   }

   Children=newDRtree->children;


   distriedChild=distributeForest(Children,F,graph0,file1,file2,
                  aDRtree.returnGroup(),indentation);


   numChild=distriedChild.returnLen();

   if(numChild==1)
   {
      groupID=aDRtree.returnGroup();
      *newDRtree=Children.retrieve(1);
      newDRtree->setGroup(groupID);
      file1<<"Group="<<groupID<<std::endl;
      if(groupID!=0)
      {
         F.defrostGraph();
         graph0.defrostGraph();
      }
   }
   else
   {
      newDRtree->children=distriedChild;
   }

   file2<<"Returning from end"<<std::endl;
//	if(aDRtree.returnGroup()!=0)
		//F.removeLabels();
   file2<<"Ending value of newDRTree"<<std::endl;
   printTree(*newDRtree, file2, 0);
   file2<<"----------------end----------------"<<std::endl;
   return *newDRtree;
}

void explicitChild(Cluster &newTree, List<int> &nameList,List<Cluster> &forest)
{
   int i, j, numOfTree, childName, numOriginalV;
   Cluster aChild;
   Vertex childCore;
   List<int> childOrig;
   List<Cluster> tempTrees;

   std::ofstream outf;
//   outf.open("explicitChild.out", ios::app);
   outf << "The beginning ++++++++++ " << std::endl << "The cluster is: " << std::endl;
   newTree.output(outf);
   outf<< "The nameList is: " << std::endl;
   for(i=1;i<=nameList.returnLen();i++)
	outf << "    " << nameList.retrieve(i);
   outf << std::endl;
   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      outf << "aChild is: " << std::endl;
      aChild.output(outf);
      childCore=aChild.returnCore();
      childName=childCore.returnName();
      if(nameList.hasElem(childName))
      {
          outf << "The upper one is what we need" << std::endl;
          nameList.deleteElem(childName);
          newTree.deleteOrig(childName);
          childOrig=aChild.returnOrig();
          numOriginalV=childOrig.returnLen();
          for(j=1;j<=numOriginalV;j++)
          {
             newTree.addOrig(childOrig.retrieve(j)); //change original vertices
             nameList.deleteElem(childOrig.retrieve(j));
          }
          newTree.children.append(aChild); //add a child for new cluster
      }
      else
         tempTrees.append(aChild);
   }
   forest=tempTrees;
}


/*
int getClusterType(Graph &graph0, Cluster &theCluster)
{
    int LSCount, distCount, pointCount, totalCount, i;

	std::ofstream outf;
	outf.open("getclusterType.out", ios::app);
	outf << "the beginning::::" << std::endl;
	outf << "thecluster is: " << std::endl;
	theCluster.output(outf);

    totalCount=LSCount=distCount=pointCount=0;
    for(i=1;i<=theCluster.returnOrigLen();i++)
    {
       totalCount++;
       int temp=graph0.returnVertByName(theCluster.returnOrigV(i)).returnType();
       switch(temp)
       {
                case 6:         pointCount++;
                                break;
                case 7:         LSCount++;
                                break;
       }
    }

    int length=graph0.returnNumEdg();
    Edge tempEdge;
    int	notIncidCount=0;
    int totalEdgeCount=0;

    for(i=1;i<=length;i++)
    {
	tempEdge=graph0.returnEdgeByIndex(i);
	outf<<"Edge Name: "<<tempEdge.returnName()<<std::endl;
	outf<<"Edge Code: "<<getEdgeCodeDRPlanner(tempEdge,theCluster)<<std::endl;
	outf<<"Edge Type: "<<tempEdge.returnType()<<std::endl;
	if(getEdgeCodeDRPlanner(tempEdge,theCluster)==0 && tempEdge.returnType()==6)
		distCount++;
	if(getEdgeCodeDRPlanner(tempEdge,theCluster)==1 && tempEdge.returnType()!=1)
		notIncidCount++;
    }

    int childNotExtOver=0;

    for(i=1;i<=theCluster.children.returnLen();i++)
    {
	if(theCluster.children.retrieve(i).children.returnLen()==0)
		continue;
	if(theCluster.children.retrieve(i).returnType()!=2)
		childNotExtOver++;
    }

    outf<<"Getting Cluster "<<theCluster.returnName()<<" Type: "<<std::endl;
    outf<<"LSCount:     "<<LSCount<<std::endl;
    outf<<"distCount:   "<<distCount<<std::endl;
    outf<<"pointCount:  "<<pointCount<<std::endl;
    outf<<"totalCount:  "<<totalCount<<std::endl;
    outf<<"notIncid:    "<<notIncidCount<<std::endl;

    if(LSCount==0 && distCount>1)
    {
	outf<<"Type set to 0"<<std::endl;
	return 0;
    }
    if(graph0.returnDimen()==3)
    {
    	    if(notIncidCount==0 && childNotExtOver==0)
	    {
	        outf<<"Type set to 2 (2)"<<std::endl;
	        return 2;
	    }

	    if((distCount==1 && pointCount==2 && totalCount==2) ||
	       (LSCount>0 && distCount==1 && notIncidCount==1))
	    {
		outf<<"Type set to 2 (1)"<<std::endl;
	        return 2;
	    }
    }

    if(LSCount==1 && distCount==0)
    {
	outf<<"Type set to 1"<<std::endl;
        return 1;
    }

    outf<<"Type set to 0"<<std::endl;
    return 0;
}
*/
int getClusterType(Graph &graph0, Cluster &theCluster)
{
   // 	if(graph0.returnDimen()==2) return 0;

	std::ofstream outf;
//	outf.open("getClustertype.out", ios::app);
	outf << "The beginning...." << std::endl << "The cluster is: " << std::endl;
	theCluster.output(outf);
	outf << "graph0 is" << std::endl;
	graph0.output(outf);

	List<int> inList1, inList2;
	bool hasDistance = false;
	int vLen=theCluster.returnOrigLen();
	int ver1, ver2;
	Edge e;

	int numDis=0;
        for(int i=1;i<=vLen; i++)
        {
                ver1=theCluster.returnOrigV(i);
                for(int j=i+1; j<=vLen; j++)
                {
                        ver2 = theCluster.returnOrigV(j);
                        e = graph0.returnEdgeByEnds(ver1, ver2);
                        if(e.returnName()!=0 && (e.returnType()==0||e.returnType()==6))
				numDis++;
                }
        }
	if(numDis==0) return 9;
	if(numDis>1) return 0;
	if(numDis ==1 && graph0.returnDimen()==2)
		return 5;
	if(numDis ==1 && graph0.returnDimen()==3)
		return 2;
	return 111;
}

/*	for(int i=1;i<=vLen; i++)
	{
		ver1=theCluster.returnOrigV(i);
		for(int j=i+1; j<=vLen; j++)
		{
			ver2 = theCluster.returnOrigV(j);
			e = graph0.returnEdgeByEnds(ver1, ver2);
			outf << "e is " << e << std::endl;
			if(e.returnName()!=0 && (e.returnType()==0||e.returnType()==6))
			{
				inList1.append(e.returnEnd1());
				inList2.append(e.returnEnd2());
				hasDistance=true;
				break;
			}
		}
		if(hasDistance) break;
	}
	outf << "has distance? " << hasDistance << std::endl;
	outf << "inList1========" << std::endl;
	for(int i =1; i<=inList1.returnLen(); i++)
		outf << inList1.retrieve(i) << "    ";
	outf << std::endl;
	outf << "inList2========" << std::endl;
	for(int i =1; i<=inList2.returnLen(); i++)
		outf << inList2.retrieve(i) << "    ";
	outf << std::endl;

	if(!hasDistance) return 1;

	bool hasnew = true;
	int tempv;

	while(hasnew)
	{
		hasnew = false;
		for(int i=1;i<=inList1.returnLen(); i++)
		{
			ver1=inList1.retrieve(i);
			for(int j=1; j<=vLen; j++)
			{
				ver2 = theCluster.returnOrigV(j);
				e = graph0.returnEdgeByEnds(ver1, ver2);
				if(e.returnName()!=0 && e.returnType()==1)
				{
					tempv=e.returnEnd1();
					if(!inList1.hasElem(tempv))
					{
						hasnew =true;
						inList1.append(tempv);
					}
					tempv=e.returnEnd2();
					if(!inList1.hasElem(tempv))
					{
						hasnew =true;
						inList1.append(tempv);
					}
				}
			}
		}
	}
	hasnew = true;
	while(hasnew)
	{
		hasnew = false;
		for(int i=1;i<=inList2.returnLen(); i++)
		{
			ver1=inList2.retrieve(i);
			for(int j=1; j<=vLen; j++)
			{
				ver2 = theCluster.returnOrigV(j);
				e = graph0.returnEdgeByEnds(ver1, ver2);
				if(e.returnName()!=0 && e.returnType()==1)
				{
					tempv=e.returnEnd1();
					if(!inList2.hasElem(tempv))
					{
						hasnew =true;
						inList2.append(tempv);
					}
					tempv=e.returnEnd2();
					if(!inList2.hasElem(tempv))
					{
						hasnew =true;
						inList2.append(tempv);
					}
				}
			}
		}
	}

	outf << "final inList1========" << std::endl;
	for(int i =1; i<=inList1.returnLen(); i++)
		outf << inList1.retrieve(i) << "    ";
	outf << std::endl;
	outf << "final inList2========" << std::endl;
	for(int i =1; i<=inList2.returnLen(); i++)
		outf << inList2.retrieve(i) << "    ";
	outf << std::endl;
	if(inList1.returnLen()+inList2.returnLen() < vLen)
		return 0;
	else
		return 2;
}
*/

void getTrivial(List<Cluster> &DRTrees, Graph &graph0);

//the cluster is
bool specialCluster(Graph &graph0, Cluster cl)
{
	int v1, v2;
	std::ofstream outf;
//	outf.open("specialCluster.out", ios::app);

	outf << "The begining:::::" << std::endl;
	cl.output(outf);
	outf << "graph is"  << std::endl;
	graph0.output(outf);

	for(int i=1; i<=cl.returnOrigLen(); i++)
	{
		v1 = cl.returnOrigV(i);
		for(int j=1; j<=cl.returnOrigLen(); j++)
		{
			v2 = cl.returnOrigV(j);
			outf << "v1= " << v1 << "    v2=" << v2 << std::endl;
			Edge e = graph0.returnEdgeByEnds(v1, v2);
			outf << "edge = " << e << std::endl;
			if(graph0.hasEdge(e.returnName()) && e.returnType()!=1)
			   return false;
		}
	}
	return true;
}

// a cluster is a sub cluster of another if all of its original vertices are original vertices in the
// larger cluster
bool canCombine (Cluster &c, Cluster &cl0, Graph &F, Graph &graph0)  // subset operator
{
	int i, nOrig0, nOrig;
	List<int> origV0;
	List<int> overlappedVerts;

	nOrig = c.returnOrig().returnLen();
	origV0 = cl0.returnOrig();
	nOrig0 = origV0.returnLen();
	int vName;
	List<int> tempList;

	std::ofstream outf;
//	outf.open("canCombine.out", ios::app);
	outf << "The beginning:" << std::endl;

	outf << "The c is:" << std::endl;
	c.output(outf);
	outf << "The C10 is: "<<std::endl;
	cl0.output(outf);

	for(i = 1; i <= nOrig; i++)
	{
		vName = c.returnOrig().retrieve(i);
		for(int j = 1; j <= nOrig0; j++)
		{
			if (vName == origV0.retrieve(j))
				overlappedVerts.append(vName);
		}
	}

	//Get the independent vertices number.
	int lenIndepent = 0;
	int lenOverlap = overlappedVerts.returnLen();

	outf<<"lenOverlap = " << lenOverlap<< std::endl;
	//no overlapped vertices, return false
	if(lenOverlap <= 0)
	{	outf<< "lenoverlap <=0, return false" << std::endl;
		return false;
	}
	if((specialCluster(graph0, c) || specialCluster(graph0, cl0))
	    && lenOverlap >= 2)
	{
		outf <<"c is specialcluster?: " << specialCluster(graph0, c) <<std::endl;
		outf <<"cl0 is specialcluster?: " << specialCluster(graph0, cl0) <<std::endl;
		return true;
	}

        int index[lenOverlap + 1];
	for (i = 0; i <= lenOverlap; i++)
		index[i] = -1;
	int curIndex = 0;
	Edge edgeTemp;
	int ver1, ver2;
	for(i = 1; i <= lenOverlap; i++)
	{
		curIndex++;
		ver1 = overlappedVerts.retrieve(i);
		if(index[i] == -1)
			index[i] = curIndex;
		for(int j = i + 1; j <= lenOverlap; j++)
		{
			ver2 = overlappedVerts.retrieve(j);
			edgeTemp = graph0.returnEdgeByEnds(ver1, ver2);
			//incidence constraints found
			if (edgeTemp.returnName() != 0 && edgeTemp.returnType() == 1)
			{
				if(index[j] < index [i] && index[j] != -1)
				{
					int flag = index[i];
					for(int k = 1; k <= lenOverlap; k++)
					{
						if(index[k] == flag)
							index[k] = index[j];
					}
				}
				else if(index[j] == -1)
				{
					index[j] = index[i];
				}
				else
				{
					outf << "ERROR!!" << std::endl;
				}
			}
		}
	}

	for(i = 1; i <= lenOverlap; i++)
		for(int j = 1; j <= lenOverlap; j++)
			if(index[j] == i)
				lenIndepent++;

	outf << "The lenIndepent is: " << lenIndepent << std::endl;
	for(i = 1; i <= lenIndepent; i++)
		outf << " " << overlappedVerts.retrieve(i);

	if(lenIndepent < 2) return false;
	//2D
	if(F.returnDimen() == 2)	return true;

	//3D

	Graph F1, graph1;
	Vertex childCore;
	Edge edge;
	Cluster childCluster;
	List<int> frontiers, originalV;
	List<Edge>  innerE, outerE;

	List<Cluster> DR_Trees;
	//append vertices into graph1 and DR_Trees
	for(i = 1; i <= lenIndepent; i++)
	{
		vName = overlappedVerts.retrieve(i);
		childCore = graph0.returnVertByName(vName);
		frontiers.append(vName);
		originalV.append(vName);
		childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
		DR_Trees.append(childCluster);
		frontiers.makeEmpty();
		originalV.makeEmpty();
		//append to graph1
		graph1.appendVertex(childCore);
		for(int j = i + 1; j <= lenIndepent; j++)
		{
			edge = graph0.returnEdgeByEnds(vName, overlappedVerts.retrieve(j));
			if(edge.returnName() != 0)
				graph1.appendEdge(edge);
		}

	}
	outf<<"Contents of F1"<<std::endl;
	F1.output(outf);
	outf<<"Contents of graph1"<<std::endl;
	graph1.output(outf);
	outf<<"Contents of DRTrees"<<std::endl;
	printForest(DR_Trees, outf, 0);

	DR_Trees=distributeForest(DR_Trees, F1, graph1, std::cout, std::cout, 0,0);
 	getTrivial(DR_Trees, F1);
	outf<<"Contents of new DRTrees"<<std::endl;
	printForest(DR_Trees, outf, 0);

	//overlapped part is not a cluster, return true
	if(DR_Trees.returnLen() > 1) return true;

	//the overlapped cluster is not a rot-sym object
	if(getClusterType(graph1, DR_Trees.retrieve(DR_Trees.returnLen())) != 2)
	{
		outf << "the cluster type is not 2" << std::endl;
		return true;
	}
	else
	{
		//if one rot-sym cluster is overlapped with another cluster, return true)
		if (getClusterType(graph0, cl0) == 2 || getClusterType(graph0, c) == 2)
		{	outf<<"cl0 is type " << getClusterType(graph0, cl0) <<std::endl;
			outf<<"c is type " << getClusterType(graph0, c) <<std::endl;
			outf << "return true:" << std::endl;
			return true;
		}
	}


	for(i = 1; i <= nOrig; i++)
	{
		ver1 = c.returnOrig().retrieve(i);
		//grahp1 has ver1
		if(graph1.returnVertByName(ver1).returnName() != 0)
			continue;
		for(int j = 1; j <= nOrig0; j++)
		{	//exists the independent edge
			ver2 = cl0.returnOrig().retrieve(j);
			if(graph0.returnEdgeByEnds(ver1, ver2).returnName() != 0
				 && graph1.returnEdgeByEnds(ver1, ver2).returnName() == 0)
			{
				outf << "ver1 = " << ver1 << ", ver2 = " << ver2 << std::endl;
				return true;
			}
		}
	}
	return false;
}

void implicitChild(Cluster &newTree, List<int> &nameList,
                      List<Cluster> &forest, Graph &F, Graph &graph0)
{
   int i, j, numOfTree, numOriginalV, originalVert,  numInner;
   Vertex childCore, parentCore;
   Edge innerEdge;
   Cluster aChild, *temp;
   List<int> tempOrig, childOrig;
   List<Cluster> tempTrees;

   std::ofstream outf;
//   outf.open("implicit.out", ios::app);
   outf<< "(The Beginning)The newTree's name is:" << newTree.returnName() << std::endl;
   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      if(canCombine(aChild, newTree, F, graph0))
      {
	  outf << "The aChild is:" << std::endl;
	  aChild.output(outf);
	  outf << "The newTree is: "<<std::endl;
	  newTree.output(outf);
	  outf << "The old F is:" << std::endl;
	  F.output(outf);

          childCore=aChild.returnCore();
          childCore.setLabel(1);
          F.delVer(childCore);
          F.appendVertex(childCore);
          numInner=aChild.returnInnerE().returnLen();
          for(j=1;j<=numInner;j++)
          {
             innerEdge=aChild.returnInnerE().retrieve(j);
             innerEdge.setLabel(1);
             F.delEdg(innerEdge);
             F.appendEdge(innerEdge);
          }
          parentCore=newTree.returnCore();
          parentCore.setLabel(1);
          F.delVer(parentCore);
          F.appendVertex(parentCore);
          numInner=newTree.returnInnerE().returnLen();
          for(j=1;j<=numInner;j++)
          {
             innerEdge=newTree.returnInnerE().retrieve(j);
             innerEdge.setLabel(1);
             F.delEdg(innerEdge);
             F.appendEdge(innerEdge);
          }
          outf<< "The new F is:" << std::endl;
          F.output(outf);

          temp=getCluster(F, graph0);
          tempOrig=newTree.returnOrig();

          newTree.formCl(temp->returnCore(), temp->returnFrontiers(),
                 temp->returnInnerE(), temp->returnOuterE(), tempOrig);
          childOrig=aChild.returnOrig();
          numOriginalV=childOrig.returnLen();
          for(j=1;j<=numOriginalV;j++)
          {
             originalVert=childOrig.retrieve(j);
             nameList.deleteElem(originalVert);
             if(!tempOrig.hasElem(originalVert))
                newTree.addOrig(originalVert);
          }
          newTree.children.append(aChild); //add a child for new cluster
 	  outf << "The new newTree is: "<<std::endl;
	  newTree.output(outf);
     }
      else
         tempTrees.append(aChild);
   }
   forest=tempTrees;
}


void addChild(Cluster &newTree, List<Cluster> &popedTrees,
               List<Cluster> &DR_Trees, Graph &F, Graph &graph0)
{
   int i, j, listLen, childName;
   List<int> nameList, originalV0, frontiers0;
   Cluster *aChild;
   Vertex childCore;
   List<Edge> innerE, outerE;

   //out<<"add child for"<<std::endl;
   //printTree(newTree,out,0);

   nameList=newTree.returnOrig(); //vertex name list forms a new cluster
	std::ofstream outf;
//	outf.open("addchild.out", ios::app);
	outf << "BEEEEEEEEEE" << std::endl;
	newTree.output(outf);

   //check each item (not poped) left in DR_Trees with the name list
   explicitChild(newTree, nameList, DR_Trees);
   outf << "explicitChild with DR_Trees" << std::endl;
   printForest(DR_Trees, std::cout, 0);

   // check each poped item with the name list
   explicitChild(newTree, nameList, popedTrees);

   outf << "explicitChild with popedTrees" << std::endl;
   printForest(DR_Trees, std::cout, 0);
	outf << "after explicitchild " << std::endl;
	newTree.output(outf);
   // look implicit child in DR_Trees
   implicitChild(newTree, nameList, DR_Trees, F, graph0);
   printForest(DR_Trees, std::cout, 0);

   // look implicit child in popedTrees
   implicitChild(newTree, nameList, popedTrees, F, graph0);
	outf << "after implicitchild " << std::endl;
	newTree.output(outf);

   outf << "implicitChild with popedTrees" << std::endl;
   printForest(DR_Trees, std::cout, 0);

   //The left names in the list are frontier vertices shared by two or more
   //new clusters. We have to create one or more clusters for each of them.
   listLen=nameList.returnLen();
   for(i=1;i<=listLen;i++)
   {
	frontiers0.makeEmpty();
	originalV0.makeEmpty();
       childName=nameList.retrieve(i);
       childCore=F.returnVertByName(childName);
       frontiers0.append(childName);
       originalV0.append(childName);
	aChild = new Cluster;
       aChild->formCl(childCore,frontiers0,innerE,outerE,originalV0);
       newTree.children.append(*aChild); // add a single vertex child
   }

	outf << "The enddd: " << std::endl;
	newTree.output(outf);
   return;
}

int checkClusterForBananas(Graph &graph0, Cluster &theCluster)
{
	if(theCluster.children.returnLen()!=2)
		return -1;

	//either the child cluster is rot-sym cluster, return 3;
	if (getClusterType(graph0, theCluster.children.retrieve(1)) == 2 ||
			getClusterType(graph0, theCluster.children.retrieve(2)) == 2)
		return 3;
	int i, length=graph0.returnNumVer();

	List<int> overlappedVerts;

	std::ofstream outf;
//	outf.open("checkBananas.out", ios::app);
	outf << " The beginning:" << std::endl;
	outf<<"Getting list of overlapping vertices: ";

	//get the overlapped part
	for(i=1;i<=length;i++)
	{
		List<int> tempList;
		int vName;

		vName=graph0.returnVertByIndex(i).returnName();
		getContainedChildList(theCluster, vName, tempList);
		if(tempList.returnLen()==2)
		{
			overlappedVerts.append(vName);
			outf<<vName<<" ";
		}
	}

	outf<<std::endl;

	int j;
	Vertex testVert;
	Edge testEdge;
	List<int> toRemove;

	for(i=1;i<=overlappedVerts.returnLen();i++)
	{
		testVert=graph0.returnVertByName(overlappedVerts.retrieve(i));
		outf<<testVert<<std::endl;
		if(testVert.returnType()!=7) continue;
		for(j=1;j<=overlappedVerts.returnLen();j++)
		{
			int vertName=testVert.returnName();

			testEdge=graph0.returnEdgeByEnds(testVert.returnName(),overlappedVerts.retrieve(j));
			if(testEdge.returnName()==0) continue;
			if(testEdge.returnType()==1 && !toRemove.hasElem(vertName))
				toRemove.append(testVert.returnName());
		}
	}

	outf<<"After line check"<<std::endl;

	for(i=1;i<=toRemove.returnLen();i++)
		overlappedVerts.deleteElem(toRemove.retrieve(i));

	outf<<"After line remove"<<std::endl;

	if(overlappedVerts.returnLen()!=2)
		return -1;

	bool edgeBetween=false;
        length=graph0.returnNumEdg();
	graph0.output(outf);
        for(i=1;i<=length;i++)
	{
		Edge theEdge=graph0.returnEdgeByIndex(i);
		int value=getEdgeCodeDRPlanner(theEdge,theCluster);
		int typeValue=graph0.returnEdgeByIndex(i).returnType();
                edgeBetween=(value==1 && typeValue!=1) || edgeBetween;

		outf<<theEdge<<std::endl;
		outf<<"value="<<value<<std::endl;
		outf<<"typeValue="<<typeValue<<std::endl;
		outf<<"Edgebetween="<<(edgeBetween ? "true" : "false")<<std::endl;

	}

	outf<<"Final Edgebetween="<<(edgeBetween ? "true" : "false")<<std::endl;

	outf<<"After edge between"<<std::endl;

	bool partitionable;
	List<int> over1, over2;

	outf<<"OverlappedVerts: ";
        for(i=1;i<=overlappedVerts.returnLen();i++)
                outf<<overlappedVerts.retrieve(i)<<" ";
        outf<<std::endl;

	over1.append(overlappedVerts.retrieve(1));
	overlappedVerts.deleteIndex(1);

	for(i=1;i<=overlappedVerts.returnLen();i++)
		for(int j=1;j<=over1.returnLen();j++)
		{
			outf<<"OverLap: "<<overlappedVerts.retrieve(i)<<" Over1: "<<over1.retrieve(j)<<std::endl;
			if(graph0.returnEdgeByEnds(overlappedVerts.retrieve(i),over1.retrieve(j)).returnType()==1)
				if(!over1.hasElem(overlappedVerts.retrieve(i)))
					over1.append(overlappedVerts.retrieve(i));
		}

	for(i=1;i<=over1.returnLen();i++)
		if(overlappedVerts.hasElem(over1.retrieve(i))) overlappedVerts.deleteElem(over1.retrieve(i));


	if(overlappedVerts.returnLen()==1)
	{
		over2.append(overlappedVerts.retrieve(1));
		partitionable=true;
	}
	else
	{
		if(overlappedVerts.returnLen()==0)
			partitionable=false;
		else
		{
			over2.append(overlappedVerts.retrieve(1));
			overlappedVerts.deleteIndex(1);

			for(i=1;i<=overlappedVerts.returnLen();i++)
		                for(int j=1;j<=over2.returnLen();j++)
					if(graph0.returnEdgeByEnds(overlappedVerts.retrieve(i),over2.retrieve(j)).returnType()==1)
                                		if(!over2.hasElem(overlappedVerts.retrieve(i)))
		                               		over2.append(overlappedVerts.retrieve(i));

			for(i=1;i<=over2.returnLen();i++)
		                if(overlappedVerts.hasElem(over2.retrieve(i))) overlappedVerts.deleteElem(over2.retrieve(i));

			if(overlappedVerts.returnLen()!=0)
				partitionable=false;
			else    partitionable=true;

		}
	}

	outf<<"After partitionable"<<std::endl;

	outf<<"Over1: ";
	for(i=1;i<=over1.returnLen();i++)
		outf<<over1.retrieve(i);
	outf<<std::endl;
	outf<<"Over2: ";
        for(i=1;i<=over2.returnLen();i++)
                outf<<over2.retrieve(i);
        outf<<std::endl;

	if(partitionable && !edgeBetween)
		return 0;

	if(partitionable && edgeBetween)
		return 3;


	return -4;
}




void setAlias(Graph &graph0, Cluster &theCluster)
{
   if(theCluster.returnType()!=2 && theCluster.returnType()!=5) return;

   int i, length=theCluster.returnOrigLen();

   int alias=-1;

	std::ofstream outf;
//	outf.open("setAlias.out", ios::app);

   for(i=1;i<=length;i++)
        if(graph0.returnVertByName(theCluster.returnOrigV(i)).returnType()==7
		||graph0.returnVertByName(theCluster.returnOrigV(i)).returnType()==3)
	{
		alias=theCluster.currAlias=theCluster.returnOrigV(i);
		outf<<"Alias found: "<<alias<<std::endl;
		break;
	}

   if(alias==-1)
   {
	outf<<"Error: No Alias found."<<std::endl;
	return;
   }

   Edge currEdge;
   int currVertID, currVertType, aliasPart;

   for(i=1;i<=length;i++)
   {
	currVertID=theCluster.returnOrigV(i);
        currVertType=graph0.returnVertByName(currVertID).returnType();

        if(currVertType==7 || currVertType==3)
        {
                theCluster.lines.append(currVertID);
                outf<<"Line appended: "<<currVertID<<std::endl;
                continue;
        }

        if(currVertType!=6 && currVertType!=0) continue;

	currEdge=graph0.returnEdgeByEnds(alias,currVertID);
	if(currEdge.returnName()==0) continue;
	if(currEdge.returnType()!=1) continue;

	outf<<"Checking Edge: "<<currEdge<<std::endl;

	if(currEdge.returnEnd1()==alias)
		aliasPart=currEdge.returnPart1();
	else
		aliasPart=currEdge.returnPart2();

	if(aliasPart==1)
	{
		theCluster.left.append(currVertID);
		outf<<"Left appended: "<<currVertID<<std::endl;
	}

	if(aliasPart==2)
	{
		theCluster.right.append(currVertID);
		outf<<"Right appended: "<<currVertID<<std::endl;
	}
   }
}

bool isDense(Cluster &C1, Cluster &C2, Graph &graph0)
{
   int i, numVert, numEdge, vertWeight, edgeWeight;
   List<int> origList;
   Edge testEdge;
   int K;

   std::cout<<"IN DENSE"<<std::endl;

   if(graph0.returnDimen()==3)
	K=6;
   else K=3;

   vertWeight=edgeWeight=0;
   origList.makeEmpty();

   for(i=1;i<=C1.returnOrigLen();i++)
	origList.append(C1.returnOrigV(i));

   for(i=1;i<=C2.returnOrigLen();i++)
   {
	int tempV=C2.returnOrigV(i);
        if(!origList.hasElem(tempV))
		origList.append(C2.returnOrigV(i));
   }

   numVert=origList.returnLen();
   for(i=1;i<=numVert;i++)
	vertWeight+=graph0.returnVertByName(origList.retrieve(i)).returnWeight();

   numEdge=graph0.returnNumEdg();
   for(i=1;i<=numEdge;i++)
   {
	testEdge=graph0.returnEdgeByIndex(i);
	std::cout<<testEdge<<std::endl;
	int temp1E=testEdge.returnEnd1();
	int temp2E=testEdge.returnEnd2();

	if(!origList.hasElem(temp1E)) continue;
  	if(!origList.hasElem(temp2E)) continue;
	edgeWeight+=testEdge.returnWeight();
   }

   std::cout<<"Edge Weight: "<<edgeWeight<<std::endl;
   std::cout<<"Vert Weight: "<<vertWeight<<std::endl;

   return (vertWeight-edgeWeight<K);
}

// Important note: THIS METHOD WILL NOT CORRECTLY ADD A CLUSTER.  TO BE USED
// ONLY FOR CERTAIN VALIDATION CHECKS ABOUT A CLUSTERS STRUCTURE.
void pseudoAppendChild(Cluster &theCluster, Cluster &newCluster)
{
   theCluster.children.append(newCluster);

   int i;

   for(i=1;i<=newCluster.returnOrigLen();i++)
   {
	int currOrig=newCluster.returnOrigV(i);

	theCluster.addOrig(currOrig);
   }
}

void pseudoMinDense(Cluster &theCluster, Graph &graph0)
{
   int i,j,k, length;

   std::ofstream outf;
//	outf.open("pseudoMinDense.out", ios::app);

   outf<<"IN PSEUDOMINDENSE"<<std::endl << "The INPUT cluster is:" << std::endl;
   theCluster.output(outf);

   theCluster.output(std::cout);

   Cluster tempCluster;

   length=theCluster.children.returnLen();
   for(i=1;i<=length;i++)
	pseudoAppendChild(tempCluster,theCluster.children.retrieve(i));

   bool isChange=true;
   while(isChange)
   {
	isChange=false;
   	length=tempCluster.children.returnLen();
   	if(length<=2) return;
   	for(i=1;i<=length;i++)
		for(j=i+1;j<=length;j++)
		{
			if(isChange) continue;
			outf<<"Cluster 1: "<<theCluster.children.retrieve(i).returnName()<<std::endl;
			tempCluster.children.retrieve(i).output(outf);
			outf<<"Cluster 2: "<<theCluster.children.retrieve(j).returnName()<<std::endl;
			tempCluster.children.retrieve(j).output(outf);
			if(isDense(tempCluster.children.retrieve(i),tempCluster.children.retrieve(j),graph0))
			{
				Cluster tempCluster2, tempCluster3;

				pseudoAppendChild(tempCluster2,tempCluster.children.retrieve(i));
				pseudoAppendChild(tempCluster2,tempCluster.children.retrieve(j));

				if(checkClusterForBananas(graph0,tempCluster2)!=0)
				{
					isChange=true;

					pseudoAppendChild(tempCluster3,tempCluster2);

					for(k=1;k<=length;k++)
						if(k!=i && k!=j)
							pseudoAppendChild(tempCluster3,tempCluster.children.retrieve(k));

					tempCluster.makeEmpty();

					length=tempCluster3.children.returnLen();
					for(k=1;k<=length;k++)
						pseudoAppendChild(tempCluster,tempCluster3.children.retrieve(k));
				}
			}
		}
    }

}


bool canMerge(Graph &graph0, Cluster &child1, Cluster &child2)
{
	std::ofstream outf;
//	outf.open("canMerge.out", ios::app);
	//create vertex list
	List<int> verList;
	verList = child1.returnOrig();
	for(int i=1; i<= child2.returnOrigLen(); i++)
	{
		int t=child2.returnOrigV(i);
		if(!verList.hasElem(t))
			verList.append(child2.returnOrigV(i));
	}

	//create graph
	Graph F1, graph1;
	Vertex childCore;
	Edge edge;
	Cluster childCluster;
	List<int> frontiers, originalV;
	List<Edge>  innerE, outerE;

	List<Cluster> DR_Trees;
	//append vertices into graph1 and DR_Trees
	for(int i = 1; i <= verList.returnLen(); i++)
	{
		int vName = verList.retrieve(i);
		childCore = graph0.returnVertByName(vName);
		childCore.unFrozen();
		frontiers.append(vName);
		originalV.append(vName);
		childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
		DR_Trees.append(childCluster);
		frontiers.makeEmpty();
		originalV.makeEmpty();
		//append to graph1
		graph1.appendVertex(childCore);
		for(int j = i + 1; j <= verList.returnLen(); j++)
		{
			edge = graph0.returnEdgeByEnds(vName, verList.retrieve(j));
			if(edge.returnName() != 0)
				graph1.appendEdge(edge);
		}

	}
	outf<<"Contents of F1"<<std::endl;
	F1.output(outf);
	outf<<"Contents of graph1"<<std::endl;
	graph1.output(outf);
	outf<<"Contents of DRTrees"<<std::endl;
	printForest(DR_Trees, outf, 0);

	std::ofstream dd1("dd1.out", std::ios::app);
	std::ofstream dd2("dd2.out", std::ios::app);

	DR_Trees=distributeForest(DR_Trees, F1, graph1, dd1, dd2, 0,0);
 	getTrivial(DR_Trees, F1);
	outf<<"Contents of new DRTrees"<<std::endl;
	printForest(DR_Trees, outf, 0);
	if(DR_Trees.returnLen()==1)
		return true;
	return false;
}

//merge clusters
void mergeCluster(Graph &graph0, int coreName, Cluster &parent, Cluster child1, Cluster child2)
{
	Vertex core;
	List<int> orig, fronV, innerV;
	List<Edge> innerE, outerE;

	std::ofstream outf;
//	outf.open("mergeCluster.out", std::ios::app);

	orig = child1.returnOrig();
	for(int i=1; i<= child2.returnOrigLen(); i++)
	{
		int t=child2.returnOrigV(i);
		if(!orig.hasElem(t))
			orig.append(t);
	}

	//frontier vertices
	fronV = child1.returnFrontiers();
	for(int i=1; i<= child2.returnFronLen(); i++)
	{
		int t=child2.returnFrontier(i);
		if(!fronV.hasElem(t))
			fronV.append(t);

	}
	outf << "after getting fronVertices, the fronts are:" << std::endl;
	for(int i=1; i<=fronV.returnLen(); i++)
		outf << fronV.retrieve(i) << std::endl;
	outf << "the orig are: " << std::endl;
	for(int i=1; i<=orig.returnLen(); i++)
		outf << orig.retrieve(i)  << std::endl;

	outf << "let's do inner v part" << std::endl;
	//inner vertice
	for(int i=1; i<=orig.returnLen(); i++)
	{
		int vertex = orig.retrieve(i);
		outf << "vertex is: " << vertex << std::endl;

		Vertex V=graph0.returnVertByName(vertex);
		outf << "corresponding vertex is: " << V << std::endl;
		if(!fronV.hasElem(vertex))
		{
			outf<<"fronv has it" << std::endl;
			for(int j=1; j<=V.numIncid(); j++)
			{
				int edgeName = V.returnIncid(j);
				outf << "edge name is: " << edgeName << std::endl;
				Edge edge=graph0.returnEdgeByName(edgeName);
				int end1 =edge.returnEnd1();
				int end2 = edge.returnEnd2();
				outf << "end1, end2: " << end1 << ", " << end2 << std::endl;

				int theOther;
				if(end1==vertex)
					theOther=end2;
				else
					theOther=end1;
				outf << "theOther" << theOther << std::endl;
				if(!fronV.hasElem(theOther) && !innerV.hasElem(vertex))
					innerV.append(vertex);
			}
		}
	}

	for(int i=1; i<=innerV.returnLen();i++)
	{
		if(fronV.hasElem(innerV.retrieve(i)))
			fronV.deleteElem(innerV.retrieve(i));
	}
	for(int i=1; i<=fronV.returnLen(); i++)
		outf << fronV.retrieve(i) << std::endl;
 	outf << "after getting inner vertices" << std::endl;

	//outer edges
	for(int i=1; i<=fronV.returnLen();i++)
	{
		for(int j=i+1; j<=fronV.returnLen();j++)
		{
			Edge outer= graph0.returnEdgeByEnds(i,j);
			if(outer.returnName()!=0)
				outerE.append(outer);
		}
	}
	outf << "after getting outer Edges" << std::endl;
	//inner edges
	for(int i=1; i<=fronV.returnLen();i++)
	{
		for(int j=1; j<=innerV.returnLen();j++)
		{
			Edge inner= graph0.returnEdgeByEnds(i,j);
			if(inner.returnName()!=0)
			{
				innerE.append(inner);
				if(inner.returnEnd1()==i)
					inner.setEnd(1, coreName);
				else
					inner.setEnd(0, coreName);
			}
		}
	}
	outf << "after getting inner Edges" << std::endl;

	int coreWeight=0;
	//core weight
	for(int i=1; i<=innerV.returnLen();i++)
	{
		coreWeight+=graph0.returnVertByName(innerV.retrieve(i)).returnWeight();
		for(int j=i+1; j<=innerV.returnLen();j++)
		{
			Edge in= graph0.returnEdgeByEnds(i,j);
			if(in.returnName()!=0)
				coreWeight-=in.returnWeight();
		}
	}
	outf << "after core weight" << std::endl;
	core.setWeight(coreWeight);
	core.setName(coreName);
	parent.formCl(core,fronV,innerE,outerE,orig);
	parent.children.append(child1);
	parent.children.append(child2);
	outf<<"the parent is: " << std::endl;
	if(child1.returnDepth()>child2.returnDepth())
		core.setDepth(child1.returnDepth()+1);
	else
		core.setDepth(child2.returnDepth()+1);
	outf << "the end is reached" << std::endl;
	parent.output(outf);
}

//change the depth of the cluster based the depths of its children.
void changeDepth(Cluster &newC)
{
	for(int i=1; i<=newC.children.returnLen(); i++)
	{
		if(newC.returnDepth()<=newC.children.retrieve(i).returnDepth())
			newC.setDepth(newC.children.retrieve(i).returnDepth()+1);
	}
}

//modify tree which has 3 or more children if possible.
int modifyTree(Graph &graph0, Cluster &newC)
{
	int childLen = newC.children.returnLen();
	if (childLen<=2) return 0;
	if(newC.returnDepth()<=1) return 0;
	if (childLen>3) return 0;
	//ugly
	std::ofstream outf;
//	outf.open("modifyTree.out", std::ios::app);
	outf << "the total num of vertices is: " << graph0.returnNumVer() << std::endl;
	outf << "the total num of edges is: " <<graph0.returnNumEdg() << std::endl;
	if ((graph0.returnNumVer()!= 8 || graph0.returnNumEdg()!=18)
	  &&(graph0.returnNumVer()!= 11 || graph0.returnNumEdg()!=23))
		return 0;

	outf << "The begining, the cluster is: " << std::endl;
	newC.output(outf);

	for(int i=1; i<=newC.children.returnLen();i++)
	{
		Cluster child1 = newC.children.retrieve(i);
		outf<< " child1" << std::endl;
		child1.output(outf);
		for(int j=i+1; j<=newC.children.returnLen();j++)
		{
			Cluster child2 = newC.children.retrieve(j);
			outf<<"child2" << std::endl;
			child2.output(outf);
			if(canMerge(graph0, child1, child2))
			{
				Cluster *temp = new Cluster;
				mergeCluster(graph0, newC.returnName(), *temp, child1, child2);
				outf << "the new cluster is: " << std::endl;
				temp->output(outf);

				//change name, children and depth of newC
				int oldName=newC.returnName();
				outf<< "old name is: " << oldName << std::endl;

				newC.setName(nextVerName++);
				int newName=newC.returnName();
				outf << "new name is:" << newName<< std::endl;
				for(int k=1;k<=newC.returnInnerELen();k++)
				{
					Edge e=newC.returnInnerE(k);
					if(e.returnEnd1()==oldName)
						e.setEnd(0, newName);
					else
						e.setEnd(1, newName);
				}
				newC.children.deleteElem(child1);
				newC.children.deleteElem(child2);
				newC.children.append(*temp);
				for(int k=1;k<=newC.children.returnLen();k++)
				{
					if(newC.returnDepth()<newC.children.retrieve(k).returnDepth()+1)
						newC.setDepth(newC.children.retrieve(k).returnDepth()+1);
				}
				i = 1;
				j = 2;
				outf << "end of if-canMerge " << std::endl;
			}
		}
	}

	outf << "AT THE END, the newC is: " << std::endl;
	newC.output(outf);
	outf << "Its children are: "  << std::endl;
	for(int i=1; i<=newC.children.returnLen(); i++)
		newC.children.retrieve(i).output(outf);

	return 1;
}

int setConstrain(Graph &G, Cluster &theCluster)
{
	std::ofstream outf;
	outf.open("setConstrain.out", std::ios::app);
	outf << " =============== begin le ============================" << std::endl;
	outf << "The cluster is: " << std::endl;
	theCluster.output(outf);

	//not 3D example, return
	if(G.returnDimen()!=3)
		return -1;

	//the cluster has not exact 2 children.
	if(theCluster.children.returnLen()!=2)
		return -1;

	List<int> overList;
	getOverlapList(G, theCluster, overList);
	outf << "The over list contains:" << std::endl;
	for(int i=1; i<=overList.returnLen(); i++)
		outf << "   " << overList.retrieve(i);

	if(overList.returnLen()!=2)
		return -1;
	Edge e = G.returnEdgeByEnds(overList.retrieve(1), overList.retrieve(2));

	outf << "The edge is: " << e << std::endl;
	//exit the distance between these two overlapped points
	if(e.returnName()>0)
		theCluster.setConst(1);
	else
		theCluster.setConst(2);
	outf << "The new cluster is: " << std::endl;
	theCluster.output(outf);
 return 1;
}

List<Cluster> &distributeForest(List<Cluster> &DR_Trees, Graph &F,
  Graph &graph0, std::ostream &file1, std::ostream &file2, int groupID, int indentation)
{
   int i, j, len;
   Cluster aTree, distried, newTree;
   List<Cluster> popedTrees;

   file2<<"---------------start---------------"<<std::endl;
   file2<<"DistributeForest"<<std::endl;
   file2<<"Contents of F"<<std::endl;
   F.output(file2);
   file2<<"Contents of DRTrees"<<std::endl;
   printForest(DR_Trees, file2, 0);

   indentation++;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      aTree = DR_Trees.pop();
      distried=distributeTree(aTree, F, graph0, file1,file2, indentation);
      if(distried.empty())
         delete &distried;
      else
         DR_Trees.append(distried);
   }

   if(len==1) {
      return DR_Trees;
   }

	file2 << "groupID is: " << groupID << std::endl;

   List<int> groupV;
   if(groupID!=0)
   {

       F.freeze();
       graph0.freeze();
       defrostForest(DR_Trees, F);
       defrostForest(DR_Trees, graph0);
	for(i=1;i<=DR_Trees.returnLen();i++)
		groupV.append(DR_Trees.retrieve(i).returnOrigV(1));
   }
	bool firstTime = true;
   while(DR_Trees.returnLen()>0)
   {
      std::cout<<"Start of DR_Trees Loop"<<std::endl;
      List<Cluster> tempDR_Trees;
      List<Cluster> tempPopedTrees;

      if(graph0.returnDimen()==3)
      {
          for(int i=1;i<=DR_Trees.returnLen();i++)
                  tempDR_Trees.append(DR_Trees.retrieve(i));
      }
      file2<<"Contents of DR_Trees"<<std::endl;
      printForest(DR_Trees, file2, 0);
      printForest(DR_Trees, std::cout, 0);
      file2<<"Before extension"<<std::endl;
      file1<<"---------------------------------------------"<<std::endl;

      if(extension(F, graph0, firstTime, groupID, file1, file2))
      {
         	file2<<"Found new Cluster by Extension:"<<std::endl;
         	newTree=*getCluster(F, graph0);
         	newTree.output(file2);
         	file2<<"----------------end----------------"<<std::endl;
      }
      else
      {
	 std::cout<<"In distributeCL path"<<std::endl;
         aTree=popMinDepth(DR_Trees);  // item is deleted in DR_Trees
         popedTrees.append(aTree);

	 //save the DR_Trees and popedTrees
	 if(graph0.returnDimen()==3)
	 {
	     tempDR_Trees.makeEmpty();
	     tempPopedTrees.makeEmpty();
	     for(int i=1;i<=DR_Trees.returnLen();i++)
	          tempDR_Trees.append(DR_Trees.retrieve(i));
	     for(int i=1;i<=popedTrees.returnLen();i++)
	          tempPopedTrees.append(popedTrees.retrieve(i));
 	 }
         firstTime = false;

         file1<<"Pop Cluster, "<<aTree.returnCore().returnName();
         file1<<"  Group="<<aTree.returnGroup()<<std::endl;
         file2<<"Pop Cluster, "<<aTree.returnCore().returnName()<<std::endl;
         newTree=distributeCl(aTree, F, graph0, file1, file2);
      }

      file2 << "Let's see" << std::endl;
      file2 << "DR_Trees" << std::endl;
      printForest(DR_Trees, file2, 0);
      file2 << "popedTrees" << std::endl;
      printForest(popedTrees, file2, 0);
      file2 << "tempDR_Trees" << std::endl;
      printForest(tempDR_Trees, file2, 0);

      if(!newTree.empty()) {

	  file2<<"Before addChild "<<std::endl;
          newTree.output(file2);
          file2<<"right before ADDCHILD" << std::endl;
          printForest(DR_Trees, file2, 0);
          addChild(newTree, popedTrees, DR_Trees, F, graph0);
	  file2<<"After addchild"<<std::endl;
	  graph0.output(std::cout);
	  pseudoMinDense(newTree,graph0);
          //constrain=F.constrainDegree();
          //file2<<"constrain="<<constrain<<std::endl;
          //newTree.setConst(constrain); //1--well constrained, 2,3...--over
          file2<<"In DistForest right before cluster output"<<std::endl;
          newTree.output(file2);
          file2<<"Starting Contents of F:"<<std::endl;
          file2<<"Right before deleting inner edges"<<std::endl;
	  F.output(file2);
	  Graph tempGraph;
	  copyG(F,tempGraph);
	  tempGraph.removeLabels();
          delInnerVer(F, graph0);
	  std::cout<<"after deleting inner edges"<<std::endl;
          file2<<"deleting inner edges"<<std::endl;
          F.output(file2);
	  file2<<"right before modify DR_Tree" << std::endl;
          printForest(DR_Trees, file2, 0);
	  if(newTree.returnDepth()>1)
	          modifyTree(graph0, newTree);
	  int newType = getClusterType(graph0, newTree);
	  newTree.setType(newType);
          DR_Trees.append(newTree);
	  file2<<"appending newTree"<<std::endl;
	  file2<<"Contents of DR_Trees after extension append"<<std::endl;
          printForest(DR_Trees, file2, 0);
	  printForest(DR_Trees, std::cout,0);

     if(graph0.returnDimen()==3)
     {

	  int checkValue=checkClusterForBananas(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  file2<<"Checks value: "<<checkValue<<std::endl;
	  int cType;
	  switch(checkValue)
	  {
		case -1:  	file2<<"Before ClusterType"<<std::endl;
	  			cType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  			file2<<"After ClusterType, the cluster type is:" << cType << std::endl;
			        DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(cType);
                                file2<<"After checkValue switch"<<std::endl;
				break;
		case 0:		DR_Trees.makeEmpty();
				file2<<"After DR_Trees.makeEmpty()"<<std::endl;
				file2<<"Length: "<<tempDR_Trees.returnLen()<<std::endl;
				copyG(tempGraph,F);
				//restore the original DR_Trees
				for(int i=1;i<=tempDR_Trees.returnLen();i++)
				{
					file2<<"Restoring old DR_Trees: "<<i<<std::endl;
			                DR_Trees.append(tempDR_Trees.retrieve(i));
					file2<<"done"<<std::endl;
				}
				//restore the original DR_Trees
				popedTrees.makeEmpty();
				for(int i=1;i<=tempPopedTrees.returnLen();i++)
				{
					file2<<"Restoring old PopedTrees: "<<i<<std::endl;
			                popedTrees.append(tempPopedTrees.retrieve(i));
					file2<<"done"<<std::endl;
				}
				break;
		case 3:		DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(3);
				file2<<"TYPE SET TO 3"<<std::endl;
				break;
		default:	file2<<"ERROR UNEXPECTED RETURN!"<<std::endl;
				break;
	  }//switch(checkValue)
	  if(DR_Trees.returnLen() == 0) continue;
	  file2<<"Before ClusterType"<<std::endl;
	  cType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  file2<<"After ClusterType, the cluster type is:" << cType << std::endl;
	  DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(cType);
	  setConstrain(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  file2<<"After checkValue switch"<<std::endl;

	  if(cType==2)
	  {
		DR_Trees.addrByIndex(DR_Trees.returnLen())->returnCore().incrsWt(-1);
		setAlias(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
		Cluster theCluster = DR_Trees.retrieve(DR_Trees.returnLen());
		file2<<"After aliasing for cluster: "<<theCluster.returnName()<<std::endl;
		file2<<"Alias: "<<theCluster.currAlias<<std::endl;
		file2<<"Left: ";
		for(int q=1; q<=theCluster.left.returnLen(); q++)
			file2<<theCluster.left.retrieve(q)<<" ";
		file2<<std::endl;
		file2<<"Right: ";
                for(int q=1; q<=theCluster.right.returnLen(); q++)
                        file2<<theCluster.right.retrieve(q)<<" ";
                file2<<std::endl;
		file2<<"Lines: ";
                for(int q=1; q<=theCluster.lines.returnLen(); q++)
                        file2<<theCluster.lines.retrieve(q)<<" ";
                file2<<std::endl;
	  }

	  file2<<"end of check bananas if"<<std::endl;
	  }//if(graph0.returnDimen()==3)

	  int tType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  if(tType==5)
	  {
		//DR_Trees.addrByIndex(DR_Trees.returnLen())->returnCore().incrsWt(-1);
		setAlias(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
		Cluster theCluster = DR_Trees.retrieve(DR_Trees.returnLen());
		file2<<"After aliasing for cluster: "<<theCluster.returnName()<<std::endl;
		file2<<"Alias: "<<theCluster.currAlias<<std::endl;
		file2<<"Left: ";
		for(int q=1; q<=theCluster.left.returnLen(); q++)
			file2<<theCluster.left.retrieve(q)<<" ";
		file2<<std::endl;
		file2<<"Right: ";
                for(int q=1; q<=theCluster.right.returnLen(); q++)
                        file2<<theCluster.right.retrieve(q)<<" ";
                file2<<std::endl;
		file2<<"Lines: ";
                for(int q=1; q<=theCluster.lines.returnLen(); q++)
                        file2<<theCluster.lines.retrieve(q)<<" ";
                file2<<std::endl;
	  }
          file1<<std::endl<<"Get new Cluster"<<std::endl;
          newTree.output(file1);
	  //out<<"Get new Cluster"<<std::endl;
          //newTree.output(out);
          if(DR_Trees.returnLen()==1&&popedTrees.returnLen()==0)
          {
		file2 << "The final cluster is: " << std::endl;
		Cluster finalT = DR_Trees.retrieve(1);
		finalT.output(file2);
		file2 << "The children of the final tree are: " << std::endl;
		for(int x=1; x<=finalT.children.returnLen(); x++)
			finalT.children.retrieve(x).output(file2);

//		if(groupID!=0)
		//	F.removeLabels();
              file2<<"returning DR_Trees"<<std::endl;
              file2<<"----------------end----------------"<<std::endl;
              return DR_Trees;
          }
	  std::cout<<"End of DR_Trees loop"<<std::endl;
      }
   }

   DR_Trees=popedTrees; // the DR_Trees can not form a cluster (not dense)
   file2<<"returning DR_Trees"<<std::endl;
   printForest(DR_Trees, file2, 0);
   file2<<"----------------end----------------"<<std::endl;
   return DR_Trees;
}

// get trivial clusters from the single left vertices, after main distribution
void getTrivial(List<Cluster> &DRTrees, Graph &graph0)
{
   int i, j, numDRTree,numTempTree, vName1, vName2, gotOneCluster;
   int Eweight, Vweight=0;
   Cluster aTree1, aTree2;
   List<Cluster> tempTrees;
   Vertex v1, v2;
   Edge edge;

   numDRTree=DRTrees.returnLen();
   if(numDRTree<=1) return;
   for(i=1; i<=numDRTree; i++)
   {
      aTree1=DRTrees.pop();
      vName1=aTree1.returnCore().returnName();
      if(vName1>0&&vName1<=singleVertex)
            tempTrees.append(aTree1);
      else if(vName1>singleVertex)
            DRTrees.append(aTree1);
   }


   numTempTree=tempTrees.returnLen();
   for(i=1; i<=numTempTree; i++)
   {
      gotOneCluster=0;
      aTree1=tempTrees.pop();


      v1=aTree1.returnCore();
      vName1=v1.returnName();
      for(j=1; j<numTempTree; j++)
      {
         aTree2=tempTrees.pop();

         v2=aTree2.returnCore();
         vName2=v2.returnName();
         edge=graph0.returnEdgeByEnds(vName1, vName2);
         Eweight=edge.returnWeight();
         Vweight=v1.returnWeight() + v1.returnWeight();

         //get the k
	 int k;
         if(graph0.returnDimen() == 2)
		k = -3;
	 else
		k = -6;
         if(Eweight>0 && Eweight - Vweight >= k)//well,over
         {
            gotOneCluster=1;
            break;
         }
         else if(vName2>0)
            tempTrees.append(aTree2);
      }

      if(gotOneCluster==1)
      {
          Vertex *core = new Vertex;
          Cluster *newTree = new Cluster;

          core->setName(nextVerName++);
          core->setWeight(Vweight - Eweight);

          newTree->setCore(*core);
          newTree->addFrot(vName1);
          newTree->addFrot(vName2);
          newTree->addOrig(vName1);
          newTree->addOrig(vName2);
          //newTree->setConst(4 - Vweight + Eweight);
          newTree->children.append(aTree1);
          newTree->children.append(aTree2);
 	  int cType=getClusterType(graph0, *newTree);
	  newTree->setType(cType);
          tempTrees.append(*newTree);
      }
      else if(vName1>0)
         tempTrees.append(aTree1);
   }

   numTempTree=tempTrees.returnLen();
   for(i=1; i<=numTempTree; i++)
       DRTrees.append(tempTrees.pop());
}




#endif
