//file: mfa2Clas.h
/* header file that contains the interface for the datastructures
used by the UTU, ESM and DR-planner*/

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

#ifndef MFA_CLASS_H
#define MFA_CLASS_H

// numerical recipies p. 279
#include <math.h>

#include <iostream>

#include "mfa1List.h"

using std::ostream;
using std::cout;
using std::endl;
using std::cin;
using std::string;

namespace ffnx::mfa {

// simple random numbers by numerical recipies
// ran0 returns a value between 0.0 and 1.0
    class Random {
    private:
        int idum;
    public:
        Random(int seed) { idum = seed; }

        float ran0();
    };


    static const int RAND_IA = 16807;
    static const int RAND_IM = 2147483647;
    static const float RAND_AM = 1.0 / RAND_IM;
    static const int RAND_IQ = 127773;
    static const int RAND_IR = 2836;
    static const int RAND_MASK = 123459876;

    float Random::ran0() {
        long int k;
        float ans;
        idum ^= RAND_MASK;
        k = idum / RAND_IQ;
        idum = RAND_IA * (idum - k * RAND_IQ) - RAND_IR * k;
        if (idum < 0) idum += RAND_IM;
        ans = RAND_AM * idum;
        idum ^= RAND_MASK;
        return ans;
    }

    class Vertex {
    private:
        int name;           // unique object ID
        int type;           // point=0,line=1,ray=2,LSeg=3,circle=4,arc=5,cluster=-1
        int weight;         // degree of freedom
        float degValues[8]; // solved values of this object
        int freeDegs[8];    //if freeDegs[i]=0, solver can change degValues[i]
        //if freeDegs[i]=1, degValues[i] is changed by solver
        //if freeDegs[i]=-1, degValues[i] is fixed by user
        //if freeDegs[i]=-2, degValues[i] is NA(Not Applicable) or not defined


        //Table shows the different types of degrees of freedom used for each object type and
        //the degvalue location where it is stored
        /******************************************************************\
        *              point line   Ray   LineSeg circle  arc      cluster *
        *  degValue[0]   x    x1     x1     x1      x      x        x-sft  *
        *  degValue[1]   y    y1     y1     y1      y      y        y-sft  *
        *  degValue[2]   -    x2     x2     x2      r      r       rot-cos *
        *  degValue[3]   -    y2     y2     y2      -    angle     rot-sin *
        *  degValue[4]   -  slope   cos   slope     -  start_cos      -    *
        *  degValue[5]   -          sin   length    -  start_sin      -    *
        *  degValue[6]   -    -      -      -       -   end_cos       -    *
        *  degValue[7]   -    -      -      -       -   end_sin       -    *
        \******************************************************************/

        List<int> incid;  // List of the ID's of the inicident edges
        int nIncid;       // number of incident edges
        int label;        // 0 or 1 used for augmenting path
        int scan;         // 0 or 1 used for augmenting path
        int frozen;       // If frozen=1, we don't distribute; If frozen=2, frozen forever
        int posFlow;      // possible flow that could be delivered to this vertex
        int existFlow;    // flow already absorbed by this vertex
        int predEdge;     // predecessor edge in the augmenting path
        int depth;        // depth==0 for original vertex

    public:

        //Constructors

        //default initializes all values to zero
        Vertex();

        //sets the ID of vertex to theName, and theType to theType
        Vertex(int theName, int theType);

        //copy constructor
        Vertex(const Vertex &);

        void setName(int nm) { name = nm; }

        void setType(int tp) { type = tp; }

        void setWeight(int wt) { weight = wt; }

        void setPosFlow(int fl) { posFlow = fl; }

        void setEstFlow(int fl) { existFlow = fl; }

        void setPredEdg(int e) { predEdge = e; }

        void setDepth(int d) { depth = d; }

        void notDefinedValue(int ix) { freeDegs[ix] = -2; }

        void initialValue(int ix, float value, int free) {
            degValues[ix] = value;
            freeDegs[ix] = free;
        }

        bool setValues(int ix, float value) {
            if (ix >= 8 || freeDegs[ix] < 0)  // solver cannot change degValues[ix]
                return false;
            freeDegs[ix] = 1;
            degValues[ix] = value;
        }

        void appendIncid(int edge_name) {
            incid.append(edge_name);
            nIncid++;
        }

        void setLabel(int lb) { label = lb; }

        void setScan(int sc) { scan = sc; }

        void freeze() { if (frozen == 0) frozen = 1; } // can be defrosted
        void freezeForever() { frozen = 2; }        // can't be defrosted
        void incrsWt(int wt) { weight = weight + wt; }

        void incrsEstFlow(int fl) { existFlow = existFlow + fl; }

        void incrsPosFlow(int fl) { posFlow = posFlow + fl; }

        void delIncid(int e) {
            if (incid.hasElem(e)) {
                incid.deleteElem(e);
                nIncid--;
            }
        }

        void defrost() { if (frozen == 1) frozen = 0; }  // if frozen=2, it can't be defrosted
        int returnName() { return name; }

        int returnType() { return type; }

        int returnWeight() { return weight; }

