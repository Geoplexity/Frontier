#ifndef ENTITIES_GRAPH_H
#define ENTITIES_GRAPH_H

#include "vertex.h"
#include "edge.h"

namespace ffnx::mfa {

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

        //stream output
        void output(std::ostream &);

        //set all of the labels in both the edges and the vertices to the unlabeled state, 0
        void removeLabels();

        // returns the flow on an edge back to undistributed flow on vertices
        void restoreFlow(Edge *edgep);

        // see the attached paper for complete documentation
        int distribute0(Edge &edge, std::ostream &file2);

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
            //out<<"defrosting ver="<<vertices.retrieve(i)<<std::endl;
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
    int Graph::distribute0(Edge &edge, std::ostream &file2) {
        int no_of_unscanned = 1;
        Edge *edgep, *ep;
        Vertex *v1p, *v2p, *v3p;
        int i, j;


        removeLabels();
        //file2<<"Before distribute edge"<<edge.returnName()<<std::endl;
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

    void Graph::output(std::ostream &os) {
        int i;
        if (NumVert == 0 && NumEdge == 0) {
            os << "Graph is Empty." << std::endl;
            return;
        }
        for (i = 1; i <= NumVert; i++)
            os << vertices.retrieve(i) << std::endl;

        for (i = 1; i <= NumEdge; i++)
            os << edges.retrieve(i) << std::endl;
    }


}

#endif