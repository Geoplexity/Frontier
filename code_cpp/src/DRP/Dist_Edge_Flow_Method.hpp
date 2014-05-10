//file: mfa6Dist.cpp
/*Contains the distribute-edge flow method  and the pushoutside method
used by the Distribute-forest, Distribute-cluster methods,
and all the copying methods associated with it. */


#ifndef DIST_EDGE_FLOW_METHOD_HPP
#define DIST_EDGE_FLOW_METHOD_HPP

#include <iostream>
#include <fstream>
#include <math.h>

#include "../GDS/GDS.hpp"

#include "Cluster_Get.hpp"



//determines if cluster is a special case for pushing more weight outside.
bool specialPushCase(Graph &graph0, Cluster &C)
{
    if(C.returnOrigLen()!=3) return false;

    List<int> origs;

    origs=C.returnOrig();

    int i,length;
    int pointCount, lineCount;

    pointCount=lineCount=0;

    length=origs.returnLen();

    for(i=1;i<=length;i++)
    {
       switch(graph0.returnVertByName(origs.retrieve(i)).returnType())
       {
	     case 6: pointCount++;
		     break;
             case 7: lineCount++;
		     break;
       }
    }

    if(pointCount==2 && lineCount==1) return true;
    if(pointCount==3) return true;

    return false;
}


//copies only thet labels from g0 to g1
void copyLabel(Graph &g0, Graph &g1)
{
    int i, nVer, nEdg;
    Vertex *v;
    Edge *e;

    nVer=g0.returnNumVer();
    for(i=1;i<=nVer;i++)
    {
        v=g1.VertAddr(g0.returnVertByIndex(i).returnName());
        v->setLabel(g0.returnVertByIndex(i).returnLabel());
    }
    nEdg=g0.returnNumEdg();
    for(i=1;i<=nEdg;i++)
    {
        e=g1.EdgeAddr(g0.returnEdgeByIndex(i).returnName());
        e->setLabel(g0.returnEdgeByIndex(i).returnLabel());
    }
    return;
}

//delete inner vertices and edges of newTree from F
void delInnerVer(Graph &F, Graph &graph0)
{
   int i, j, nVerF, vName, nIncid, eOldName;
   Vertex vOld;

   //out<<"before delete inner vertex of new cluster"<<std::endl;
   //F.output(out);

   nVerF=F.returnNumVer();
   for(i=1;i<=nVerF;i++)
   {
     vOld=F.returnVertByIndex(1);
     F.delVer(vOld);
     if(vOld.returnScan()<=-1) // inner vertices
     {
         vName=vOld.returnName();
         graph0.freezeVertForever(vName);
         nIncid=vOld.numIncid();
         for(j=1;j<=nIncid;j++)
         {
            //eOld=F.returnEdgeByName(vOld.returnIncid(j));
            //out<<"edge "<<eOld.returnName()<<" deleted"<<std::endl;
            //if(eOld.returnName()>0) F.delEdg(eOld);
            eOldName=vOld.returnIncid(j);
            //out<<"edge "<<eOldName<<std::endl;
            F.delEdgeByName(vOld.returnIncid(j));
         }
     }
     else
     {
        //if(vOld.returnLabel()<0)
        //   vOld.setLabel(1);
        F.appendVertex(vOld);
     }
   }
}

// distribute edge when K!=0
int distributEdge(Edge &edge, Graph &F, std::ostream &file2)
{
  int a, K;
  Graph CF;

  file2<<"---------------start---------------"<<std::endl;
  file2<<"DistributeEdge edge="<<edge.returnName()<<std::endl;

  a=F.distribute0(edge, file2);

  if (a==1)
  {
     file2<<"distribute0 returned 1, found dense in phase one"<<std::endl;
     file2<<"returning 1"<<std::endl;
     file2<<"----------------end----------------"<<std::endl;
     return 1; //Found dense in Phase one
  }
  else
  {
     K=copyG(F, CF);   // copy F to CF and return K value
     CF.delEdg(edge);
     edge.setWeight(K);
     a=CF.distribute0(edge, file2);
     if (a==1)
     {
         file2<<"distribute0 for CF returned 1, found dense in phase two"<<std::endl;
         file2<<"returning 2"<<std::endl;
         file2<<"----------------end----------------"<<std::endl;
         copyLabel(CF, F); // copy label from CF to F
         return 2;
     }

     file2<<"distribute0 returned 0, no dense graph found"<<std::endl;
     file2<<"returning 0"<<std::endl;
     file2<<"----------------end----------------"<<std::endl;
     return 0;
  }
}

