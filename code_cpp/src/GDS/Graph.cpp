#include "Graph.hpp"

#include <fstream>

#include "defs.h"
#include "Random.hpp"



void Graph::simplify()
{
   int i, j, tempNum, e1Name, e2Name, v11, v12, v21, v22;
   Edge edg1, edg2;
   Vertex *vert1, *vert2;

   tempNum=NumEdge;
   for(i=1;i<=tempNum;i++)
   {
      edg1=edges.pop();
      e1Name=edg1.returnName();
      v11=edg1.returnEnd1();
      v12=edg1.returnEnd2();
      if(edg1.returnWeight()==0)
      {
          vert1=VertAddr(e1Name);
          vert2=VertAddr(e1Name);
          vert1->delIncid(e1Name);
          vert2->delIncid(e1Name);
          NumEdge--;
      }
      else
      {
         for(j=1;j<tempNum;j++)
         {
            edg2=edges.pop();
            e2Name=edg2.returnName();
            if(e2Name!=0)
            {
               v21=edg2.returnEnd1();
               v22=edg2.returnEnd2();
               if((v11==v21&&v12==v22)||(v11==v22&&v12==v21))
               {
                  vert1=VertAddr(v21);
                  vert2=VertAddr(v22);
                  vert1->delIncid(e2Name);
                  vert2->delIncid(e2Name);
                  edg1.incrsWt(edg2.returnWeight());
                  NumEdge--;
                }
                else
                {
                   edges.append(edg2);
                }
            }//end of if(e2Name!=0)
         } //end of for(j=0;j<NumEdge;i++)
         edges.append(edg1);
      } //end of else
   }
}

void Graph::freeze()
{
   int i, len;
   Vertex *verP;

   len=vertices.returnLen();
   for(i=1;i<=len;i++)
   {
      verP=vertices.addrByIndex(i);
      verP->freeze();
   }
}



void Graph::defrostGraph()
{
   int i, len;
   Vertex *verP;

   len=vertices.returnLen();
   for(i=1;i<=len;i++)
   {
      //out<<"defrosting ver="<<vertices.retrieve(i)<<std::endl;
      verP=vertices.addrByIndex(i);
      verP->defrost();
   }
}

void Graph::freezeVertForever(int vName)
{
   int i, len;
   Vertex *verP;

   len=vertices.returnLen();
   for(i=1;i<=len;i++)
   {
      verP=vertices.addrByIndex(i);
      if(verP->returnName()==vName)
      {
         verP->freezeForever();
         return;
      }
   }
}

void Graph::defrostOneVert(int vName)
{
   int i, len;
   Vertex *verP;

   len=vertices.returnLen();
   for(i=1;i<=len;i++)
   {
      verP=vertices.addrByIndex(i);
      if(verP->returnName()==vName)
      {
         verP->defrost();
         return;
      }
   }
}

void Graph::delVerByName(int vName)
{
   int i;
   for(i=1;i<=NumVert;i++)
   {
      if(vertices.retrieve(i).returnName()==vName)
      {
         vertices.deleteIndex(i);
         NumVert--;
      }
   }
}

void Graph::delEdgeByName(int eName)
{
   int i;
   for(i=1;i<=NumEdge;i++)
   {
      if(edges.retrieve(i).returnName()==eName)
      {
        edges.deleteIndex(i);
        NumEdge--;
      }
   }
}

int Graph::numLabeled()
{
   int i, labeled;
   labeled=0;
   for(i=1;i<=NumVert;i++)
   {
      if(vertices.retrieve(i).returnLabel()!=0) labeled++;
   }
   return labeled;
}

Edge & Graph::returnEdgeByEnds(int v1, int v2)
{
   int i, v01, v02;
   for(i=0;i<NumEdge;i++)
   {
      v01=edges.retrieve(i+1).returnEnd1();
      v02=edges.retrieve(i+1).returnEnd2();
      if((v1==v01&&v2==v02)||(v1==v02&&v2==v01))
          return edges.retrieve(i+1);
   }
   Edge *dummy=new Edge;
   return *dummy;
}