        bool hasFreeDeg() {
            for (int i = 0; i < 8; i++) if (freeDegs[i] >= 0) return true;
            return false;
        }

        int returnFreeDegs(int index) { return freeDegs[index]; }

        float returnDegValueByName(int index) { return degValues[index]; }

        int returnIncid(int index) { return incid.retrieve(index); }

        int numIncid() { return nIncid; }

        bool hasIncid(int e) { return incid.hasElem(e); }

        int returnLabel() { return label; }

        int returnScan() { return scan; }

        int isFrozen() { return frozen; }

        int returnEstFlow() { return existFlow; }

        int returnPosFlow() { return posFlow; }

        int returnPredEdg() { return predEdge; }

        int returnDepth() { return depth; }

        //overridden = operator
        Vertex &operator=(const Vertex &ver);

        //two vertices are equal if they have the same name and weight
        inline bool operator==(const Vertex &v) {
            if (name == v.name && weight == v.weight) return true;
            else return false;
        }

        //stream output
        friend ostream &operator<<(ostream &, const Vertex &);
    };

    Vertex::Vertex() {
        name = 0;
        type = 0;
        weight = 0;
        nIncid = 0;
        label = 0;
        scan = 0;
        frozen = 0;
        posFlow = 0;
        existFlow = 0;
        predEdge = 0;
        depth = 0;
        for (int i = 0; i < 8; i++) {
            degValues[i] = 0;
            freeDegs[i] = 0;
        }
    }

    Vertex::Vertex(int theName, int theType) {
        name = theName;
        type = theType;
        weight = 0;
        nIncid = 0;
        label = 0;
        scan = 0;
        frozen = 0;
        posFlow = 0;
        existFlow = 0;
        predEdge = 0;
        depth = 0;
        for (int i = 0; i < 8; i++) {
            degValues[i] = 0;
            freeDegs[i] = 0;
        }
    }

    Vertex::Vertex(const Vertex &ver) {
        name = ver.name;
        type = ver.type;
        weight = ver.weight;
        nIncid = ver.nIncid;
        label = ver.label;
        scan = ver.scan;
        frozen = ver.frozen;
        posFlow = ver.posFlow;
        existFlow = ver.existFlow;
        predEdge = ver.predEdge;
        depth = ver.depth;
        incid = ver.incid;
        for (int i = 0; i < 8; i++) {
            freeDegs[i] = ver.freeDegs[i];
            degValues[i] = ver.degValues[i];
        }
    }

    Vertex &Vertex::operator=(const Vertex &ver) {
        if (this != &ver) {
            name = ver.name;
            type = ver.type;
            weight = ver.weight;
            nIncid = ver.nIncid;
            label = ver.label;
            scan = ver.scan;
            frozen = ver.frozen;
            posFlow = ver.posFlow;
            existFlow = ver.existFlow;
            predEdge = ver.predEdge;
            depth = ver.depth;
            incid = ver.incid;
            for (int i = 0; i < 8; i++) {
                freeDegs[i] = ver.freeDegs[i];
                degValues[i] = ver.degValues[i];
            }
        }
        return *this;
    }

    ostream &operator<<(ostream &os, const Vertex &ver) {
        int i;
        os << "Ver=" << ver.name << ",T=" << ver.type << ", W=" << ver.weight << ", d=" << ver.depth
           << ", LSF=" << ver.label << ver.scan << ver.frozen
           << ", PF=" << ver.posFlow << ", EF=" << ver.existFlow;
        os << ", edge:";
        if (ver.nIncid > 0)
            for (i = 1; i <= ver.nIncid; i++)
                os << (ver.incid).retrieve(i) << ' ';
        os << "PD=" << ver.predEdge;
        os << " DOF=";
        for (i = 0; i < 8; i++) {
            if (*(ver.freeDegs + i) == 0)
                os << "un";          //unknown
            else if (*(ver.freeDegs + i) == -2)
                os << "NA";          //Not Applicable (not defined)
            else
                os << *(ver.degValues + i);
            if (i < 7) os << ' ';
        }
        return os;
    }

    class Edge {
    protected:
        int name;             // name = Constraint ID
        int type;             // 0=distance, 1=incidence, ...
        int weight;           // DOF is removed by the constraint
        float value;          // of distance, angle, etc.
        int end1;         // ID of the vertex at end1
        int end2;             // ID of the vertex at end2
        int part1;            // part of shape involved in an incidence constraint
        // 0=arbitrary, 1=point1, 2=point2, 3=center
        int part2;            // same as part1

        int label;            // 0 or 1 used for augmenting path
        int scan;             // 0 or 1 used for augmenting path
        int flow1;            // flow to end1
        int flow2;            // flow to end2
        int posFlow;          // possible flow to deliver to this edge
        int predVertex;       // predecessor Vertex in the augmenting path

    public:

        //default constructor sets all values to zero
        Edge() {
            name = 0;
            type = 0;
            weight = 0;
            value = 0.0;
            end1 = 0;
            end2 = 0;
            part1 = 0;
            part2 = 0;
            label = 0;
            scan = 0;
            flow1 = 0;
            flow2 = 0;
            posFlow = 0;
            predVertex = 0;
        }

