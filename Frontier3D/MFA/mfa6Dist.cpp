//file: mfa6Dist.cpp
/*Contains the distribute-edge flow method  and the pushoutside method
used by the Distribute-forest, Distribute-cluster methods, 
and all the copying methods
associated with it. */

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

   //out<<"before delete inner vertex of new cluster"<<endl;
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
            //out<<"edge "<<eOld.returnName()<<" deleted"<<endl;
            //if(eOld.returnName()>0) F.delEdg(eOld);
            eOldName=vOld.returnIncid(j);
            //out<<"edge "<<eOldName<<endl;
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
int distributEdge(Edge &edge, Graph &F, ostream &file2)
{
  int a, K;
  Graph CF;

  file2<<"---------------start---------------"<<endl;
  file2<<"DistributeEdge edge="<<edge.returnName()<<endl;

  a=F.distribute0(edge, file2);

  if (a==1) 
  {
     file2<<"distribute0 returned 1, found dense in phase one"<<endl;
     file2<<"returning 1"<<endl;
     file2<<"----------------end----------------"<<endl;
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
         file2<<"distribute0 for CF returned 1, found dense in phase two"<<endl;
         file2<<"returning 2"<<endl;
         file2<<"----------------end----------------"<<endl;
         copyLabel(CF, F); // copy label from CF to F
         return 2;
     }
     
     file2<<"distribute0 returned 0, no dense graph found"<<endl;
     file2<<"returning 0"<<endl;
     file2<<"----------------end----------------"<<endl;
     return 0;
  }
}