bool Graph::hasEdge(int eName)
{
   int i;
   for(i=0;i<NumEdge;i++)
      if(eName==edges.retrieve(i+1).returnName()) return true;
   return false;
}

bool Graph::hasVert(int vName)
{
   int i;
   for(i=0;i<NumVert;i++)
      if(vName==vertices.retrieve(i+1).returnName()) return true;
   return false;
}

Edge & Graph::returnEdgeByName(int eName)
{
   int i;
   for(i=0;i<NumEdge;i++)
      if(eName==edges.retrieve(i+1).returnName())
          return (edges.retrieve(i+1));
   Edge *dummy;
   return *dummy;
}

Vertex & Graph::returnVertByName(int vName)
{
 int i;
 for(i=0;i<NumVert;i++)
  if(vName==vertices.retrieve(i+1).returnName())
     return (vertices.retrieve(i+1));
 Vertex *dummy;
 return *dummy;
}

Vertex * Graph::VertAddr(int vName)
{
int i;
for(i=1;i<=NumVert;i++)
 if(vName==vertices.retrieve(i).returnName()) return vertices.addrByIndex(i);
return NULL;
}

Edge * Graph::EdgeAddr(int eName)
{
 int i;
for(i=1;i<=NumEdge;i++)
 if(eName==edges.retrieve(i).returnName()) return edges.addrByIndex(i);
return NULL;
}

void Graph::augment(Vertex *v3p, Edge *edgep)
{
  int wt;
  Vertex *vp;
  Edge *ep;

  wt=min(v3p->returnPosFlow(), v3p->returnWeight() - v3p->returnEstFlow());
  edgep->incrsPosFlow(-wt);
  vp=v3p;
  ep=EdgeAddr(vp->returnPredEdg());

  while (ep!=NULL)
  {
     if(ep->returnEnd1()==vp->returnName())
     {
        ep->incrsFlow1(wt);
        vp->incrsEstFlow(wt);
     }
     if (ep->returnEnd2()==vp->returnName())
     {
        ep->incrsFlow2(wt);
        vp->incrsEstFlow(wt);
     }

     if (ep->returnPredVer() == 0)
     {
       ep=NULL;
     }
     else
     {
         vp=VertAddr(ep->returnPredVer());
         if (ep->returnEnd1()==vp->returnName())
         {
             ep->incrsFlow1(-wt);
             vp->incrsEstFlow(-wt);
         }
         if (ep->returnEnd2()==vp->returnName())
         {
             ep->incrsFlow2(-wt);
             vp->incrsEstFlow(-wt);
         }
         ep=EdgeAddr(vp->returnPredEdg());
     }
  }
}

void Graph::removeLabels()
{
  int i;
  Vertex *v;
  Edge *e;

  for(i=1;i<=NumVert;i++)
  {
     v=vertices.addrByIndex(i);
     v->setLabel(0);
     v->setScan(0);
  }

  for(i=1;i<=NumEdge;i++)
  {
     e=edges.addrByIndex(i);
     e->setLabel(0);
     e->setScan(0);
  }
}

void Graph::restoreFlow(Edge *edgep)
{
  Vertex *end1, *end2;
  end1=VertAddr(edgep->returnEnd1());
  end2=VertAddr(edgep->returnEnd2());

  end1->incrsEstFlow(-(edgep->returnFlow1()));
  end2->incrsEstFlow(-(edgep->returnFlow2()));

  edgep->setFlow1(0);
  edgep->setFlow2(0);
}

int Graph::constrainDegree()
{
   int i, constrain;
   Vertex vert;
   Edge edge;

   if(dimension==2) constrain=4;
   if(dimension==3) constrain=7;

   for(i=1;i<=NumVert;i++)
   {
      vert=vertices.retrieve(i);
      if(vert.returnLabel()!=0)
         constrain=constrain-vert.returnWeight();
   }

   for(i=1;i<=NumEdge;i++)
   {
      edge=edges.retrieve(i);
      if(edge.returnLabel()!=0)
         constrain=constrain+edge.returnWeight();
   }
   return constrain;
}