        //copy constructor
        Edge(const Edge &);


        void setName(int nm) { name = nm; }

        void setType(int tp) { type = tp; }

        void setWeight(int wt) { weight = wt; }

        void setValue(float vl) { value = vl; }

        void setEnd(int i, int v) { if (i == 0) end1 = v; else end2 = v; }

        void setPart(int i, int p) { if (i == 0) part1 = p; else part2 = p; }

        void setLabel(int lb) { label = lb; }

        void setScan(int sc) { scan = sc; }

        void setFlow1(int fl) { flow1 = fl; }

        void setFlow2(int fl) { flow2 = fl; }

        void setPosFlow(int fl) { posFlow = fl; }

        void setPredVer(int v) { predVertex = v; }

        void incrsWt(int wt) { weight = weight + wt; }

        void incrsFlow1(int fl) { flow1 = flow1 + fl; }

        void incrsFlow2(int fl) { flow2 = flow2 + fl; }

        void incrsPosFlow(int fl) { posFlow = posFlow + fl; }

        void makeEmpty() {
            name = 0;
            weight = 0;
        }

        int returnName() { return name; }

        int returnType() { return type; }

        int returnWeight() { return weight; }

        float returnValue() { return value; }

        int returnEnd1() { return end1; }

        int returnEnd2() { return end2; }

        int returnPart1() { return part1; }

        int returnPart2() { return part2; }

        int otherEnd(int v) { if (v == end1) return end2; else return end1; }

        int returnLabel() { return label; }

        int returnScan() { return scan; }

        int returnFlow1() { return flow1; }

        int returnFlow2() { return flow2; }

        int returnPosFlow() { return posFlow; }

        int returnPredVer() { return predVertex; }

        //= operator
        Edge &operator=(const Edge &edg);

        //two edges are equal if their ID's are the same
        inline bool operator==(const Edge &e) {
            if (name == e.name) return true;
            else return false;
        }

        //stream output
        friend ostream &operator<<(ostream &, const Edge &);
    };

    Edge::Edge(const Edge &edg) {
        name = edg.name;
        type = edg.type;
        weight = edg.weight;
        value = edg.value;
        end1 = edg.end1;
        end2 = edg.end2;
        part1 = edg.part1;
        part2 = edg.part2;
        label = edg.label;
        scan = edg.scan;
        flow1 = edg.flow1;
        flow2 = edg.flow2;
        posFlow = edg.posFlow;
        predVertex = edg.predVertex;
    }

    Edge &Edge::operator=(const Edge &edg) {
        if (this != &edg) {
            name = edg.name;
            type = edg.type;
            weight = edg.weight;
            value = edg.value;
            end1 = edg.end1;
            end2 = edg.end2;
            part1 = edg.part1;
            part2 = edg.part2;
            label = edg.label;
            scan = edg.scan;
            flow1 = edg.flow1;
            flow2 = edg.flow2;
            posFlow = edg.posFlow;
            predVertex = edg.predVertex;
        }
        return *this;
    }

    ostream &operator<<(ostream &os, const Edge &edg) {
        os << "Edg=" << edg.name << ",T=" << edg.type << ", W=" << edg.weight << ",V=";
        if (edg.value < -1)     //if (edg.value==-2.0)
            os << "NA";          // Not Applicable
        else if (edg.value < 0) //if (edg.value==-1.0)
            os << "un";          //unknown
        else
            os << edg.value;
        os << ", LS=" << edg.label << edg.scan << ", PF=" << edg.posFlow << ", F1=" << edg.flow1
           << ", F2=" << edg.flow2 << ", Ends: " << edg.end1 << '(' << edg.part1 << ") "
           << edg.end2 << '(' << edg.part2 << ") PD=" << edg.predVertex;
        return os;
    }

    class Graph {
    private:
        int dimension;        // dimension=2 or 3
        int NumVert;        // # of vertices
        int NumEdge;            // # of edges
        int depth;            // depth of the largest cluster in the graph
        List<Vertex> vertices;    // list of the vertices in the graph
        List<Edge> edges;        // list of the edges in the graph

        //see separate pseudocode and paper documentation of explanation
        void augment(Vertex *, Edge *);

    public:

        //constructor
        Graph() {
            dimension = 2;
            NumVert = 0;
            NumEdge = 0;
            depth = 0;
        }

        void setDimen(int dimen) { dimension = dimen; }

        void setDepth(int d) { depth = d; }

        void setVlist(List<Vertex> &vl) {
            NumVert = vl.returnLen();
            vertices = vl;
        }

        void setElist(List<Edge> &el) {
            NumEdge = el.returnLen();
            edges = el;
        }

        // merge multi-edges and remove zero-weight edges
        void simplify();

        // freeze all vertices
        void freeze();

        // defrost vertices if any of them are frozen
        void defrostGraph();

        // freeze the vertex with ID vName forever (set frozen to 2)
        void freezeVertForever(int vName);

        //defrost the vertex with ID vName
        void defrostOneVert(int vName);


        void appendVertex(Vertex &ver) {
            vertices.append(ver);
            NumVert++;
        }

