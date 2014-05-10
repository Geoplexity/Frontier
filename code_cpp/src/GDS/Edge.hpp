#ifndef EDGE_HPP
#define EDGE_HPP


#include <iostream>



class Edge
{
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
   Edge() { name=0; type=0; weight=0; value=0.0; end1=0; end2=0; part1=0;part2=0;
            label=0; scan=0; flow1=0; flow2=0; posFlow=0; predVertex=0; }

   //copy constructor
   Edge(const Edge &);


   void setName(int nm) { name=nm; }
   void setType(int tp) { type=tp; }
   void setWeight(int wt) { weight=wt; }
   void setValue(float vl) { value=vl; }
   void setEnd(int i, int v) { if(i==0) end1=v; else end2=v; }
   void setPart(int i, int p) { if(i==0) part1=p; else part2=p; }
   void setLabel(int lb) { label=lb; }
   void setScan(int sc) { scan=sc; }
   void setFlow1(int fl) { flow1=fl; }
   void setFlow2(int fl) { flow2=fl; }
   void setPosFlow(int fl) { posFlow=fl; }
   void setPredVer(int v) { predVertex=v; }
   void incrsWt(int wt) { weight=weight+wt; }
   void incrsFlow1(int fl) { flow1=flow1+fl; }
   void incrsFlow2(int fl) { flow2=flow2+fl; }
   void incrsPosFlow(int fl) { posFlow=posFlow+fl; }
   void makeEmpty() { name=0; weight=0; }
   int returnName() { return name; }
   int returnType() { return type; }
   int returnWeight() { return weight; }
   float returnValue() { return value; }
   int returnEnd1() { return end1; }
   int returnEnd2() { return end2; }
   int returnPart1() { return part1; }
   int returnPart2() { return part2; }
   int otherEnd(int v) { if(v==end1) return end2; else return end1; }
   int returnLabel() { return label; }
   int returnScan() { return scan; }
   int returnFlow1() { return flow1; }
   int returnFlow2() { return flow2; }
   int returnPosFlow() { return posFlow; }
   int returnPredVer() { return predVertex; }
 
   //= operator
   Edge & operator = (const Edge &edg);
   
   //two edges are equal if their ID's are the same
   inline bool operator == (const Edge &e)
   {
      if(name==e.name) return true;
      else return false;
   }

   //stream output
   friend std::ostream& operator <<(std::ostream&, const Edge&);
};



#endif