//distribut Edge
int Graph::distribute0(Edge &edge, std::ostream &file2)
{
  int no_of_unscanned=1;
  Edge *edgep, *ep;
  Vertex *v1p, *v2p, *v3p;
  int i, j;


  removeLabels();
  file2<<"---------------start---------------"<<std::endl;
  file2<<"Distribute0 for edge"<<edge.returnName()<<std::endl;
  file2<<"Contents of F: "<<std::endl;
  //output(file2);
  edges.append(edge); //add edge in F
  file2<<"Contents of F after appending edge"<<std::endl;
  NumEdge++;
  output(file2);
  edgep=EdgeAddr(edge.returnName());
  edgep->setLabel(1);
  edgep->setScan(0);
  edgep->setPosFlow(edgep->returnWeight());
  edgep->setPredVer(0);

  file2<<"Distributing "<<*edgep<<std::endl;

  while(no_of_unscanned>0 && edgep->returnPosFlow()>0)
  {
      no_of_unscanned=0;
      for (j=1;j<=NumEdge;j++)  // for every labeled edge in F
      {
         ep=edges.addrByIndex(j);
         if(ep->returnLabel()==1 && ep->returnScan()==0)
         {
        file2<<"Labeling endpoints of edge "<<ep->returnName()<<std::endl;

            v1p=VertAddr(ep->returnEnd1());
            v2p=VertAddr(ep->returnEnd2());
            if(v1p->returnLabel()==0)
            {
           file2<<"Labeled Vert  "<<v1p->returnName()<<std::endl;
               v1p->setLabel(1);
               v1p->setPosFlow(ep->returnPosFlow());
               v1p->setPredEdg(ep->returnName());
               no_of_unscanned++;
            }
            if(v2p->returnLabel()==0)
            {

           file2<<"Labeled Vert  "<<v2p->returnName()<<std::endl;
               v2p->setLabel(1);
               v2p->setPosFlow(ep->returnPosFlow());
               v2p->setPredEdg(ep->returnName());
               no_of_unscanned++;
            }
            ep->setScan(1);
         }
      }

      for (i=1;i<=NumVert;i++)  // for every labeled vertices in F
      {
         v3p=vertices.addrByIndex(i);
         if (v3p->returnLabel()==1 && v3p->returnScan()==0)
         {
            if(v3p->returnEstFlow() < v3p->returnWeight() )
            {
               augment(v3p, edgep);
               file2<<"edgep PosFlow="<<edgep->returnPosFlow()<<std::endl;
               file2 << "****after augment, F is:**** " << std::endl;
               output(file2);
               removeLabels();

               if (edgep->returnPosFlow()>0)
                  edgep->setLabel(1);
            }
            else
            {
               for(j=1;j<=NumEdge;j++)//for every non-label edge,incid to v3
               {
                 ep=edges.addrByIndex(j);
                 if(ep->returnLabel()==0 && ep->returnFlow1()>0
                     && v3p->returnName()==ep->returnEnd1() )
                 {
                   ep->setLabel(1);
                   ep->setPosFlow(min(v3p->returnPosFlow(),ep->returnFlow1()));
                   file2<<"j="<<j<<" edgep PosFlow="<<edgep->returnPosFlow()<<std::endl;
                   ep->setPredVer(v3p->returnName());
                 }
                 if(ep->returnLabel()==0 && ep->returnFlow2()>0
                     && v3p->returnName()==ep->returnEnd2() )
                 {
                   ep->setLabel(1);
                   ep->setPosFlow(min(v3p->returnPosFlow(),ep->returnFlow2()));
                   file2<<"j="<<j<<" edgep PosFlow="<<edgep->returnPosFlow()<<std::endl;
                   ep->setPredVer(v3p->returnName());
                 }
               }
               v3p->setScan(1);
               no_of_unscanned++;
            }
         }
      }
  }

  if(edgep->returnPosFlow()<=0)
  {
    file2<<"Ending Contents of F:"<<std::endl;
    output(file2);
    file2<<"Distribute0 returns 0"<<std::endl;
    file2<<"----------------end----------------"<<std::endl;
    return 0;
  }
  else
  {
    restoreFlow(edgep);
    file2<<"Ending Contents of F:"<<std::endl;
    output(file2);
    file2<<"Distribute0 returns 1"<<std::endl;
    file2<<"----------------end----------------"<<std::endl;
    return 1; // not able to distribute the edge completely
  }
}