        void appendEdge(Edge &edg) {
            edges.append(edg);
            NumEdge++;
        }

        void delAllVer() {
            vertices.makeEmpty();
            NumVert = 0;
        }

        void delAllEdg() {
            edges.makeEmpty();
            NumEdge = 0;
        }

        void makeEmpty() {
            delAllVer();
            delAllEdg();
        }

        void delVer(Vertex &v) {
            if ((NumVert > 0) && hasVert(v.returnName())) {
                vertices.deleteElem(v);
                NumVert--;
            }
        }

        void delEdg(Edge &e) {
            if ((NumEdge > 0) && hasEdge(e.returnName())) {
                edges.deleteElem(e);
                NumEdge--;
            }
        }

        //find either edge or vertex by ID and remover from vertex or edge list
        void delVerByName(int vName);

        void delEdgeByName(int eName);

        //true if the graph has an edge or vertex with the given name
        bool hasEdge(int eName);

        bool hasVert(int vName);


        int returnDimen() { return dimension; }

        int returnNumVer() { return NumVert; }

        int returnNumEdg() { return NumEdge; }

        int returnDepth() { return depth; }

        //returns the number of labeled edges in the graph
        int numLabeled();

        List<Vertex> &returnVlist() { return vertices; }

        List<Edge> &returnElist() { return edges; }

        Vertex &returnVertByIndex(int i) { return vertices.retrieve(i); }

        Edge &returnEdgeByIndex(int i) { return edges.retrieve(i); }

        Vertex &returnVertByName(int vName);

        Edge &returnEdgeByName(int eName);

        Edge &returnEdgeByEnds(int v1, int v2);


        // return a Edge or Vertex point to the edge or vertex with the given ID
        Vertex *VertAddr(int vName);

        Edge *EdgeAddr(int eName);

        // used for testing generates a random graph
        void randomGraph();

        //stream output
        void output(ostream &);

        //set all of the labels in both the edges and the vertices to the unlabeled state, 0
        void removeLabels();

        // returns the flow on an edge back to undistributed flow on vertices
        void restoreFlow(Edge *edgep);

        // see the attached paper for complete documentation
        int distribute0(Edge &edge, ostream &file2);

        // returns the difference between the total weight of the edges and the weight of the vertices
        // less the number of degrees of freedom of a rigid object in space, 2D = 3, 3D = 6
        int constrainDegree();
    };

    void Graph::simplify() {
        int i, j, tempNum, e1Name, e2Name, v11, v12, v21, v22;
        Edge edg1, edg2;
        Vertex *vert1, *vert2;

        tempNum = NumEdge;
        for (i = 1; i <= tempNum; i++) {
            edg1 = edges.pop();
            e1Name = edg1.returnName();
            v11 = edg1.returnEnd1();
            v12 = edg1.returnEnd2();
            if (edg1.returnWeight() == 0) {
                vert1 = VertAddr(e1Name);
                vert2 = VertAddr(e1Name);
                vert1->delIncid(e1Name);
                vert2->delIncid(e1Name);
                NumEdge--;
            } else {
                for (j = 1; j < tempNum; j++) {
                    edg2 = edges.pop();
                    e2Name = edg2.returnName();
                    if (e2Name != 0) {
                        v21 = edg2.returnEnd1();
                        v22 = edg2.returnEnd2();
                        if ((v11 == v21 && v12 == v22) || (v11 == v22 && v12 == v21)) {
                            vert1 = VertAddr(v21);
                            vert2 = VertAddr(v22);
                            vert1->delIncid(e2Name);
                            vert2->delIncid(e2Name);
                            edg1.incrsWt(edg2.returnWeight());
                            NumEdge--;
                        } else {
                            edges.append(edg2);
                        }
                    }//end of if(e2Name!=0)
                } //end of for(j=0;j<NumEdge;i++)
                edges.append(edg1);
            } //end of else
        }
    }

    void Graph::freeze() {
        int i, len;
        Vertex *verP;

        len = vertices.returnLen();
        for (i = 1; i <= len; i++) {
            verP = vertices.addrByIndex(i);
            verP->freeze();
        }
    }


    void Graph::defrostGraph() {
        int i, len;
        Vertex *verP;

        len = vertices.returnLen();
        for (i = 1; i <= len; i++) {
            //out<<"defrosting ver="<<vertices.retrieve(i)<<endl;
            verP = vertices.addrByIndex(i);
            verP->defrost();
        }
    }

    void Graph::freezeVertForever(int vName) {
        int i, len;
        Vertex *verP;

        len = vertices.returnLen();
        for (i = 1; i <= len; i++) {
            verP = vertices.addrByIndex(i);
            if (verP->returnName() == vName) {
                verP->freezeForever();
                return;
            }
        }
    }

    void Graph::defrostOneVert(int vName) {
        int i, len;
        Vertex *verP;

        len = vertices.returnLen();
        for (i = 1; i <= len; i++) {
            verP = vertices.addrByIndex(i);
            if (verP->returnName() == vName) {
                verP->defrost();
                return;
            }
        }
    }

