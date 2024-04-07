#ifndef ENTITIES_VERTEX_H
#define ENTITIES_VERTEX_H

#include <iostream>

#include "../mfa1List.h"

namespace ffnx::mfa {

    class Vertex {
    private:
        int name = 0;           // unique object ID
        int weight = 0;         // degree of freedom

        List<int> incid = List<int>();  // List of the ID's of the inicident edges
        int nIncid = 0;       // number of incident edges
        int label = 0;        // 0 or 1 used for augmenting path
        int scan = 0;         // 0 or 1 used for augmenting path
        int frozen = 0;       // If frozen=1, we don't distribute; If frozen=2, frozen forever
        int posFlow = 0;      // possible flow that could be delivered to this vertex
        int existFlow = 0;    // flow already absorbed by this vertex
        int predEdge = 0;     // predecessor edge in the augmenting path
        int depth = 0;        // depth==0 for original vertex

    public:

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

        //two vertices are equal if they have the same name and weight
        inline bool operator==(const Vertex &v) {
            if (name == v.name && weight == v.weight) return true;
            else return false;
        }

        //stream output
        friend std::ostream &operator<<(std::ostream & os, const Vertex & ver) {
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
    };

}

#endif