int Graph::sketchInput(int &idx, jint *inputData, int &idxDbl, jdouble *dbleData) //from sketch to graph
{

   int i, j, shapeType, constraintType, numInvolved, eName, atPart[2];
   Vertex *vert;
   Edge new_edg;

   //part 1 shape info.
   //NumVert=0;  //# of shapes in sketch == # of vertices in graph
   //idx=1;
   //idxDbl=0;

   std::ofstream outf;
   outf.open("sketchInput.out");

   output(outf);

   while(inputData[idx] >= 0)
   {
      NumVert++;
      Vertex new_ver;
      shapeType=inputData[idx++];            // shapeType
      outf<<"shapeType="<<shapeType<<std::endl;

      new_ver.setType(shapeType);
//      outf<<"singleVertex="<<singleVertex<<std::endl;
      if(singleVertex<inputData[idx])
         singleVertex=inputData[idx];
      outf<<"singleVertex="<<singleVertex<<std::endl;
      outf<<"shapeID="<<inputData[idx]<<std::endl;

      switch(shapeType)                      // get weigth based on shapeType
      {
         case 0:                             // point
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
           new_ver.setWeight(2);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
               new_ver.notDefinedValue(2);   // Not Applicable
               new_ver.notDefinedValue(3);   // Not Applicable
               new_ver.notDefinedValue(4);   // Not Applicable
               new_ver.notDefinedValue(5);   // Not Applicable
           break;
         case 1:                             // line
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(2);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
               new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
               new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
               idxDbl++;
               new_ver.notDefinedValue(5);   // Not Applicable
               break;
         case 2:                             // ray
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(3);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
               new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
               new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
               idxDbl++;
               new_ver.notDefinedValue(5);   // Not Applicable
               break;
         case 3:                             // line segment
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(4);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
               new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
               new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
               idxDbl++;

/*               if(inputData[idx]>=0) {
                  new_ver.initialValue(5, dbleData[idxDbl++], -1); //length
                  new_ver.incrsWt(-1);
               }*/
               break;
         case 4:                             // circle
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(3);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
/*             if(inputData[idx]>=0) {
                  new_ver.initialValue(2, dbleData[idxDbl++], -1); // radius
                  new_ver.incrsWt(-1);
               } */
               idxDbl++;
               new_ver.notDefinedValue(3);   // Not Applicable
               new_ver.notDefinedValue(4);   // Not Applicable
               new_ver.notDefinedValue(5);   // Not Applicable
               break;
         case 5:                             // arc
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(5);
               new_ver.initialValue(0, (float)inputData[idx++], 0); //x
               new_ver.initialValue(1, (float)inputData[idx++], 0); //y
               if(inputData[idx]>=0) {
                  new_ver.initialValue(2, dbleData[idxDbl++], -1); // radius
                  new_ver.incrsWt(-1);
               }
               idx=idx+2;  // float is 64-bit
               if(inputData[idx]>=0) {
                  new_ver.initialValue(3, dbleData[idxDbl++], -1); // angle
                  new_ver.incrsWt(-1);
               }
               break;
     case 6:                //point3D
           outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(3);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
               new_ver.initialValue(2, dbleData[idxDbl++], 0); //z
               new_ver.notDefinedValue(3);   // Not Applicable
               new_ver.notDefinedValue(4);   // Not Applicable
               new_ver.notDefinedValue(5);   // Not Applicable
               break;
         case 7:                            //point3D
               outf<<"shapeID="<<inputData[idx]<<std::endl;
               new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
               new_ver.setWeight(6);
               new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
               new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
               new_ver.initialValue(2, dbleData[idxDbl++], 0); //z
               new_ver.initialValue(3, dbleData[idxDbl++], 0); //c
               new_ver.initialValue(4, dbleData[idxDbl++], 0); //d
               new_ver.initialValue(5, dbleData[idxDbl++], 0); //e
               break;

      };
      //if(inputData[idx]!=-1) idx++;
      vertices.append(new_ver);
   }
   outf<< "Vertices Number is: " << NumVert<<std::endl;
   outf<<inputData[idx]<<std::endl;
   outf << "Start to do part 2" << std::endl;
   idx++;

   int highEdgeCount=0, temp;

   highEdgeCount=0;

   //part 2 constraint info.
   while(inputData[idx]>=0)
   {
      NumEdge++;
      constraintType=inputData[idx++];      // constraint type
      outf<<"constraintType="<<constraintType<<std::endl;


      if(constraintType==7)
      {//imaginary object???
        idx++;
        idx++;

        int name1, name2, name3;

           int temp;

        name1=inputData[idx++];
    idx++;

        name2=inputData[idx++];
    idx++;

        name3=inputData[idx++];
    idx++;

        //Check whether the imaginary object exists.
        Vertex temp1, temp2, currVert;
        Edge currEdge1, currEdge2;
        for(i=1;i<=NumVert;i++)
        {
            currVert=vertices.retrieve(i);
            if (currVert.returnType()==7)   //imaginary object
        {
                currEdge1=returnEdgeByEnds(name1, currVert.returnName());
                currEdge2=returnEdgeByEnds(name2, currVert.returnName());
                //this imaginary object exists
                if (currEdge1.returnName() && currEdge1.returnType()==1 && currEdge1.returnWeight()==3
            && currEdge2.returnName() && currEdge2.returnType()==1 && currEdge2.returnWeight()==3)
            temp1 = currVert;

                currEdge1=returnEdgeByEnds(name3, currVert.returnName());
                currEdge2=returnEdgeByEnds(name2, currVert.returnName());
                //this imaginary object exists
                if (currEdge1.returnName() && currEdge1.returnType()==1 && currEdge1.returnWeight()==3
            && currEdge2.returnName() && currEdge2.returnType()==1 && currEdge2.returnWeight()==3)
            temp2 = currVert;
            }
        }

    if (hasVert(temp1.returnName()))
        delVerByName(temp1.returnName());
        else
        {
            temp1.setType(7);
            temp1.setName(++singleVertex);
            temp1.setWeight(6);
            temp1.setIgnore(true);
        }

    if (hasVert(temp2.returnName()))
        delVerByName(temp2.returnName());
        else
        {
            temp2.setType(7);
            temp2.setName(++singleVertex);
            temp2.setWeight(6);
            temp2.setIgnore(true);
        }

        outf<<"name1 = "<<name1<<";   name2 = "<<name2<<";   name3 = "<<name3<<std::endl;
        Edge tempE1, tempE2, tempE3, tempE4, tempE5;
        int tempName;

        tempE1.setName(1000+highEdgeCount++);
        tempE1.setEnd(0, temp1.returnName());
        tempE1.setEnd(1, temp2.returnName());
    tempE1.setType(8);
    tempE1.setWeight(1);
        tempE1.setValue(dbleData[idxDbl++]);

    temp1.appendIncid(tempE1.returnName());
        temp2.appendIncid(tempE1.returnName());

        currEdge1 = returnEdgeByEnds(name1, temp1.returnName());
        if (currEdge1.returnName())
        {
        tempE2 = currEdge1;
            delEdgeByName(currEdge1.returnName());
        }
    else
    {
            tempE2.setName(1000+highEdgeCount++);
            tempName=tempE2.returnName();
            tempE2.setEnd(0, name1);
            tempE2.setPart(0, 1);
            tempE2.setEnd(1, temp1.returnName());
            tempE2.setPart(1, 1);
            tempE2.setType(1);
            tempE2.setWeight(3);
            temp1.appendIncid(tempName);
            for(j=1;j<=NumVert;j++)
            {
                vert=vertices.addrByIndex(j);
                if(vert->returnName()==name1)
                    vert->appendIncid(tempName);
            }
        }

        currEdge1 = returnEdgeByEnds(name2, temp1.returnName());
        if (currEdge1.returnName())
        {
        tempE3 = currEdge1;
            delEdgeByName(currEdge1.returnName());
        }
    else
    {
            tempE3.setName(1000+highEdgeCount++);
            tempName=tempE3.returnName();
            tempE3.setEnd(0, name2);
            tempE3.setPart(0, 1);
            tempE3.setEnd(1, temp1.returnName());
            tempE3.setPart(1, 2);
            tempE3.setType(1);
            tempE3.setWeight(3);
            temp1.appendIncid(tempName);
            for(j=1;j<=NumVert;j++)
            {
                vert=vertices.addrByIndex(j);
                if(vert->returnName()==name2)
                    vert->appendIncid(tempName);
            }
        }

        currEdge1 = returnEdgeByEnds(name2, temp2.returnName());
        if (currEdge1.returnName())
        {
        tempE4 = currEdge1;
            delEdgeByName(currEdge1.returnName());
        }
        else
    {
            tempE4.setName(1000+highEdgeCount++);
            tempName=tempE4.returnName();
            tempE4.setEnd(0, name2);
            tempE4.setPart(0, 1);
            tempE4.setEnd(1, temp2.returnName());
            tempE4.setPart(1, 2);
            tempE4.setType(1);
            tempE4.setWeight(3);
            temp2.appendIncid(tempName);
            for(j=1;j<=NumVert;j++)
            {
                vert=vertices.addrByIndex(j);
                if(vert->returnName()==name2)
                     vert->appendIncid(tempName);
            }
    }

        currEdge1 = returnEdgeByEnds(name3, temp2.returnName());
        if (currEdge1.returnName())
        {
        tempE5 = currEdge1;
            delEdgeByName(currEdge1.returnName());
        }
        else
    {
            tempE5.setName(1000+highEdgeCount++);
            tempName=tempE5.returnName();
            tempE5.setEnd(0, name3);
            tempE5.setPart(0, 1);
            tempE5.setEnd(1, temp2.returnName());
            tempE5.setPart(1, 1);
            tempE5.setType(1);
            tempE5.setWeight(3);
            temp2.appendIncid(tempName);
            for(j=1;j<=NumVert;j++)
            {
                 vert=vertices.addrByIndex(j);
                 if(vert->returnName()==name3)
                     vert->appendIncid(tempName);
            }
    }

        vertices.append(temp1);
        vertices.append(temp2);
        edges.append(tempE1);
        edges.append(tempE2);
        edges.append(tempE3);
        edges.append(tempE4);
        edges.append(tempE5);
        NumVert+=2;
    NumEdge+=4;
        output(outf);
        continue;
      }//if (constraintType = 7)


      new_edg.setType(constraintType);
      new_edg.setWeight(1);                 // weight = 1 for most constraints
      eName=inputData[idx++];               // constraint ID  == edge name
      outf<<"constraintID="<<eName<<std::endl;

      if(nextEdgeName<=eName)
         nextEdgeName=eName+1;
      new_edg.setName(eName);
      numInvolved=inputData[idx++];         // # of Shapes involved==2 for now
      for(i=0;i<numInvolved;i++)
      {
     int endID;

         //out<<"   involved="<<inputData[idx]<<std::endl;
     endID=inputData[idx];
         new_edg.setEnd(i, endID);
         for(j=1;j<=NumVert;j++)
         {
            vert=vertices.addrByIndex(j);
            if(vert->returnName()==inputData[idx])
               vert->appendIncid(eName);
         }
         idx++;

     int temp, vertType;

         if(constraintType==0||constraintType==1||constraintType==4||constraintType==6) {//dista
            atPart[i]=inputData[idx];
            //out<<"   part="<<inputData[idx]<<std::endl;
        temp=inputData[idx++];

            outf<<"Constraint Type="<<constraintType<<" part="<<temp<<std::endl;

        vertType=returnVertByName(endID).returnType();

        if(vertType==4 && temp==1)
        temp=3;

            new_edg.setPart(i, temp);
         }
         else
            new_edg.setPart(i, 0);
      }//     for(i=0;i<numInvolved;i++)

      if(constraintType==1) { //incidence
         if(atPart[0]==0&&atPart[1]==0)
            new_edg.setWeight(0);
         if(atPart[0]>0&&atPart[0]<4&&atPart[1]==0)
            new_edg.setWeight(1);
         if(atPart[1]>0&&atPart[1]<4&&atPart[0]==0)
            new_edg.setWeight(1);
         if(atPart[0]!=0&&atPart[1]!=0)
            new_edg.setWeight(2);
      }

      if(constraintType==8) new_edg.setWeight(2);

      if(constraintType==0||constraintType==4||constraintType==6||constraintType==8)
      { // distance (2 or 3D) or angle
         new_edg.setValue(dbleData[idxDbl++]);//-1.0 for arbitrary
         //new_edg.setValue(toFloat(inputData[idx]));//-1.0 for arbitrary
         //idx=idx+2;
      }
      else
         new_edg.setValue(-2.0);                 // N/A for other constraints
      edges.append(new_edg);
   }//while(inputData[idx]>=0)
   idx++;
   //out<<"in input, idx="<<idx<<" inputData[idx]="<<inputData[idx]<<std::endl;
   nextVerName=singleVertex+4;
   if(vertices.retrieve(1).returnType()>5)
    dimension=3;
   else
    dimension=2;

   output(outf);
   outf.close();
   return idx;

}