    void Graph::delVerByName(int vName) {
        int i;
        for (i = 1; i <= NumVert; i++) {
            if (vertices.retrieve(i).returnName() == vName) {
                vertices.deleteIndex(i);
                NumVert--;
            }
        }
    }

    void Graph::delEdgeByName(int eName) {
        int i;
        for (i = 1; i <= NumEdge; i++) {
            if (edges.retrieve(i).returnName() == eName) {
                edges.deleteIndex(i);
                NumEdge--;
            }
        }
    }

    int Graph::numLabeled() {
        int i, labeled;
        labeled = 0;
        for (i = 1; i <= NumVert; i++) {
            if (vertices.retrieve(i).returnLabel() != 0) labeled++;
        }
        return labeled;
    }

    Edge &Graph::returnEdgeByEnds(int v1, int v2) {
        int i, v01, v02;
        for (i = 0; i < NumEdge; i++) {
            v01 = edges.retrieve(i + 1).returnEnd1();
            v02 = edges.retrieve(i + 1).returnEnd2();
            if ((v1 == v01 && v2 == v02) || (v1 == v02 && v2 == v01))
                return edges.retrieve(i + 1);
        }
        Edge *dummy = new Edge;
        return *dummy;
    }

    bool Graph::hasEdge(int eName) {
        int i;
        for (i = 0; i < NumEdge; i++)
            if (eName == edges.retrieve(i + 1).returnName()) return true;
        return false;
    }

    bool Graph::hasVert(int vName) {
        int i;
        for (i = 0; i < NumVert; i++)
            if (vName == vertices.retrieve(i + 1).returnName()) return true;
        return false;
    }

    Edge &Graph::returnEdgeByName(int eName) {
        int i;
        for (i = 0; i < NumEdge; i++)
            if (eName == edges.retrieve(i + 1).returnName())
                return (edges.retrieve(i + 1));
        Edge *dummy;
        return *dummy;
    }

    Vertex &Graph::returnVertByName(int vName) {
        int i;
        for (i = 0; i < NumVert; i++)
            if (vName == vertices.retrieve(i + 1).returnName())
                return (vertices.retrieve(i + 1));
        Vertex *dummy;
        return *dummy;
    }

    Vertex *Graph::VertAddr(int vName) {
        int i;
        for (i = 1; i <= NumVert; i++)
            if (vName == vertices.retrieve(i).returnName()) return vertices.addrByIndex(i);
        return NULL;
    }

    Edge *Graph::EdgeAddr(int eName) {
        int i;
        for (i = 1; i <= NumEdge; i++)
            if (eName == edges.retrieve(i).returnName()) return edges.addrByIndex(i);
        return NULL;
    }

    void Graph::augment(Vertex *v3p, Edge *edgep) {
        int wt;
        Vertex *vp;
        Edge *ep;

        wt = min(v3p->returnPosFlow(), v3p->returnWeight() - v3p->returnEstFlow());
        edgep->incrsPosFlow(-wt);
        vp = v3p;
        ep = EdgeAddr(vp->returnPredEdg());

        while (ep != NULL) {
            if (ep->returnEnd1() == vp->returnName()) {
                ep->incrsFlow1(wt);
                vp->incrsEstFlow(wt);
            }
            if (ep->returnEnd2() == vp->returnName()) {
                ep->incrsFlow2(wt);
                vp->incrsEstFlow(wt);
            }

            if (ep->returnPredVer() == 0) {
                ep = NULL;
            } else {
                vp = VertAddr(ep->returnPredVer());
                if (ep->returnEnd1() == vp->returnName()) {
                    ep->incrsFlow1(-wt);
                    vp->incrsEstFlow(-wt);
                }
                if (ep->returnEnd2() == vp->returnName()) {
                    ep->incrsFlow2(-wt);
                    vp->incrsEstFlow(-wt);
                }
                ep = EdgeAddr(vp->returnPredEdg());
            }
        }
    }

    void Graph::removeLabels() {
        int i;
        Vertex *v;
        Edge *e;

        for (i = 1; i <= NumVert; i++) {
            v = vertices.addrByIndex(i);
            v->setLabel(0);
            v->setScan(0);
        }

        for (i = 1; i <= NumEdge; i++) {
            e = edges.addrByIndex(i);
            e->setLabel(0);
            e->setScan(0);
        }
    }

    void Graph::restoreFlow(Edge *edgep) {
        Vertex *end1, *end2;
        end1 = VertAddr(edgep->returnEnd1());
        end2 = VertAddr(edgep->returnEnd2());

        end1->incrsEstFlow(-(edgep->returnFlow1()));
        end2->incrsEstFlow(-(edgep->returnFlow2()));

        edgep->setFlow1(0);
        edgep->setFlow2(0);
    }

    int Graph::constrainDegree() {
        int i, constrain;
        Vertex vert;
        Edge edge;

        if (dimension == 2) constrain = 4;
        if (dimension == 3) constrain = 7;

        for (i = 1; i <= NumVert; i++) {
            vert = vertices.retrieve(i);
            if (vert.returnLabel() != 0)
                constrain = constrain - vert.returnWeight();
        }

        for (i = 1; i <= NumEdge; i++) {
            edge = edges.retrieve(i);
            if (edge.returnLabel() != 0)
                constrain = constrain + edge.returnWeight();
        }
        return constrain;
    }

