//file: mfa7DRTree.cpp
/*Methods for distribution including distribute-forest, 
distribute-cluster etc., including all the defrost methods
for incorporating  input groups */

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
  Graph &graph0, ostream &file1,ostream &file2, int groupID, int indentation);

Cluster &distributeTree(Cluster &aDRtree, Graph &F,
  Graph &graph0, ostream &file1, ostream &file2, int indentation)
{
   file2<<"---------------start---------------"<<endl;
   file2<<"DistributeTree"<<endl; 
   file2<<"Contents of F"<<endl;
   F.output(file2);
   file2<<"Contents of aDRTree"<<endl;
   printTree(aDRtree, file2, 0);

   int i, numChild, groupID;
   List<Cluster> Children, distriedChild;
   Cluster *newDRtree = new Cluster (aDRtree);

   if(newDRtree->returnCore().returnDepth()==0&&newDRtree->returnGroup()==0)
   {
	file2<<"Returning from depth 0"<<endl;
	file2<<"----------------end----------------"<<endl;
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
      file1<<"Group="<<groupID<<endl;
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

   file2<<"Returning from end"<<endl;
   file2<<"Ending value of newDRTree"<<endl;
   printTree(*newDRtree, file2, 0);
   file2<<"----------------end----------------"<<endl;
   return *newDRtree;
}

void explicitChild(Cluster &newTree, List<int> &nameList,List<Cluster> &forest)
{
   int i, j, numOfTree, childName, numOriginalV;
   Cluster aChild;
   Vertex childCore;
   List<int> childOrig;
   List<Cluster> tempTrees;

   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      childCore=aChild.returnCore();
      childName=childCore.returnName();
      if(nameList.hasElem(childName))
      {
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


int getClusterType(Graph &graph0, Cluster &theCluster)
{
    int LSCount, distCount, pointCount, totalCount, i;
    
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
	cout<<"Edge Name: "<<tempEdge.returnName()<<endl;
	cout<<"Edge Code: "<<getEdgeCodeDRPlanner(tempEdge,theCluster)<<endl;
	cout<<"Edge Type: "<<tempEdge.returnType()<<endl;
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
               
    cout<<"Getting Cluster "<<theCluster.returnName()<<" Type: "<<endl;
    cout<<"LSCount:     "<<LSCount<<endl;
    cout<<"distCount:   "<<distCount<<endl;
    cout<<"pointCount:  "<<pointCount<<endl;
    cout<<"totalCount:  "<<totalCount<<endl;
    cout<<"notIncid:    "<<notIncidCount<<endl;

    if(LSCount==0 && distCount>1)
    {
	cout<<"Type set to 0"<<endl;
	return 0;
    }

    if(notIncidCount==0 && childNotExtOver==0)
    {
        cout<<"Type set to 2 (2)"<<endl;
        return 2;
    }	

    if((distCount==1 && pointCount==2 && totalCount==2) ||
       (LSCount>0 && distCount==1 && notIncidCount==1))	
    {
	cout<<"Type set to 2 (1)"<<endl;
        return 2;
    }
 
    if(LSCount==1 && distCount==0)
    {
	cout<<"Type set to 1"<<endl;
        return 1;
    }

    cout<<"Type set to 0"<<endl;
    return 0;
}

void getTrivial(List<Cluster> &DRTrees, Graph &graph0);

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
	
	ofstream outf("canCombine.out", ios::app);
	outf << "The beginning:" << endl;
	
	outf << "The c is:" << endl;
	c.output(outf);
	outf << "The C10 is: "<<endl;
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

	//no overlapped vertices, return false
	if(lenOverlap <= 0) 	return false;

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
					outf << "ERROR!!" << endl;
				}
			}
		} 
	}

	for(i = 1; i <= lenOverlap; i++)
		for(int j = 1; j <= lenOverlap; j++)
			if(index[j] == i)
				lenIndepent++;

	outf << "The lenIndepent is: " << lenIndepent << endl;
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
	outf<<"Contents of F1"<<endl;
	F1.output(outf);   
	outf<<"Contents of graph1"<<endl;
	graph1.output(outf);   
	outf<<"Contents of DRTrees"<<endl;
	printForest(DR_Trees, outf, 0);	
	
	DR_Trees=distributeForest(DR_Trees, F1, graph1, cout, cout, 0,0);
 	getTrivial(DR_Trees, F1);
	outf<<"Contents of new DRTrees"<<endl;
	printForest(DR_Trees, outf, 0);	
	
	//overlapped part is not a cluster, return true
	if(DR_Trees.returnLen() > 1) return true;

	//the overlapped cluster is not a rot-sym object
	if(getClusterType(graph1, DR_Trees.retrieve(DR_Trees.returnLen())) != 2) 
	{
		outf << "the cluster type is 2" << endl;
		return true;
	}
	else
	{
		//if one rot-sym cluster is overlapped with another cluster, return true)
		if (getClusterType(graph0, cl0) == 2 || getClusterType(graph0, c) == 2)
		
			return true;
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
				outf << "ver1 = " << ver1 << ", ver2 = " << ver2 << endl; 
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

   ofstream outf("implicit.out", ios::app);
   outf<< "(The Beginning)The newTree's name is:" << newTree.returnName() << endl;
   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      if(canCombine(aChild, newTree, F, graph0))
      {
	  outf << "The aChild is:" << endl;
	  aChild.output(outf);
	  outf << "The newTree is: "<<endl;
	  newTree.output(outf);
	  outf << "The old F is:" << endl;
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
          outf<< "The new F is:" << endl;
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
 	  outf << "The new newTree is: "<<endl;
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
   Cluster aChild;
   Vertex childCore;
   List<Edge> innerE, outerE;

   //out<<"add child for"<<endl;
   //printTree(newTree,out,0);

   nameList=newTree.returnOrig(); //vertex name list forms a new cluster

   //check each item (not poped) left in DR_Trees with the name list
   explicitChild(newTree, nameList, DR_Trees);
   cout << "explicitChild with DR_Trees" << endl;
   printForest(DR_Trees, cout, 0);

   // check each poped item with the name list
   explicitChild(newTree, nameList, popedTrees);

   cout << "explicitChild with popedTrees" << endl;
   printForest(DR_Trees, cout, 0);

   // look implicit child in DR_Trees
   implicitChild(newTree, nameList, DR_Trees, F, graph0);
   cout << "implicitChild with DR_Trees" << endl;
   printForest(DR_Trees, cout, 0);

   // look implicit child in popedTrees
   implicitChild(newTree, nameList, popedTrees, F, graph0);
   cout << "implicitChild with popedTrees" << endl;
   printForest(DR_Trees, cout, 0);

   //The left names in the list are frontier vertices shared by two or more
   //new clusters. We have to create one or more clusters for each of them.
   listLen=nameList.returnLen();
   for(i=1;i<=listLen;i++)
   {
       childName=nameList.retrieve(i);
       childCore=F.returnVertByName(childName);
       frontiers0.append(childName);
       originalV0.append(childName);
       aChild.formCl(childCore,frontiers0,innerE,outerE,originalV0);
       newTree.children.append(aChild); // add a single vertex child
   }
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
	
	ofstream outf("checkBananas.out", ios::app);
	outf << " The beginning:" << endl;
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

	outf<<endl;

	int j;
	Vertex testVert;
	Edge testEdge;
	List<int> toRemove;

	for(i=1;i<=overlappedVerts.returnLen();i++)
	{
		testVert=graph0.returnVertByName(overlappedVerts.retrieve(i));
		outf<<testVert<<endl;
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

	outf<<"After line check"<<endl;

	for(i=1;i<=toRemove.returnLen();i++)
		overlappedVerts.deleteElem(toRemove.retrieve(i));	

	outf<<"After line remove"<<endl;

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
		
		outf<<theEdge<<endl;
		outf<<"value="<<value<<endl;
		outf<<"typeValue="<<typeValue<<endl;
		outf<<"Edgebetween="<<(edgeBetween ? "true" : "false")<<endl;

	}

	outf<<"Final Edgebetween="<<(edgeBetween ? "true" : "false")<<endl;
  
	outf<<"After edge between"<<endl;

	bool partitionable;
	List<int> over1, over2;

	outf<<"OverlappedVerts: ";
        for(i=1;i<=overlappedVerts.returnLen();i++) 
                outf<<overlappedVerts.retrieve(i)<<" ";
        outf<<endl;	

	over1.append(overlappedVerts.retrieve(1));
	overlappedVerts.deleteIndex(1);

	for(i=1;i<=overlappedVerts.returnLen();i++)
		for(int j=1;j<=over1.returnLen();j++)
		{
			outf<<"OverLap: "<<overlappedVerts.retrieve(i)<<" Over1: "<<over1.retrieve(j)<<endl;
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
				partitionable==false;
			else    partitionable==true;
			
		}
	}

	outf<<"After partitionable"<<endl;

	outf<<"Over1: ";
	for(i=1;i<=over1.returnLen();i++)
		outf<<over1.retrieve(i);
	outf<<endl;
	outf<<"Over2: "; 
        for(i=1;i<=over2.returnLen();i++)
                outf<<over2.retrieve(i);
        outf<<endl;

	if(partitionable && !edgeBetween)
		return 0;

	if(partitionable && edgeBetween)
		return 3;


	return -4;
}