//remove one flow which is into C. Return the edge name
int removeFlow(Cluster &C, Graph &F, Graph &graph0)
{
	int fvName;
	Vertex *fv;
	List<Edge> eList = F.returnElist();
	List<int> incid;

	std::ofstream outf("removeFlow.out", std::ios::app);

	outf << "AT BEGINNING +++++++++++++++" << std::endl << "The F is: " << std::endl;
	F.output(outf);
	outf << "The cluster is: " << std::endl;
	C.output(outf);

	for(int i=1; i<=C.returnFronLen(); i++)
	{
		outf << "i = " << i << std::endl;
		fvName = C.returnFrontier(i);
		fv = F.VertAddr(fvName);
		outf << "the frontier vertex is: " << *fv << std::endl;
		incid = fv->returnIncid();
		for(int j=1; j<=incid.returnLen(); j++)
		{
			outf << "j = " << j << std::endl;
			int eName = incid.retrieve(j);
			Edge* ep = F.EdgeAddr(eName);
			//F has the edge
			if(ep!=NULL)
			{
				outf << "The choosed edge is: " << *ep << std::endl;
				if(ep->returnEnd1()==fvName && ep->returnFlow1()>0)
				{
					ep->setFlow1(ep->returnFlow1()-1);
					//ep->setWeight(ep->returnWeight()-1);
					fv->setEstFlow(fv->returnEstFlow()-1);
					fv->setPosFlow(fv->returnPosFlow()-1);
	                                outf << "The choosed edge is: " << *ep << std::endl;

					return eName;
				}
				else if(ep->returnEnd2()==fvName && ep->returnFlow2()>0)
				{
					ep->setFlow2(ep->returnFlow2()-1);
					//ep->setWeight(ep->returnWeight()-1);
					fv->setEstFlow(fv->returnEstFlow()-1);
					fv->setPosFlow(fv->returnPosFlow()-1);
					return eName;
				}
			}
		}//for each incid edge of current vertex
	}//for each frontier vertex
	outf << "return 0" << std::endl;
	return 0;
}