void Graph::randomGraph()
{
   int i, j, Vweight, Eweight;
   float propability;
   Vertex old_ver;
   Random random(11);

   std::cout << "creating a random graph\n";
   std::cout << "input number of vertices: ";
   std::cin  >> NumVert;
   std::cout << "edge existing propability between two vertices:";
   std::cin >> propability;

   for(i=1;i<=NumVert;i++)
   {
      Vertex new_ver;
      new_ver.setName(i);
      Vweight=2+ (int) (3*random.ran0());
      new_ver.setWeight(Vweight);
      vertices.append(new_ver);
   }

   NumEdge=0;
   for(i=1;i<=NumVert;i++)
   {
      for(j=i+1;j<=NumVert;j++)
      {
          if(random.ran0()+propability>=1.0)
          {
             Edge new_edg;
             NumEdge++;
             new_edg.setName(NumEdge);
             Eweight=1+ (int) (2*random.ran0());
             new_edg.setWeight(Eweight);
             new_edg.setEnd(0, i);
             new_edg.setEnd(1, j);
             edges.append(new_edg);

             old_ver=vertices.retrieve(i);
             old_ver.appendIncid(NumEdge);
             vertices.changeElem(i, old_ver); // Vname==position

             old_ver=vertices.retrieve(j);
             old_ver.appendIncid(NumEdge);
             vertices.changeElem(j, old_ver); // Vname==position
          }
      }
   }
   std::cout<<"NumEdge="<<NumEdge<<std::endl;
}