void setAlias(Graph &graph0, Cluster &theCluster)
{
   if(theCluster.returnType()!=2) return;

   int i, length=theCluster.returnOrigLen();

   int alias=-1;

   for(i=1;i<=length;i++)
        if(graph0.returnVertByName(theCluster.returnOrigV(i)).returnType()==7)
	{
		alias=theCluster.currAlias=theCluster.returnOrigV(i);
		cout<<"Alias found: "<<alias<<endl;
		break;
	}

   if(alias==-1)
   {
	cout<<"Error: No Alias found."<<endl;
	return;
   }

   Edge currEdge;
   int currVertID, currVertType, aliasPart;

   for(i=1;i<=length;i++)
   {
	currVertID=theCluster.returnOrigV(i);
        currVertType=graph0.returnVertByName(currVertID).returnType();

        if(currVertType==7)
        {
                theCluster.lines.append(currVertID);
                cout<<"Line appended: "<<currVertID<<endl;
                continue;
        }
        
        if(currVertType!=6) continue;

	currEdge=graph0.returnEdgeByEnds(alias,currVertID);
	if(currEdge.returnName()==0) continue;
	if(currEdge.returnType()!=1) continue;

	cout<<"Checking Edge: "<<currEdge<<endl;

	if(currEdge.returnEnd1()==alias)
		aliasPart=currEdge.returnPart1();
	else
		aliasPart=currEdge.returnPart2();

	if(aliasPart==1)
	{
		theCluster.left.append(currVertID);
		cout<<"Left appended: "<<currVertID<<endl;
	}

	if(aliasPart==2)
	{
		theCluster.right.append(currVertID);
		cout<<"Right appended: "<<currVertID<<endl;
	}
   }
}