//see attached paper documentation for description of push outside
int pushOutside(Cluster &C, Graph &F, Graph &graph0, std::ostream &file1, std::ostream &file2)
{
   int i, j, a, K, nFront;
   int coreName, v1Name, v2Name, incidName, innerName, targetName;
   Vertex core, v1, v2; // v1--frontier, v2--vertex in F
   Edge incidEdge, targetEdge;
   List<int> frontierVs;
   Graph CF, CCF;

   file2<<"---------------start---------------"<<std::endl;
   file2<<"Push Outside cluster="<<C.returnName()<<std::endl;
   F.output(file2);

   core=C.returnCore();
   frontierVs=C.returnFrontiers();
   coreName=core.returnName();
   nFront=frontierVs.returnLen();
   if(nFront==0) return 0;

   for(i=1;i<=nFront;i++)    // for every frontier vertices
   {
      v1Name=frontierVs.retrieve(i);   //frontier vertex
      F.defrostOneVert(v1Name);
   }

   // add core to F
   if(core.returnWeight()!=0)
      F.appendVertex(core);           // add core to F

   file2<<"OUTER DISTRIBUTE"<<std::endl;

   Edge targetEdgeTemp;
   int totalFlow;

   //distribute outer edges
   for(i=1;i<=C.returnOuterELen();i++)
   {
	totalFlow=0;
	targetEdgeTemp=C.returnOuterE(i);
	if(targetEdgeTemp.returnName()==0) continue;
        file2<<"Testing Outer Edge: "<<targetEdgeTemp<<std::endl;
	if(!F.hasEdge(targetEdgeTemp.returnName()))
		F.appendEdge(targetEdgeTemp);
	totalFlow+=F.returnEdgeByName(targetEdgeTemp.returnName()).returnFlow1();
	totalFlow+=F.returnEdgeByName(targetEdgeTemp.returnName()).returnFlow2();
	file2<<"Total Flow: "<<totalFlow<<std::endl;
	if(totalFlow>0) continue;
	if(F.hasEdge(targetEdgeTemp.returnName()))
		F.delEdgeByName(targetEdgeTemp.returnName());
	F.distribute0(targetEdgeTemp, file2);
   }

   file2<<"OUTER DISTRIBUTE END"<<std::endl;

   K=copyG(F, CF);    // copy from F to CF

   file2<<"INNER DISTRIBUTE"<<std::endl;

   //distribute inner edges
   file2 << "Look at the results of distributing inner edges" << std::endl;
   for(i=1;i<=nFront;i++)
   {
      F.distribute0(C.returnInnerE().retrieve(i), file2);
   }

   file2<<"INNER DISTRIBUTE END"<<std::endl;


   file2<<"BEGIN FILLER SECTION"<<std::endl;

// NEW FILLER SECTION

   CF.output(file2);

   int nInner=C.returnInnerELen();
   int count, per, highBreak;
   double temp1, iPart, fPart;
   Edge innerEdge;

   a=0;
   int kValue=K-1;

   if(C.returnType()==2)
   {
        kValue--;
   }

   temp1=kValue/nInner;
   fPart=modf(temp1, &iPart);
   per=(int) iPart;
   highBreak=(kValue)-(per*nInner);

   file2<<"PER: "<<per<<" HIGHBREAK: "<<highBreak<<std::endl;
   Graph oldCF;
   int rmEdgeName;
   for(i=1;i<=nInner;i++)
   {
        innerEdge=C.returnInnerE(i);
        innerName=innerEdge.returnName();
        innerEdge.incrsWt(i<=highBreak ? per+1 : per);
        copyG(CF, oldCF);
        a=CF.distribute0(innerEdge, file2);
        if(a==1)
        {//fail to distribute inner edge
        	//restore CF
        	copyG(oldCF, CF);
        	rmEdgeName = removeFlow(C, CF, graph0);
		file2 << "remove edge is: " << rmEdgeName << std::endl;
        	//restore i to distribute the inner edge again.
        	i--;
        }
      	file2 <<"The inner result is: " <<  a << std::endl;
   }

   CF.output(file2);

// NEW FILLER SECTION

/** OLD FILLER SECTION **

   CF.output(file2);

   int nInner=C.returnInnerELen();
   int count, per, highBreak;
   double temp1, iPart, fPart;
   Edge innerEdge;

   a=0;
   int kValue=K-1;

   if(C.returnType()==2)
   {
	kValue--;
   }

   temp1=kValue/nInner;
   fPart=modf(temp1, &iPart);
   per=(int) iPart;
   highBreak=(kValue)-(per*nInner);

   file2<<"PER: "<<per<<" HIGHBREAK: "<<highBreak<<std::endl;

   for(i=1;i<=nInner;i++)
   {
	innerEdge=C.returnInnerE(i);
	innerName=innerEdge.returnName();
	innerEdge.setWeight(i<=highBreak ? per+1 : per);
        CF.delEdgeByName(innerName);
	a+=CF.distribute0(innerEdge, file2);
   }

//   if(a>0)
//	exit(2);

   CF.output(file2);

** END OLD FILLER SECTION **/

   file2<<"END FILLER SECTION"<<std::endl;

   CCF.makeEmpty();

   for(i=1;i<=nFront;i++)    // for every frontier vertices
   {
      v1Name=frontierVs.retrieve(i);
      for(v2Name=1;v2Name<nextVerName;v2Name++)
      {
         if(CF.hasVert(v2Name)) v2=CF.returnVertByName(v2Name);
         targetEdge=CF.returnEdgeByEnds(v1Name,v2Name);
         targetName=targetEdge.returnName();

	 if(C.returnOuterE().hasElem(targetEdge)) continue;

         if(targetName!=0&&v2Name!=coreName&&!frontierVs.hasElem(v2Name)
            &&(!v2.isFrozen()))
         {
		  if(targetEdge.returnFlow1()+targetEdge.returnFlow2()==0)
		  {
			if(F.hasEdge(targetEdge.returnName()))
				F.delEdgeByName(targetEdge.returnName());
			file2<<"Distribute Edge in F"<<std::endl;
			a=F.distribute0(targetEdge, file2);
			if(a!=0)
                  	{
                     		file2<<"distribute0 returned "<<a<<" for the frontier edges"<<std::endl;
                     		file2<<"returning "<<a<<std::endl;
                     		file2<<"----------------end----------------"<<std::endl;
                     		return a;
                  	}

			if(CF.hasEdge(targetEdge.returnName()))
                                CF.delEdgeByName(targetEdge.returnName());
			file2<<"Distribute Edge in CF"<<std::endl;
			a=CF.distribute0(targetEdge, file2);
			if(a!=0)
                  	{
                     		copyLabel(CF, F);  // from CCF to F
                     		file2<<"distribute0 returned "<<a<<" for the frontier edges"<<std::endl;
                     		file2<<"returning "<<a<<std::endl;
                     		file2<<"----------------end----------------"<<std::endl;
                     		return a;
                  	}
		  }

                  copyG(CF, CCF);  // copy from CF to CCF
                  CCF.delEdgeByName(targetName);
		  int type=C.returnType();
		  file2<<"type value read in pushOutside="<<type<<std::endl;
  		  if(type==2)
		  {
			targetEdge.setWeight(2);
		  }
		  else
		  {
			targetEdge.setWeight(1);
		  }

                  a=CCF.distribute0(targetEdge, file2);
                  if(a!=0)
                  {
                     copyLabel(CCF, F);  // from CCF to F
                     file2<<"distribute0 returned "<<a<<" for the frontier edges"<<std::endl;
                     file2<<"returning "<<a<<std::endl;
                     file2<<"----------------end----------------"<<std::endl;
                     return a;
                  }
         }
      }
   }
   file2<<"distribute0 returned 0 for all edges"<<std::endl;
   file2<<"returning 0"<<std::endl;
   file2<<"----------------end----------------"<<std::endl;
   return 0;
}