    //distribut Edge
    int Graph::distribute0(Edge &edge, ostream &file2) {
        int no_of_unscanned = 1;
        Edge *edgep, *ep;
        Vertex *v1p, *v2p, *v3p;
        int i, j;


        removeLabels();
        //file2<<"Before distribute edge"<<edge.returnName()<<endl;
        //output(file2);
        edges.append(edge); //add edge in F
        NumEdge++;
        edgep = EdgeAddr(edge.returnName());
        edgep->setLabel(1);
        edgep->setPosFlow(edgep->returnWeight());
        edgep->setPredVer(0);


        while (no_of_unscanned > 0 && edgep->returnPosFlow() > 0) {
            no_of_unscanned = 0;
            for (j = 1; j <= NumEdge; j++)  // for every labeled edge in F
            {
                ep = edges.addrByIndex(j);
                if (ep->returnLabel() == 1 && ep->returnScan() == 0) {
                    v1p = VertAddr(ep->returnEnd1());
                    v2p = VertAddr(ep->returnEnd2());
                    if (v1p->returnLabel() == 0) {
                        v1p->setLabel(1);
                        v1p->setPosFlow(ep->returnPosFlow());
                        v1p->setPredEdg(ep->returnName());
                        no_of_unscanned++;
                    }
                    if (v2p->returnLabel() == 0) {
                        v2p->setLabel(1);
                        v2p->setPosFlow(ep->returnPosFlow());
                        v2p->setPredEdg(ep->returnName());
                        no_of_unscanned++;
                    }
                    ep->setScan(1);
                }
            }

            for (i = 1; i <= NumVert; i++)  // for every labeled vertices in F
            {
                v3p = vertices.addrByIndex(i);
                if (v3p->returnLabel() == 1 && v3p->returnScan() == 0) {
                    if (v3p->returnEstFlow() < v3p->returnWeight()) {
                        augment(v3p, edgep);
                        removeLabels();

                        if (edgep->returnPosFlow() > 0)
                            edgep->setLabel(1);
                    } else {
                        for (j = 1; j <= NumEdge; j++)//for every non-label edge,incid to v3
                        {
                            ep = edges.addrByIndex(j);
                            if (ep->returnLabel() == 0 && ep->returnFlow1() > 0
                                && v3p->returnName() == ep->returnEnd1()) {
                                ep->setLabel(1);
                                ep->setPosFlow(min(v3p->returnPosFlow(), ep->returnFlow1()));
                                ep->setPredVer(v3p->returnName());
                            }
                            if (ep->returnLabel() == 0 && ep->returnFlow2() > 0
                                && v3p->returnName() == ep->returnEnd2()) {
                                ep->setLabel(1);
                                ep->setPosFlow(min(v3p->returnPosFlow(), ep->returnFlow2()));
                                ep->setPredVer(v3p->returnName());
                            }
                        }
                        v3p->setScan(1);
                        no_of_unscanned++;
                    }
                }
            }
        }

        if (edgep->returnPosFlow() <= 0)
            return 0;
        else {
            restoreFlow(edgep);
            return 1; // not able to distribute the edge completely
        }
    }

    void Graph::randomGraph() {
        int i, j, Vweight, Eweight;
        float propability;
        Vertex old_ver;
        Random random(11);

        cout << "creating a random graph\n";
        cout << "input number of vertices: ";
        cin >> NumVert;
        cout << "edge existing propability between two vertices:";
        cin >> propability;

        for (i = 1; i <= NumVert; i++) {
            Vertex new_ver;
            new_ver.setName(i);
            Vweight = 2 + (int) (3 * random.ran0());
            new_ver.setWeight(Vweight);
            vertices.append(new_ver);
        }

        NumEdge = 0;
        for (i = 1; i <= NumVert; i++) {
            for (j = i + 1; j <= NumVert; j++) {
                if (random.ran0() + propability >= 1.0) {
                    Edge new_edg;
                    NumEdge++;
                    new_edg.setName(NumEdge);
                    Eweight = 1 + (int) (2 * random.ran0());
                    new_edg.setWeight(Eweight);
                    new_edg.setEnd(0, i);
                    new_edg.setEnd(1, j);
                    edges.append(new_edg);

                    old_ver = vertices.retrieve(i);
                    old_ver.appendIncid(NumEdge);
                    vertices.changeElem(i, old_ver); // Vname==position

                    old_ver = vertices.retrieve(j);
                    old_ver.appendIncid(NumEdge);
                    vertices.changeElem(j, old_ver); // Vname==position
                }
            }
        }
        cout << "NumEdge=" << NumEdge << endl;
    }


    void Graph::output(ostream &os) {
        int i;
        if (NumVert == 0 && NumEdge == 0) {
            os << "Graph is Empty." << endl;
            return;
        }
        for (i = 1; i <= NumVert; i++)
            os << vertices.retrieve(i) << endl;

        for (i = 1; i <= NumEdge; i++)
            os << edges.retrieve(i) << endl;
    }


//class that stores one set of degvalues exactly as described in Class Vertex above
    class DegValues {