//see attached paper documentation for description of push outside
int pushOutside(Cluster &C, Graph &F, Graph &graph0, ostream &file1, ostream &file2)
{
   int i, j, a, K, nFront;  
   int coreName, v1Name, v2Name, incidName, innerName, targetName;
   Vertex core, v1, v2; // v1--frontier, v2--vertex in F
   Edge incidEdge, targetEdge;
   List<int> frontierVs;
   Graph CF, CCF;

   file2<<"---------------start---------------"<<endl;
   file2<<"Push Outside cluster="<<C.returnName()<<endl;

   core=C.returnCore();
   frontierVs=C.returnFrontiers();
   coreName=core.returnName();
   nFront=frontierVs.returnLen();
   if(nFront==0) return 0;

   //defrost frontier vertices and distribute all incident edges
   for(i=1;i<=nFront;i++)    // for every frontier vertices
   {
      v1Name=frontierVs.retrieve(i);   //frontier vertex
      F.defrostOneVert(v1Name);
      for(v2Name=1;v2Name<=singleVertex;v2Name++)
      {
         incidEdge=F.returnEdgeByEnds(v1Name, v2Name);
         incidName=incidEdge.returnName();
         if(F.hasVert(v2Name)) v2=F.returnVertByName(v2Name);
         if((!v2.isFrozen())&&F.hasVert(v2Name)
            &&(incidEdge.returnFlow1()+incidEdge.returnFlow2()==0))
         {
                  F.delEdgeByName(incidName);
                  incidEdge=graph0.returnEdgeByEnds(v1Name, v2Name);
                  if(incidEdge.returnName())
                     F.distribute0(incidEdge, file2);
         }
      }
   }

   // add core to F
   if(core.returnWeight()!=0)
      F.appendVertex(core);           // add core to F

   //distribute inner edges
   for(i=1;i<=nFront;i++)
   {
      F.distribute0(C.returnInnerE().retrieve(i), file2);
   }

   file2<<"BEGIN FILLER SECTION"<<endl;

   K=copyG(F, CF);    // copy from F to CF

//NEW FILLER SECTION

   CF.output(file2);

   int nInner=C.returnInnerELen();
   int count, per, highBreak;
   double temp1, iPart, fPart;
   Edge innerEdge;
   
   a=0;
   int kValue=K-1;

   if(C.returnType()==2)
   {
	cout<<"In Filler, C Type 2"<<endl;
	kValue--;
   	cout<<"kValue: "<<kValue<<endl;
   }

   temp1=kValue/nInner;
   fPart=modf(temp1, &iPart);
   per=(int) iPart;
   highBreak=(kValue)-(per*nInner);

   file2<<"PER: "<<per<<" HIGHBREAK: "<<highBreak<<endl;

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

//END NEW FILLER SECTION


/**OLD FILLER SECTION**

   Edge innerEdge;

   // make vertices of C full
   innerEdge=C.returnInnerE(1);
   innerName=innerEdge.returnName();
   innerEdge.setWeight(K-1);
   CF.delEdgeByName(innerName);
   a=CF.distribute0(innerEdge, file2);  // make vertices of C full

   int a1, a2;	

   if(a!=0)
   {
      K=copyG(F, CCF);
      if(C.returnInnerE().returnLen()>=2)
      {
	Edge innerEdge1, innerEdge2;

	innerEdge1=C.returnInnerE(1);
	innerEdge2=C.returnInnerE(2);
	
	innerEdge1.setWeight(K-2);
	innerEdge2.setWeight(1);
	CCF.delEdgeByName(innerEdge1.returnName());
	CCF.delEdgeByName(innerEdge2.returnName());

	a1=CCF.distribute0(innerEdge1, file2);
	a2=CCF.distribute0(innerEdge2, file2);

	if(a1!=0 || a2!=0)
	{
          copyLabel(CCF, F);  // from CF to F
          file2<<"distribute0 returned "<<a<<" for the filler edge, second pass"<<endl;
          file2<<"returning "<<a<<endl;
          file2<<"----------------end----------------"<<endl;
          return (a1!=0 ? a1 : a2);
	}

	CF.makeEmpty();
	copyG(CCF, CF);
      }
      else
      {

      	copyLabel(CF, F);  // from CF to F
      	file2<<"distribute0 returned "<<a<<" for the filler edge, first pass"<<endl;
      	file2<<"returning "<<a<<endl;
      	file2<<"----------------end----------------"<<endl;
      	return a;
      }
   }

/***END OLD FILLER SECTION***/

   file2<<"END FILLER SECTION"<<endl;

   CCF.makeEmpty();

   for(i=1;i<=nFront;i++)    // for every frontier vertices
   {
      v1Name=frontierVs.retrieve(i);
      for(v2Name=1;v2Name<nextVerName;v2Name++)
      {
         if(CF.hasVert(v2Name)) v2=CF.returnVertByName(v2Name);
         targetEdge=CF.returnEdgeByEnds(v1Name,v2Name);
         targetName=targetEdge.returnName();
         if(targetName!=0&&v2Name!=coreName&&!frontierVs.hasElem(v2Name)
            &&(!v2.isFrozen()))
         {
                  copyG(CF, CCF);  // copy from CF to CCF
                  CCF.delEdgeByName(targetName);
		  int type=C.returnType();
		  cout<<"type value read in pushOutside="<<type<<endl;
  		  if(type==2)
		  {
			targetEdge.setWeight(2);
			cout<<"Pushing out 2"<<endl;
		  }
		  else  
		  {
			targetEdge.setWeight(1);
			cout<<"Pushing out 1"<<endl;
		  }

                  a=CCF.distribute0(targetEdge, file2);
                  if(a!=0)
                  {
                     copyLabel(CCF, F);  // from CCF to F
                     file2<<"distribute0 returned "<<a<<" for the frontier edges"<<endl;
                     file2<<"returning "<<a<<endl;
                     file2<<"----------------end----------------"<<endl;
                     return a;
                  }
         }
      }
   }
   file2<<"distribute0 returned 0 for all edges"<<endl;
   file2<<"returning 0"<<endl;
   file2<<"----------------end----------------"<<endl;
   return 0;
}

//only for trivial dense subgraph
int pushOutside0(Graph &F, ostream &file2)
{
   int i, j, a, diff=0, nVerF, numTrivial, v1Name, v2Name, innerName,outName;
   Vertex v1, v2;
   List<Vertex> trivial;
   Edge innerEdge, outEdge;
   List<Edge> innerEdges;
   Graph CF, CCF;

   file2<<"---------------start---------------"<<endl;
   file2<<"PushOutside0"<<endl;   
   file2<<"Contents of F"<<endl;
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
	     if(checkSpecialExtensionCase(CCF, cout))
		outEdge.setWeight(2);
	     else outEdge.setWeight(1);   // push one unit out side
             a=CCF.distribute0(outEdge, file2);  //distribute outEdge
             if(a!=0)
             {
                copyLabel(CCF, F); // from CF to F
                file2<<"distribute0 returned "<<a<<" for edge"<<outName<<endl;
		file2<<"ending contents of F"<<endl;
		F.output(file2);
                file2<<"returning "<<a<<endl;
                file2<<"----------------end----------------"<<endl;
                return a;
             }//end of if(a!=0)
         } //end of if(v2.returnLabel()==0)
      }//end of for(j=1;j<=nVerF;j++)
   }
   file2<<"pushOutside0 failed to extend cluster"<<endl;
   file2<<"returning 0"<<endl;
   file2<<"----------------end----------------"<<endl;
   return 0;
}