//only for trivial dense subgraph
int pushOutside0(Graph &F, std::ostream &file2)
{
   int i, j, a, diff=0, nVerF, numTrivial, v1Name, v2Name, innerName,outName;
   Vertex v1, v2;
   List<Vertex> trivial;
   Edge innerEdge, outEdge;
   List<Edge> innerEdges;
   Graph CF, CCF;

   file2<<"---------------start---------------"<<std::endl;
   file2<<"PushOutside0"<<std::endl;
   file2<<"Contents of F"<<std::endl;
   F.output(file2);

   copyG(F, CF);    // copy from F to CF
   nVerF=F.returnNumVer(); // # of vertices in F

   //find the vertices belong to trivial dense subgraph
   for(i=1;i<=nVerF;i++)
   {
       v1=F.returnVertByIndex(i);
       if(v1.returnLabel()!=0)
       {
          trivial.append(v1);
          diff=diff+v1.returnWeight();
       }
   }

   //calculate the left capacity
   numTrivial=trivial.returnLen();
   for(i=1;i<numTrivial;i++)
   {
       v1=trivial.retrieve(i);
       v1Name=v1.returnName();
       for(j=i+1;j<=numTrivial;j++)
       {
          v2=trivial.retrieve(j);
          v2Name=v2.returnName();
          innerEdge=F.returnEdgeByEnds(v1Name,v2Name);
          innerName=innerEdge.returnName();
          if(innerName)
          {
             diff=diff-innerEdge.returnWeight();
             innerEdges.append(innerEdge);
          }
       }
   }

   //make the vertices belong to trivial dense subgraph full
   innerEdge=innerEdges.retrieve(1);
   innerName=innerEdge.returnName();
   CF.delEdgeByName(innerName);
   innerEdge.setWeight(diff);
   CF.distribute0(innerEdge, file2);

   //push one unit out side
   for(i=1;i<=numTrivial;i++)
   {
      v1=trivial.retrieve(i);
      v1Name=v1.returnName();
      for(j=1;j<=nVerF;j++)
      {
         v2=F.returnVertByIndex(j);
         v2Name=v2.returnName();
         outEdge=F.returnEdgeByEnds(v1Name,v2Name);
         outName=outEdge.returnName();
         if(v2.returnLabel()==0&&(!v2.isFrozen())&&outName)
         {
             copyG(CF,CCF); // from CF to CCF
             CCF.delEdgeByName(outName);
	     if(checkSpecialExtensionCase(CCF, std::cout))
		outEdge.setWeight(2);
	     else outEdge.setWeight(1);   // push one unit out side
             a=CCF.distribute0(outEdge, file2);  //distribute outEdge
             if(a!=0)
             {
                copyLabel(CCF, F); // from CF to F
                file2<<"distribute0 returned "<<a<<" for edge"<<outName<<std::endl;
		file2<<"ending contents of F"<<std::endl;
		F.output(file2);
                file2<<"returning "<<a<<std::endl;
                file2<<"----------------end----------------"<<std::endl;
                return a;
             }//end of if(a!=0)
         } //end of if(v2.returnLabel()==0)
      }//end of for(j=1;j<=nVerF;j++)
   }
   file2<<"pushOutside0 failed to extend cluster"<<std::endl;
   file2<<"returning 0"<<std::endl;
   file2<<"----------------end----------------"<<std::endl;
   return 0;
}