    private:

        float degValues[8];

    public:

        DegValues() {
            int i;

            for (i = 0; i < 8; i++) degValues[i] = 0;
        }

        void setValue(int i, float value) { degValues[i] = value; }

        float returnValue(int i) { return degValues[i]; }

        DegValues &operator=(const DegValues &deg);
    };

    DegValues &DegValues::operator=(const DegValues &deg) {
        int i;

        if (this != &deg) {
            for (i = 0; i < 8; i++)
                degValues[i] = deg.degValues[i];

        }
        return *this;
    }

    class Cluster {
    private:
        Vertex core;           // core vertex formed by the reduction of all the interior vertices in the cluster
        int group;             // group ID is specified by user
        List<int> frontiers;   // list of the frontier edges in the cluster by ID
        List<Edge> innerE;     // list of edges between core and frontiers
        List<Edge> outerE;     // list of edges between frontiers
        List<int> originalV;   // list of vertices corresponding to original graph
        List<string> bifurs;   // list of the strings representing the bifurcations of this cluster
        int constrain;         // 1--well constrained, 2,3,...--over constrained
        int numBifurs;         // lenght of the list bifurs
        int currBifur;         // the currently selected bifurcation, -1 means no selection
        bool solved;           // true if the cluster has been solved before
        int currDeg;           // the index of the currect set of DegValues;
        List<DegValues> values;// same format and use as in Vertex

    public:
        List<Cluster> children;    // list of children

        //Default constructor sets all values to default, note that the default value of the degValues[2] is 1
        //since a zero rotation has a cosine of 1
        Cluster() {
            int i, j;
            DegValues first;

            first.setValue(2, 1.0);
            group = 0;
            constrain = 0;
            currBifur = -1;
            numBifurs = 0;
            solved = false;
            currDeg = 1;
            values.append(first);

        }

        //intializes a cluster in group g
        Cluster(int g) { group = g; }

        //copy constructor
        Cluster(const Cluster &);

        void setName(int n) { core.setName(n); }

        void setGroup(int g) { group = g; }

        void setBifurs(List<string> &theBifurs) {
            bifurs = theBifurs;
            numBifurs = theBifurs.returnLen();
        }

        void setCurrBifur(int theNum) { currBifur = theNum; }

        void setCore(Vertex &c) { core = c; }

        void setConst(int c) { constrain = c; }

        void setValue(int index, float value) { values.retrieve(currDeg).setValue(index, value); }

        void setSolved(bool sol) { solved = sol; }

        void setCurrDeg(int i) { currDeg = i; }

        void setDegValues(List<DegValues> theValues) { values = theValues; }

        //adds a new set of degvalues to this cluster in the case that this cluster has multiple parents
        void setToCluster(Cluster &theCluster) {
            int i, length, name;
            bool found;
            DegValues theNew;

            length = values.returnLen();
            name = theCluster.returnName();

            if (length == 1 && values.retrieve(1).returnValue(7) == 0) {
                currDeg = 1;
                values.retrieve(currDeg).setValue(7, name);
            } else {
                found = false;
                for (i = 1; i <= length; i++)
                    if (values.retrieve(i).returnValue(7) == name) {
                        found = true;
                        currDeg = 1;
                    }
                if (!found) {
                    theNew.setValue(2, 1.0);
                    values.append(theNew);
                    currDeg = length + 1;
                    values.retrieve(currDeg).setValue(7, name);
                }
            }
        }

        //creates a new cluster
        void formCl(Vertex &, List<int> &, List<Edge> &, List<Edge> &, List<int> &);

        void addOrig(int v) { if (!originalV.hasElem(v)) originalV.append(v); }

        void addFrot(int v) { if (!frontiers.hasElem(v)) frontiers.append(v); }

        void deleteOrig(int ver) { originalV.deleteElem(ver); }

        void deleteFrot(int ver) { frontiers.deleteElem(ver); }

        void makeEmpty() {
            int i, j;
            DegValues first;

            core.setName(0);
            group = 0;
            frontiers.makeEmpty();
            innerE.makeEmpty();
            outerE.makeEmpty();
            originalV.makeEmpty();
            children.makeEmpty();
            bifurs.makeEmpty();
            currBifur = -1;
            numBifurs = 0;
            constrain = 0;
            solved = false;
            values.makeEmpty();
            currDeg = 1;
            first.setValue(2, 1.0);
            values.append(first);
        }

        Vertex &returnCore() { return core; }

        int returnName() { return core.returnName(); }

        int returnGroup() { return group; }

        int returnConst() { return constrain; }

        int returnCurrDeg() { return currDeg; }

        int returnNumDeg() { return values.returnLen(); }

        float returnValue(int index) { return values.retrieve(currDeg).returnValue(index); }

        int returnNumBifurs() { return numBifurs; }

        int returnCurrBifur() { return currBifur; }

        string returnCurrBifurString() { return bifurs.retrieve(currBifur); }

        List<string> &returnBifurs() { return bifurs; }

        List<int> &returnFrontiers() { return frontiers; }

