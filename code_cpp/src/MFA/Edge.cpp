#include "Edge.hpp"


Edge::Edge(const Edge &edg)
{
      name=edg.name;
      type=edg.type;
      weight=edg.weight;
      value=edg.value;
      end1=edg.end1;
      end2=edg.end2;
      part1=edg.part1;
      part2=edg.part2;
      label=edg.label;
      scan=edg.scan;
      flow1=edg.flow1;
      flow2=edg.flow2;
      posFlow=edg.posFlow;
      predVertex=edg.predVertex;
}


Edge & Edge::operator = (const Edge &edg)
{
      if(this != &edg)
      {
         name=edg.name;
         type=edg.type;
         weight=edg.weight;
         value=edg.value;
         end1=edg.end1;
         end2=edg.end2;
         part1=edg.part1;
         part2=edg.part2;
         label=edg.label;
         scan=edg.scan;
         flow1=edg.flow1;
         flow2=edg.flow2;
         posFlow=edg.posFlow;
         predVertex=edg.predVertex;
      }
      return *this;
}

std::ostream& operator <<(std::ostream& os, const Edge& edg)
{
   os<<"Edg="<<edg.name<<",T="<<edg.type<<", W="<<edg.weight<<",V=";
   if(edg.value<-1)     //if (edg.value==-2.0)
     os<<"NA";          // Not Applicable
   else if(edg.value<0) //if (edg.value==-1.0)
     os<<"un";          //unknown
   else
     os<<edg.value;
   os<<", LS="<<edg.label<<edg.scan<<", PF="<<edg.posFlow<<", F1="<<edg.flow1
     <<", F2="<<edg.flow2<< ", Ends: "<<edg.end1<<'('<<edg.part1<<") "
     <<edg.end2<<'('<<edg.part2<<") PD="<<edg.predVertex;
   return os;
}
