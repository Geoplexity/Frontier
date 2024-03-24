//file: mfa5GetC.cpp
/*contains two methods: copying  graph and sequential extensions for
  the DR-planner*/
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


#ifndef MFA_6_GETC
#define MFA_6_GETC

#include "mfa2Clas.h"

namespace ffnx::mfa {

    // copy a graph and return K value
    int copyG(Graph &g0, Graph &g1)
    {
        int i, nVer, nEdg, dimen;

        g1.delAllVer();
        g1.delAllEdg();

        dimen=g0.returnDimen();
        g1.setDimen(dimen);

        nVer=g0.returnNumVer();
        nEdg=g0.returnNumEdg();

        for(i=1;i<=nVer;i++)
            g1.appendVertex(g0.returnVertByIndex(i));

        for(i=1;i<=nEdg;i++)
            g1.appendEdge(g0.returnEdgeByIndex(i));

        if(dimen==3) return 7;  //K=7 for 3-D problem
        else return 4;          //K=4 for 2-D problem
    }


//performs a sequential extension on the labeled section of F, sequential extensions are described in the
//attached paper documentation
    int extension(Graph &F, Graph &graph0, ostream &file1, ostream &file2, StaticVariables& staticVariables)
    {
        Vertex v0, v1;
        Edge edge;
        List<Edge> newEdges, oldEdges;  // put new edges F when extend a vertex
        int j, nVertg0, nVertF, nLabelF, possible, extEdgeNum;
        int v0Name, v1Name;
        int eWeight, newEdgeLen, oldEdgeLen, newLabel=-2;

        nLabelF=F.numLabeled();
        if(nLabelF==0) return 0;
        file2<<"In the beginning of extension F"<<endl;
        F.output(file2);

        for(v0Name=1;v0Name < staticVariables.nextVerName;v0Name++) // v0--new vertex to be extended
        {
            possible=0;
            if(F.hasVert(v0Name))
            {
                v0=F.returnVertByName(v0Name);
                if(v0.returnLabel()==0&&(!v0.isFrozen())) possible=1; //v0 in F, but NOT in new cluster
            }
            else if(v0Name <= staticVariables.singleVertex&&graph0.hasVert(v0Name))
            {
                v0=graph0.returnVertByName(v0Name); //v0 not in F but in graph0
                if(F.returnDepth()==0&&v0.returnName()!=0&&(!v0.isFrozen())) possible=1;
            }

            if(possible==1)  // v0 is possible to be extended
            {
                eWeight=0;
                extEdgeNum=0;         // new extended edges for new cluster
                newEdges.makeEmpty(); // new edges for F
                oldEdges.makeEmpty();

                //file2<<"possible v0="<<v0<<endl;

                nVertF=F.returnNumVer();
                for(j=1;j<=nVertF;j++)           // for every vertex v1 in F
                {
                    v1=F.returnVertByIndex(j);
                    v1Name=v1.returnName();
                    edge=F.returnEdgeByEnds(v0Name,v1Name);
                    edge.setScan(0);
                    if(edge.returnName()==0) // F dose not have this edge
                    {
                        edge=graph0.returnEdgeByEnds(v0Name,v1Name);
                        if(v1.returnLabel()!=0) edge.setScan(1);//connected to new clust
                        if(edge.returnName()!=0) newEdges.append(edge);
                    }
                    else
                    {
                        if(v1.returnLabel()!=0) edge.setScan(1);//connected to new clustr
                        oldEdges.append(edge);
                    }

                    if(v1.returnLabel()!=0)       // v1 belongs to new cluster
                    {
                        if(edge.returnWeight()!=0)
                        {
                            extEdgeNum++;
                            eWeight=eWeight+edge.returnWeight();
                        }
                    }
                }

                if(eWeight>=v0.returnWeight()&&extEdgeNum>=F.returnDimen()) //exists
                {
                    if(eWeight>v0.returnWeight())
                        v0.setLabel(3);
                    else if(nLabelF>F.returnDimen())
                        v0.setLabel(2);
                    else
                        v0.setLabel(1);

                    if(F.hasVert(v0Name))
                    {
                        F.delVerByName(v0Name);
                    }
                    newEdgeLen=newEdges.returnLen();
                    for(j=1;j<=newEdgeLen;j++)
                    {
                        edge=newEdges.pop();
                        if(edge.returnScan()==1) //edge is connected to new cluster
                        {
                            edge.setLabel(1);
                        }
                        F.appendEdge(edge);
                    }
                    oldEdgeLen=oldEdges.returnLen();
                    for(j=1;j<=oldEdgeLen;j++)
                    {
                        edge=oldEdges.pop();
                        if(edge.returnScan()==1) //edge is connected to new cluster
                        {
                            F.delEdgeByName(edge.returnName());
                            edge.setLabel(1);
                            F.appendEdge(edge);
                        }
                    }
                    F.appendVertex(v0);  // extended vertex is added to F
                    file1<<"Extend Vertice "<<v0Name<<endl;
                    file2<<"new extended v0="<<v0Name<<" current F is "<<endl;
                    F.output(file2);
                    return 1;
                    //v0Name=0;   // try one more cycle
                }
            } //end of if(possible==1)
        }
        return 0;
    }

//After distribute has been run, Graph F contains vertices labeled for the creation of a new cluster
//this method isolates those vertices, performs any reductions to the interior vertices and edges
//as necessary, and creates a new Cluster object, returning a pointer to it
//for more on the specifics of this reduction process, see attached paper documentation
    Cluster * getCluster(Graph &F, Graph &graph0, StaticVariables& staticVariables)
    {
        Vertex vOld, vOther, vNew, v1, v2, v3, core;
        Edge e1Old, eOld, eNew;
        List<Vertex> innerVers;
        List<Edge> innerE, outerE;
        List<int> frontiers;
        List<int> originalV;
        //List<int> originalE;
        int depth=0, oldDepth, constrain=0;
        Cluster *new_cluster=new Cluster;

        int i, j, k, l, nVerF, nFront=0, nIncid, nVerInner=0;
        int ItIsFront, e1Name, coreWeight, eWeight;
        int vOldName, v1Name, v2Name, v3Name, vOtherName;
        int v1OutF;

        frontiers.makeEmpty();
        //out<<"before getCluster"<<endl;
        //F.output(out);

        //labeled vertex in F is classified as frontier or not
        nVerF=F.returnNumVer();
        for(i=1;i<=nVerF;i++) // for every vertex in F
        {
            vOld=F.returnVertByIndex(i);
            if(vOld.returnLabel()!=0)  // make sure vOld in new cluster
            {
                if(vOld.returnLabel()-1>constrain) constrain=vOld.returnLabel()-1;
                oldDepth=vOld.returnDepth();
                if(depth<oldDepth) depth=oldDepth;
                vOldName=vOld.returnName();
                //prevClst.append(vOldName); //previous found clusters
                originalV.append(vOldName);
                //out<<"after append originalV"<<endl;
                //F.output(out);
                //out<<"originalV is"<<endl;
                //originalV.output(out);

                ItIsFront=0;
                nIncid=vOld.numIncid();
                for(j=1;j<=nIncid;j++)
                {
                    e1Name=vOld.returnIncid(j);
                    if(F.hasEdge(e1Name))
                    {
                        e1Old=F.returnEdgeByName(e1Name);
                        vOtherName=e1Old.otherEnd(vOld.returnName());
                        vOther=F.returnVertByName(vOtherName);
                        if(vOther.returnLabel()==0) ItIsFront=1;//other not in cluster
                    }
                    else
                        ItIsFront=1;  // edge not in F
                }
                if(ItIsFront)
                {
                    //prevClst.append(vOldName);
                    frontiers.append(vOldName);
                    //out<<"vOld="<<vOld<<endl;
                    nFront++;
                }
                else
                {
                    vOld.setScan(-1);
                    innerVers.append(vOld);
                    nVerInner++;
                }
            }
        }
        //out<<"after classify"<<endl;

        // set scan=-1 for all inner vertices
        for(i=1;i<=nVerInner;i++)
        {
            v2=innerVers.retrieve(i);
            F.delVer(v2);
            v2.setScan(-1);
            F.appendVertex(v2);
            //prevClst.append(v2.returnName());
        }

        /***
        out<<"frontiers"<<endl;
        for(i=1;i<=frontiers.returnLen();i++)
           out<<frontiers.retrieve(i)<<endl;
        out<<"innerVers"<<endl;
        for(i=1;i<=innerVers.returnLen();i++)
           out<<innerVers.retrieve(i)<<endl;
        ***/

        //form inner and get outer edges
        coreWeight=0;
        for(i=1;i<=nFront;i++)
        {
            v1OutF=0; //part of v1's flow, absorbed from frontier & outer edges
            v1Name=frontiers.retrieve(i);
            v1=F.returnVertByName(v1Name);
            coreWeight=coreWeight-v1.returnWeight();

            //get outer edges
            eWeight=0;
            for(j=1;j<nVerF;j++)
            {
                v3=F.returnVertByIndex(j);
                v3Name=v3.returnName();
                eOld=F.returnEdgeByEnds(v1Name,v3Name);
                if(v3.returnScan()!=-1&&eOld.returnName())//v3 is not an inner vertex
                {
                    if(v1Name==eOld.returnEnd1()) v1OutF=v1OutF+eOld.returnFlow1();
                    if(v1Name==eOld.returnEnd2()) v1OutF=v1OutF+eOld.returnFlow2();

                    if(v3.returnLabel()!=0&&v1Name!=v3Name&&!outerE.hasElem(eOld))
                    {
                        outerE.append(eOld);   // edge between frontiers
                        eWeight=eWeight+eOld.returnWeight(); //claculate outer weight
                    }
                }
            }
            coreWeight=coreWeight+eWeight;  // add outer edge weight

            //form inner edges
            eWeight=0;
            for(j=1;j<=nVerInner;j++)
            {
                v2=innerVers.retrieve(j);  // v2 is inner vertex
                v2Name=v2.returnName();
                eOld=F.returnEdgeByEnds(v1Name,v2Name);//v1,v2 form old innerEdge
                eWeight=eWeight+eOld.returnWeight(); //claculate inner weight
                v1.delIncid(eOld.returnName()); // delete an incident edge
            }
            //out<<"v1="<<v1<<endl;
            eNew.setName(staticVariables.nextEdgeName);
            if(eWeight!=0)
            {
                eNew.setWeight(eWeight);
                coreWeight=coreWeight+eWeight;
            }
            else
            {
                eNew.setWeight(1);         // dummy inner edge
                coreWeight=coreWeight+1;
            }
            eNew.setEnd(0, v1Name);
            //eNew.setFlow1(min(eWeight,v1.returnWeight()-v1OutF));
            //v1.setEstFlow(eNew.returnFlow1()+v1OutF);
            v1.appendIncid(staticVariables.nextEdgeName);//form an inner edge for each frontier

            eNew.setEnd(1, staticVariables.nextVerName); //nextVerName is core's name
            core.appendIncid(staticVariables.nextEdgeName); // form an incident edge for core

            innerE.append(eNew);       // get a new inner edge for cluster
            eNew.makeEmpty();

            staticVariables.nextEdgeName++;
            v1.setEstFlow(v1OutF);
            F.delVerByName(v1Name);
            F.appendVertex(v1);
        }
        core.setName(staticVariables.nextVerName++);
        //core.setEstFlow(coreWeight);
        if(F.returnDimen()==3) { coreWeight = coreWeight + 6; }
        else  { coreWeight =coreWeight+3; }   //core(w)+frontiers(w)-allEdge(w)=DOF
        core.setWeight(coreWeight);

        core.setDepth(depth+1);
        new_cluster->formCl(core,frontiers,innerE,outerE,originalV);

        if(constrain==0)
            new_cluster->setConst(F.constrainDegree());
        else
            new_cluster->setConst(constrain);
        //out<<"new Cluster"<<endl;
        //new_cluster->output(out);
        //out<<"after getCluster F is"<<endl;
        //F.output(out);
        return new_cluster;
    }

}

#endif