#include "Vertex.hpp"


Vertex::Vertex()
{
   name=0;
   type=0;
   weight=0;
   nIncid=0;
   label=0;
   scan=0;
   frozen=0;
   posFlow=0;
   existFlow=0;
   predEdge=0;
   depth=0;
   ignore=false;
   for(int i=0; i<9; i++) {
       degValues[i]=0;
       freeDegs[i]=0;
   }
   leftEndIncid.makeEmpty();
   rightEndIncid.makeEmpty();
}

Vertex::Vertex(int theName, int theType)
{
   name=theName;
   type=theType;
   weight=0;
   nIncid=0;
   label=0;
   scan=0;
   frozen=0;
   posFlow=0;
   existFlow=0;
   predEdge=0;
   depth=0;
   ignore=false;
   for(int i=0; i<9; i++) {
       degValues[i]=0;
       freeDegs[i]=0;
   }
   leftEndIncid.makeEmpty();
   rightEndIncid.makeEmpty();
}

Vertex::Vertex(const Vertex &ver)
{
   name=ver.name;
   type=ver.type;
   weight=ver.weight;
   nIncid=ver.nIncid;
   label=ver.label;
   scan=ver.scan;
   frozen=ver.frozen;
   posFlow=ver.posFlow;
   existFlow=ver.existFlow;
   predEdge=ver.predEdge;
   depth=ver.depth;
   incid=ver.incid;
   ignore=ver.ignore;
   for(int i=0;i<9;i++) {
      freeDegs[i] = ver.freeDegs[i];
      degValues[i] = ver.degValues[i];
   }
}

Vertex & Vertex::operator = (const Vertex &ver)
{
   if(this != &ver)
   {
      name=ver.name;
      type=ver.type;
      weight=ver.weight;
      nIncid=ver.nIncid;
      label=ver.label;
      scan=ver.scan;
      frozen=ver.frozen;
      posFlow=ver.posFlow;
      existFlow=ver.existFlow;
      predEdge=ver.predEdge;
      depth=ver.depth;
      incid=ver.incid;
      ignore=ver.ignore;
      for(int i=0;i<9;i++) {
         freeDegs[i] = ver.freeDegs[i];
         degValues[i] = ver.degValues[i];
      }
      leftEndIncid=ver.leftEndIncid;
      rightEndIncid=ver.rightEndIncid;
   }
   return *this;
}

std::ostream& operator <<(std::ostream& os, const Vertex& ver)
{
   int i;
   os<<"Ver="<<ver.name<<",T="<<ver.type<<", W="<<ver.weight<<", d="<<ver.depth
     <<", LSFI="<<ver.label<<ver.scan<<ver.frozen;
   if(ver.ignore) os<<1;
   else os<<0;
     
   os <<", PF="<<ver.posFlow<<", EF="<<ver.existFlow;
   os<<", edge:";
   if(ver.nIncid>0)
      for(i=1;i<=ver.nIncid;i++)
        os<<(ver.incid).retrieve(i)<<' ';
   os<<"PD="<<ver.predEdge;

    
   os << " leftEnds:";
   int len = ver.leftEndIncid.returnLen();
   for(i = 1; i <=len; i++)
    os << ver.leftEndIncid.retrieve(i) << ' ';
   os << " rightEnds:";
   len = ver.rightEndIncid.returnLen();
   for(i = 1; i <=len; i++)
    os << ver.rightEndIncid.retrieve(i) << ' ';

   os<<" DOF="; 
   for(i=0;i<9;i++) {
      if(*(ver.freeDegs+i)==0)
          os<<"un";          //unknown
      else if(*(ver.freeDegs+i)==-2)
          os<<"NA";          //Not Applicable (not defined)
      else
          os<<*(ver.degValues+i);
      if(i<7) os<<' ';
   }
   return os;
}






DegValues & DegValues::operator = (const DegValues &deg)
{
      int i;

      if( this != &deg)
      {
         for(i=0;i<9;i++)
            degValues[i]=deg.degValues[i];

      }
      return *this;
}

std::ostream& operator <<(std::ostream& os, const DegValues& deg)
{
    for(int i=0;i<9;i++)
        os << deg.degValues[i] << "    ";
    os << std::endl;
    return os;
}