//distribute a single vertex
// this is similar to 'for every edge e incident to v1 do'
Cluster * distributeVert
(Vertex &v1, Graph &F, Graph &graph0, std::ostream &file1, std::ostream &file2)
{
   int v1Name, v2Name, edgeName, k, a;
   int K, diff, edgeWeight, v1Weight, v2Weight, ans;
   Vertex v2;
   Edge edge;
   Cluster *newTree;

   file2<<"---------------start---------------"<<std::endl;
   file2<<"distributeVert for v"<<v1.returnName()<<std::endl;
   file2<<"Contents of F: "<<std::endl;

      if(F.returnDimen()==2) K=4;
      if(F.returnDimen()==3) K=7;
      v1Name=v1.returnName();
      F.setDepth(0);
      if(F.hasVert(v1Name)) F.defrostOneVert(v1Name);
      else F.appendVertex(v1);       // F = F + {v}

      F.output(file2);

      for(v2Name=1;v2Name<=singleVertex;v2Name++)
      {
         //out<<"in distributeVert, v2Name="<<v2Name<<std::endl;
         if(F.hasVert(v2Name)) v2=F.returnVertByName(v2Name);
         edge=graph0.returnEdgeByEnds(v1Name,v2Name);
         edgeName=edge.returnName();
         if(edgeName&&F.hasVert(v2Name))
         {
            edgeWeight=edge.returnWeight();
            v1Weight=v1.returnWeight();
            v2Weight=v2.returnWeight();
            diff=edgeWeight+K-1-v1Weight-v2Weight;
/*          if(diff>0)
            {
               cout<<"Overconstrain between vertices v"<<v1Name<<" and v"
               <<v2Name<<std::endl;
               cout<<"Hit 1 to stop"<<std::endl;
               cout<<"Hit 2 to continue (replace it)"<<std::endl;
               cin>>ans;
               if(ans==1)
                 exit(0);
               else
                 edge.setWeight(edgeWeight-diff);
            }
*/
            if(v2.isFrozen())   // the other end in F is frozen
            {
               if(!F.hasEdge(edgeName))
                   F.appendEdge(edge); //not distrbute it, just add it
            }
            else
            {
                if(F.hasEdge(edgeName))
                {
                   edge=F.returnEdgeByName(edgeName);
                   F.delEdgeByName(edgeName);
                }

                if(edge.returnFlow1()+edge.returnFlow2()==0)
                {
                   k=distributEdge(edge, F, file2);
                }
                else
                {
                    F.appendEdge(edge);
                    k=0;
                }

                if(k>0)
                {
                   if(F.numLabeled()>F.returnDimen())
                   {
                      newTree=getCluster(F, graph0);
                      file2<<"Found new Cluster: "<<std::endl;
 		      newTree->output(file2);
                      file2<<"----------------end----------------"<<std::endl;
                      return newTree;
                   }
                   else
                   {
                      if(extension(F, graph0, file1, file2)==1)
                      {
                         newTree=getCluster(F, graph0);
                         if(!newTree->empty())
                         {
                           file2<<"Found new Cluster: "<<std::endl;
                           newTree->output(file2);
                           file2<<"----------------end----------------"<<std::endl;
			   return newTree;
  			 }
                      }
                      a=pushOutside0(F, file2);//only trivial dense subgraph
                      if(a>0)
                      {
                          newTree=getCluster(F, graph0);
			  file2<<"F after cluster creation"<<std::endl;
			  F.output(file2);
                          file2<<"In distVert"<<std::endl;
                          if(!newTree->empty())
			  {
                            file2<<"Found new Cluster: "<<std::endl;
                            newTree->output(file2);
                            file2<<"----------------end----------------"<<std::endl;
                            return newTree;
                          }
                      } //end of if(a>0)
                   } //end of else
                } //end of if(k>0)
            }// end of else
         } //end of if( edgeName&&F.hasVert(v2Name) )
      } //end of for(v2Name=1;v2Name<=singleVertex;v2Name++)

   file2<<"no Clusters found, returning empty cluster"<<std::endl;
   file2<<"----------------end----------------"<<std::endl;
   Cluster *dummy=new Cluster;
   return dummy;
}