bool isDense(Cluster &C1, Cluster &C2, Graph &graph0)
{
   int i, numVert, numEdge, vertWeight, edgeWeight;
   List<int> origList;
   Edge testEdge;
   int K;

   cout<<"IN DENSE"<<endl;

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
	cout<<testEdge<<endl;
	int temp1E=testEdge.returnEnd1();
	int temp2E=testEdge.returnEnd2();

	if(!origList.hasElem(temp1E)) continue;
  	if(!origList.hasElem(temp2E)) continue;
	edgeWeight+=testEdge.returnWeight();
   }

   cout<<"Edge Weight: "<<edgeWeight<<endl;
   cout<<"Vert Weight: "<<vertWeight<<endl;

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

   cout<<"IN PSEUDOMINDENSE"<<endl;

   theCluster.output(cout);

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
			cout<<"Cluster 1: "<<theCluster.children.retrieve(i).returnName()<<endl;
			tempCluster.children.retrieve(i).output(cout);
			cout<<"Cluster 2: "<<theCluster.children.retrieve(j).returnName()<<endl;
			tempCluster.children.retrieve(j).output(cout);
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


List<Cluster> &distributeForest(List<Cluster> &DR_Trees, Graph &F,
  Graph &graph0, ostream &file1, ostream &file2, int groupID, int indentation)
{
   int i, j, len;
   Cluster aTree, distried, newTree;
   List<Cluster> popedTrees;

   file2<<"---------------start---------------"<<endl;
   file2<<"DistributeForest"<<endl;
   file2<<"Contents of F"<<endl;
   F.output(file2);   
   file2<<"Contents of DRTrees"<<endl;
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

	file2 << "groupID is: " << groupID << endl;
   if(groupID!=0)
   {
	
       F.freeze();
       graph0.freeze();
       defrostForest(DR_Trees, F);
       defrostForest(DR_Trees, graph0);
   }

   while(DR_Trees.returnLen()>0)
   {
      cout<<"Start of DR_Trees Loop"<<endl;
      List<Cluster> tempDR_Trees;
      List<Cluster> tempPopedTrees;
      
      if(graph0.returnDimen()==3)
      {
          for(int i=1;i<=DR_Trees.returnLen();i++)
                  tempDR_Trees.append(DR_Trees.retrieve(i));
      }
      file2<<"Contents of DR_Trees"<<endl;
      printForest(DR_Trees, file2, 0);      
      printForest(DR_Trees, cout, 0);
      cout<<"Before extension"<<endl;
      file1<<"---------------------------------------------"<<endl;
      if(extension(F, graph0, file1, file2))
      {
         file2<<"Found new Cluster by Extension:"<<endl;
         newTree=*getCluster(F, graph0);
         newTree.output(file2);
         file2<<"----------------end----------------"<<endl;
      }
      else
      {	 
	 cout<<"In distributeCL path"<<endl;
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
        
         file1<<"Pop Cluster, "<<aTree.returnCore().returnName();
         file1<<"  Group="<<aTree.returnGroup()<<endl;
         file2<<"Pop Cluster, "<<aTree.returnCore().returnName()<<endl;
         newTree=distributeCl(aTree, F, graph0, file1, file2);
      }
      
      file2 << "Let's see" << endl;
      file2 << "DR_Trees" << endl;
      printForest(DR_Trees, file2, 0);
      file2 << "popedTrees" << endl;
      printForest(popedTrees, file2, 0);
      file2 << "tempDR_Trees" << endl;
      printForest(tempDR_Trees, file2, 0);
      
      if(!newTree.empty()) {

	  file2<<"Before addChild "<<endl;
          newTree.output(file2);
          file2<<"Found new Cluster: "<<endl;
          addChild(newTree, popedTrees, DR_Trees, F, graph0);
	  cout<<"After addchild"<<endl;
	  graph0.output(cout);
	  pseudoMinDense(newTree,graph0);
          //constrain=F.constrainDegree();
          //file2<<"constrain="<<constrain<<endl;
          //newTree.setConst(constrain); //1--well constrained, 2,3...--over
          file2<<"In DistForest right before cluster output"<<endl;
          newTree.output(file2);
          file2<<"Starting Contents of F:"<<endl;
          file2<<"Right before deleting inner edges"<<endl;
	  F.output(file2);
	  Graph tempGraph;
	  copyG(F,tempGraph);
	  tempGraph.removeLabels();
          delInnerVer(F, graph0);
	  cout<<"after deleting inner edges"<<endl;
          file2<<"deleting inner edges"<<endl;
          F.output(file2);
          DR_Trees.append(newTree);
	  file2<<"appending newTree"<<endl;
	  file2<<"Contents of DR_Trees after extension append"<<endl;
          printForest(DR_Trees, file2, 0);	  
	  printForest(DR_Trees, cout,0);	  

	  if(graph0.returnDimen()==3)
	  {

	  int checkValue=checkClusterForBananas(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  file2<<"Checks value: "<<checkValue<<endl;
	  int cType;
	  switch(checkValue)
	  {
		case -1:  	file2<<"Before ClusterType"<<endl;
	  			cType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  			file2<<"After ClusterType, the cluster type is:" << cType << endl;
			        DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(cType);
                                file2<<"After checkValue switch"<<endl;
				break;
		case 0:		DR_Trees.makeEmpty();
				file2<<"After DR_Trees.makeEmpty()"<<endl;
				file2<<"Length: "<<tempDR_Trees.returnLen()<<endl;
				copyG(tempGraph,F);
				//restore the original DR_Trees
				for(int i=1;i<=tempDR_Trees.returnLen();i++)
				{
					file2<<"Restoring old DR_Trees: "<<i<<endl;
			                DR_Trees.append(tempDR_Trees.retrieve(i));
					file2<<"done"<<endl;
				}
				//restore the original DR_Trees
				popedTrees.makeEmpty();
				for(int i=1;i<=tempPopedTrees.returnLen();i++)
				{
					file2<<"Restoring old PopedTrees: "<<i<<endl;
			                popedTrees.append(tempPopedTrees.retrieve(i));
					file2<<"done"<<endl;
				}	
				break;
		case 3:		DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(3);
				file2<<"TYPE SET TO 3"<<endl;
				break;
		default:	file2<<"ERROR UNEXPECTED RETURN!"<<endl;
				break;
	  }
	  if(DR_Trees.returnLen() == 0) continue;
	  file2<<"Before ClusterType"<<endl;
	  cType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  file2<<"After ClusterType, the cluster type is:" << cType << endl;
	  DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(cType);
	  file2<<"After checkValue switch"<<endl;

	  if(cType==2)
	  {
		DR_Trees.addrByIndex(DR_Trees.returnLen())->returnCore().incrsWt(-1);
		setAlias(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
		Cluster theCluster = DR_Trees.retrieve(DR_Trees.returnLen());
		file2<<"After aliasing for cluster: "<<theCluster.returnName()<<endl;
		file2<<"Alias: "<<theCluster.currAlias<<endl;
		file2<<"Left: ";
		for(int q=1; q<=theCluster.left.returnLen(); q++)
			cout<<theCluster.left.retrieve(q)<<" ";
		file2<<endl;
		file2<<"Right: ";
                for(int q=1; q<=theCluster.right.returnLen(); q++)
                        cout<<theCluster.right.retrieve(q)<<" ";
                file2<<endl;
		file2<<"Lines: ";
                for(int q=1; q<=theCluster.lines.returnLen(); q++)
                        cout<<theCluster.lines.retrieve(q)<<" ";
                file2<<endl;
	  }

	  file2<<"end of check bananas if"<<endl;
	  }
          file1<<endl<<"Get new Cluster"<<endl;
          newTree.output(file1);
          //out<<"Get new Cluster"<<endl;
          //newTree.output(out);
          if(DR_Trees.returnLen()==1&&popedTrees.returnLen()==0)
          {
              file2<<"returning DR_Trees"<<endl;
              file2<<"----------------end----------------"<<endl;
              return DR_Trees;
          }
	  cout<<"End of DR_Trees loop"<<endl;
      }
   }

   DR_Trees=popedTrees; // the DR_Trees can not form a cluster (not dense)
   file2<<"returning DR_Trees"<<endl;
   printForest(DR_Trees, file2, 0);
   file2<<"----------------end----------------"<<endl;
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