//distribute a single vertex
// this is similar to 'for every edge e incident to v1 do'
Cluster * distributeVert
(Vertex &v1, Graph &F, Graph &graph0, ostream &file1, ostream &file2)
{
   int v1Name, v2Name, edgeName, k, a;
   int K, diff, edgeWeight, v1Weight, v2Weight, ans;
   Vertex v2;
   Edge edge;
   Cluster *newTree;

   file2<<"---------------start---------------"<<endl;
   file2<<"distributeVert for v"<<v1.returnName()<<endl;
   file2<<"Contents of F: "<<endl;

      if(F.returnDimen()==2) K=4;
      if(F.returnDimen()==3) K=7;
      v1Name=v1.returnName();
      F.setDepth(0);
      if(F.hasVert(v1Name)) F.defrostOneVert(v1Name);
      else F.appendVertex(v1);       // F = F + {v}

      F.output(file2);

      for(v2Name=1;v2Name<=singleVertex;v2Name++)
      {
         //out<<"in distributeVert, v2Name="<<v2Name<<endl;
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
               <<v2Name<<endl;
               cout<<"Hit 1 to stop"<<endl;
               cout<<"Hit 2 to continue (replace it)"<<endl;
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
                      file2<<"Found new Cluster: "<<endl;
 		      newTree->output(file2);
                      file2<<"----------------end----------------"<<endl;
                      return newTree;
                   }
                   else
                   {
                      if(extension(F, graph0, file1, file2)==1)
                      {
                         newTree=getCluster(F, graph0);
                         if(!newTree->empty()) 
                         {
                           file2<<"Found new Cluster: "<<endl;
                           newTree->output(file2);
                           file2<<"----------------end----------------"<<endl;
			   return newTree;
  			 }
                      }
                      a=pushOutside0(F, file2);//only trivial dense subgraph
                      if(a>0)
                      {
                          newTree=getCluster(F, graph0);
			  file2<<"F after cluster creation"<<endl;
			  F.output(file2);
                          file2<<"In distVert"<<endl;
                          if(!newTree->empty()) 
			  {
                            file2<<"Found new Cluster: "<<endl;
                            newTree->output(file2);
                            file2<<"----------------end----------------"<<endl;
                            return newTree;
                          }
                      } //end of if(a>0)
                   } //end of else
                } //end of if(k>0)
            }// end of else
         } //end of if( edgeName&&F.hasVert(v2Name) )
      } //end of for(v2Name=1;v2Name<=singleVertex;v2Name++)

   file2<<"no Clusters found, returning empty cluster"<<endl;
   file2<<"----------------end----------------"<<endl;
   Cluster *dummy=new Cluster;
   return dummy;
}

// distribute Cluster
Cluster & distributeCl
(Cluster &C, Graph &F, Graph &graph0, ostream &file1, ostream &file2)
{
   Cluster *newTree;
   int ClusterDepth, v1Name, k;
   Vertex v1;

   file2<<"---------------start---------------"<<endl;
   file2<<"DistributeCl cluster"<<C.returnName()<<endl;
   file2<<"Contents of F:"<<endl;
   F.output(file2);

   v1Name=C.returnCore().returnName();
   if(v1Name>0&&v1Name<=singleVertex) // distributing single vertex
   {
      file2<<"C is a singleton vertex"<<endl;
      //out<<"in distributeCl 1"<<endl;
      v1=C.returnCore();
      newTree=distributeVert(v1, F, graph0, file1, file2);
      //newTree->output(out);
      //out<<"in distributeCl 11"<<endl;
      if(newTree->empty())
          delete newTree;
      else
      {
          file2<<"Found new Cluster:"<<endl;
          newTree->output(file2);
          file2<<"----------------end----------------"<<endl;
          return *newTree;
      }
   }
   else  // distributing a cluster
   {
       file2<<"C is a cluster"<<endl;
       //out<<"in distributeCl 2"<<endl;
       //if(C.returnCore().returnName()==19) { C.output(out);}
       ClusterDepth=C.returnCore().returnDepth();
       if(F.returnDepth()<ClusterDepth) F.setDepth(ClusterDepth);
       k=pushOutside(C, F, graph0, file1, file2);
       if(k>0)
       {
          newTree=getCluster(F, graph0);
          file2<<"Found new Cluster:"<<endl;
          newTree->output(file2);
          file2<<"----------------end----------------"<<endl;
          return *newTree;
       }
   }

   file2<<"no Cluster found, returning empty cluster"<<endl;
   file2<<"----------------end----------------"<<endl;
   Cluster *dummy=new Cluster;
   return *dummy;
}