// distribute Cluster
Cluster & distributeCl
(Cluster &C, Graph &F, Graph &graph0, std::ostream &file1, std::ostream &file2)
{
   Cluster *newTree;
   int ClusterDepth, v1Name, k;
   Vertex v1;

   file2<<"---------------start---------------"<<std::endl;
   file2<<"DistributeCl cluster"<<C.returnName()<<std::endl;
   file2<<"Contents of F:"<<std::endl;
   F.output(file2);

   v1Name=C.returnCore().returnName();
   if(v1Name>0&&v1Name<=singleVertex) // distributing single vertex
   {
      file2<<"C is a singleton vertex"<<std::endl;
      //out<<"in distributeCl 1"<<std::endl;
      v1=C.returnCore();
      newTree=distributeVert(v1, F, graph0, file1, file2);
      //newTree->output(out);
      //out<<"in distributeCl 11"<<std::endl;
      if(newTree->empty())
          delete newTree;
      else
      {
          file2<<"Found new Cluster:"<<std::endl;
          newTree->output(file2);
          file2<<"----------------end----------------"<<std::endl;
          return *newTree;
      }
   }
   else  // distributing a cluster
   {
       file2<<"C is a cluster"<<std::endl;
       //out<<"in distributeCl 2"<<std::endl;
       //if(C.returnCore().returnName()==19) { C.output(out);}
       ClusterDepth=C.returnCore().returnDepth();
       if(F.returnDepth()<ClusterDepth) F.setDepth(ClusterDepth);
       k=pushOutside(C, F, graph0, file1, file2);
       if(k>0)
       {
          newTree=getCluster(F, graph0);
          file2<<"Found new Cluster:"<<std::endl;
          newTree->output(file2);
          file2<<"----------------end----------------"<<std::endl;
          return *newTree;
       }
   }

   file2<<"no Cluster found, returning empty cluster"<<std::endl;
   file2<<"----------------end----------------"<<std::endl;
   Cluster *dummy=new Cluster;
   return *dummy;
}





#endif