void Graph::output(std::ostream& os)
{
   int i;
   Vertex currVert;
   Edge currEdge;
   int totalVertW, totalEdgeW, totalFlow;
   int *vertFlow;

   totalFlow=totalVertW=totalEdgeW=0;

   if(NumVert==0 && NumEdge==0)
   {
      os<<"Graph is Empty."<<std::endl;
      return;
   }

     vertFlow = new int[100];
     for(i=0;i<100;i++)
       vertFlow[i]=0;

     for(i=1;i<=NumEdge;i++)
     {
     currEdge=edges.retrieve(i);
      os<<currEdge<<std::endl;
        totalEdgeW+=currEdge.returnWeight();
        totalFlow+=currEdge.returnFlow1();
        totalFlow+=currEdge.returnFlow2();
        vertFlow[currEdge.returnEnd1()]+=currEdge.returnFlow1();
        vertFlow[currEdge.returnEnd2()]+=currEdge.returnFlow2();
     }

     for(i=1;i<=NumVert;i++)
     {
      currVert=vertices.retrieve(i);
      os<<currVert<<std::endl;
      totalVertW+=currVert.returnWeight();
      if(vertFlow[currVert.returnName()]!=currVert.returnEstFlow())
    os<<"Flows do not match for vertex "<<currVert.returnName()<<": Curr Flow="
      <<vertFlow[currVert.returnName()]<<" Est Flow="<<currVert.returnEstFlow()<<std::endl;
     }

     os<<"Total weight of vertices: "<<totalVertW<<std::endl;
     os<<"Total weight of edges   : "<<totalEdgeW<<std::endl;
     os<<"Total flowed weight     : "<<totalFlow<<std::endl;

}
