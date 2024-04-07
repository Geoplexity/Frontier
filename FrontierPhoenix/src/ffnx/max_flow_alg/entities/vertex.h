#ifndef ENTITIES_VERTEX_H
#define ENTITIES_VERTEX_H

#include <iostream>

#include "../mfa1List.h"

namespace ffnx::mfa {

    class Vertex {
    public:
        static constexpr int TYPE_POINT = 0;

    private:
        int name;           // unique object ID
        int weight;         // degree of freedom

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
        Vertex() {
            name = 0;
            weight = 0;
            nIncid = 0;
            label = 0;
            scan = 0;
            frozen = 0;
            posFlow = 0;
            existFlow = 0;
            predEdge = 0;
            depth = 0;
        }

        Vertex(int theName, int theType) {
            name = theName;
            weight = 0;
            nIncid = 0;
            label = 0;
            scan = 0;
            frozen = 0;
            posFlow = 0;
            existFlow = 0;
            predEdge = 0;
            depth = 0;
        }

        Vertex(const Vertex &ver) {
            name = ver.name;
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
        }

        void setName(int nm) { name = nm; }

        void setWeight(int wt) { weight = wt; }

        void setPosFlow(int fl) { posFlow = fl; }

        void setEstFlow(int fl) { existFlow = fl; }

        void setPredEdg(int e) { predEdge = e; }

        void setDepth(int d) { depth = d; }

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

        int returnWeight() { return weight; }

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
        friend std::ostream &operator<<(std::ostream &, const Vertex &);
    };


    Vertex &Vertex::operator=(const Vertex &ver) {
        if (this != &ver) {
            name = ver.name;
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
        }
        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const Vertex &ver) {
        int i;
        os << "Ver=" << ver.name << ", W=" << ver.weight << ", d=" << ver.depth
           << ", LSF=" << ver.label << ver.scan << ver.frozen
           << ", PF=" << ver.posFlow << ", EF=" << ver.existFlow;
        os << ", edge:";
        if (ver.nIncid > 0)
            for (i = 1; i <= ver.nIncid; i++)
                os << (ver.incid).retrieve(i) << ' ';
        os << "PD=" << ver.predEdge;
        return os;
    }

}

#endif