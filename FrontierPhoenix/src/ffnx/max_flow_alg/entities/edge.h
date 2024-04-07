#ifndef ENTITIES_EDGE_H
#define ENTITIES_EDGE_H

#include <iostream>

namespace ffnx::mfa {

    class Edge {
    protected:
        int name = 0;             // name = Constraint ID
        int weight = 0;           // DOF is removed by the constraint

        int end1 = 0;         // ID of the vertex at end1
        int end2 = 0;             // ID of the vertex at end2
        int part1 = 0;            // part of shape involved in an incidence constraint
        // 0=arbitrary, 1=point1, 2=point2, 3=center
        int part2 = 0;            // same as part1

        int label = 0;            // 0 or 1 used for augmenting path
        int scan = 0;             // 0 or 1 used for augmenting path
        int flow1 = 0;            // flow to end1
        int flow2 = 0;            // flow to end2
        int posFlow = 0;          // possible flow to deliver to this edge
        int predVertex = 0;       // predecessor Vertex in the augmenting path

    public:

        void setName(int nm) { name = nm; }

        void setWeight(int wt) { weight = wt; }

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

        int returnWeight() { return weight; }

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

        //two edges are equal if their ID's are the same
        inline bool operator==(const Edge &e) {
            if (name == e.name) return true;
            else return false;
        }

        //stream output
        friend std::ostream &operator<<(std::ostream &os, const Edge &edg){
            os << "Edg=" << edg.name << ", W=" << edg.weight;

            os << ", LS=" << edg.label << edg.scan << ", PF=" << edg.posFlow << ", F1=" << edg.flow1
               << ", F2=" << edg.flow2 << ", Ends: " << edg.end1 << '(' << edg.part1 << ") "
               << edg.end2 << '(' << edg.part2 << ") PD=" << edg.predVertex;
            return os;
        }
    };

}

#endif