        int returnFronLen() { return frontiers.returnLen(); }

        int returnFrontier(int index) { return frontiers.retrieve(index); }

        List<Edge> &returnInnerE() { return innerE; }

        int returnInnerELen() { return innerE.returnLen(); }

        Edge &returnInnerE(int index) { return innerE.retrieve(index); }

        List<Edge> &returnOuterE() { return outerE; }

        int returnOuterELen() { return outerE.returnLen(); }

        Edge &returnOuterE(int index) { return outerE.retrieve(index); }

        List<int> &returnOrig() { return originalV; }

        int returnOrigV(int index) { return originalV.retrieve(index); }

        int returnOrigLen() { return originalV.returnLen(); }

        bool isSolved() { return solved; }


        bool empty() {
            if (core.returnName() == 0 && group == 0 && children.empty())
                return true;
            else return false;
        }

        //clusters are equal if they have the same name and are in the same group
        inline bool operator==(const Cluster &clu) {
            if (core == clu.core && group == clu.group) return true;
            else return false;
        }

        // = operator
        Cluster &operator=(const Cluster &clu);

        // a cluster is a sub cluster of another if all of its original vertices are original vertices in the
        // larger cluster
        bool operator<=(Cluster &cl0)  // subset operator
        {
            int i, nFront, nOrig0;
            List<int> origV0;

            nFront = frontiers.returnLen();
            origV0 = cl0.returnOrig();
            nOrig0 = origV0.returnLen();
            if (nFront > nOrig0 || nFront < 2) return false;

            for (i = 1; i <= nFront; i++) {
                if (!origV0.hasElem(frontiers.retrieve(i))) return false;
            }
            return true;
        }

        // stream output
        void output(ostream &);
    };


    Cluster::Cluster(const Cluster &clu) {
        int i;

        core = clu.core;
        group = clu.group;
        frontiers = clu.frontiers;
        innerE = clu.innerE;
        outerE = clu.outerE;
        originalV = clu.originalV;
        children = clu.children;
        bifurs = clu.bifurs;
        currBifur = clu.currBifur;
        numBifurs = clu.numBifurs;
        constrain = clu.constrain;
        solved = clu.solved;
        values = clu.values;
    }

    void Cluster::formCl(Vertex &c, List<int> &fV, List<Edge> &iE,
                         List<Edge> &oE, List<int> &o) {
        core = c;
        frontiers = fV;
        innerE = iE;
        outerE = oE;
        originalV = o;
    }

    Cluster &Cluster::operator=(const Cluster &clu) {
        int i;

        if (this != &clu) {
            core = clu.core;
            group = clu.group;
            frontiers = clu.frontiers;
            innerE = clu.innerE;
            outerE = clu.outerE;
            originalV = clu.originalV;
            children = clu.children;
            bifurs = clu.bifurs;
            currBifur = clu.currBifur;
            numBifurs = clu.numBifurs;
            constrain = clu.constrain;
            solved = clu.solved;
            values = clu.values;
        }
        return *this;
    }


    void Cluster::output(ostream &os) {
        int i, len, child;
        os << "Cluster(core):" << core.returnName()
           << ", depth=" << core.returnDepth();
        if (constrain == 1) os << ", well constrained";
        if (constrain > 1) os << ", over constrained";
        if (constrain < 1) os << ", under constrained";
        os << endl << "Frontiers: ";
        len = frontiers.returnLen();
        for (i = 1; i <= len; i++)
            os << frontiers.retrieve(i) << ' ';

        os << endl << "Previous clusters: ";
        len = children.returnLen();
        for (i = 1; i <= len; i++) {
            child = children.retrieve(i).returnCore().returnName();
            if (child != 0)
                os << child << ' ';
            else
                os << "Grp" << children.retrieve(i).returnGroup() << ' ';
        }

        os << endl << "Original Vertices: ";
        len = originalV.returnLen();
        for (i = 1; i <= len; i++)
            os << originalV.retrieve(i) << ' ';

        os << endl << "Inner Edge: ";
        len = innerE.returnLen();
        for (i = 1; i <= len; i++)
            os << innerE.retrieve(i).returnName() << '('
               << innerE.retrieve(i).returnWeight() << ','
               << innerE.retrieve(i).returnEnd1() << '-'
               << innerE.retrieve(i).returnEnd2() << ") ";

        os << endl << "Onter Edge: ";
        len = outerE.returnLen();
        for (i = 1; i <= len; i++)
            os << outerE.retrieve(i).returnName() << '('
               << outerE.retrieve(i).returnWeight() << ','
               << outerE.retrieve(i).returnEnd1() << '-'
               << outerE.retrieve(i).returnEnd2() << ") ";

        os << endl << "Bifurcations (Current Selection: ";
        if (currBifur < 1) os << "None):" << endl;
        else os << currBifur << "):" << endl;
        len = bifurs.returnLen();
        for (i = 1; i <= len; i++)
            os << "    " << bifurs.retrieve(i) << endl;

        os << endl;
    }


    class StaticVariables {
    public:
        int singleVertex = 1;
        int nextVerName = 1;
        int nextEdgeName = 1;
    };
}

#endif
