#ifndef VERTEX_HPP
#define VERTEX_HPP


#include <iostream>

#include "List.hpp"



class Vertex
{
private:
   int name;           // unique object ID
   int type;           //point=0,line=1,ray=2,LSeg=3,circle=4,arc=5,cluster=-1,point3D=6
   int weight;         // degree of freedom
   float degValues[9]; // solved values of this object
   int freeDegs[9];    //if freeDegs[i]=0, solver can change degValues[i]
                       //if freeDegs[i]=1, degValues[i] is changed by solver
                       //if freeDegs[i]=-1, degValues[i] is fixed by user
                       //if freeDegs[i]=-2, degValues[i] is NA(Not Applicable) or not defined
    

    //Table shows the different types of degrees of freedom used for each object type and
    //the degvalue location where it is stored
    /****************************************************************************\
    *              point line   Ray   LineSeg circle  arc      cluster  point3D  *
    *  degValue[0]   x    x1     x1     x1      x      x        x-sft      x     *
    *  degValue[1]   y    y1     y1     y1      y      y        y-sft      y     *
    *  degValue[2]   -    x2     x2     x2      r      r        z-sft      z     *
    *  degValue[3]   -    y2     y2     y2      -    angle    rotx-cos     -     *
    *  degValue[4]   -  slope   cos   slope     -  start_cos  rotx-sin     -     *
    *  degValue[5]   -          sin   length    -  start_sin  roty-cos     -     *
    *  degValue[6]   -    -      -      -       -   end_cos   roty-sin     -     *
    *  degValue[7]   -    -      -      -       -   end_sin   rotz-cos     -     *
    *  degValue[8]   -    -      -      -       -      -      rotz-sin     -     *
    \****************************************************************************/

   List<int> incid;  // List of the ID's of the inicident edges
   int nIncid;       // number of incident edges
   int label;        // 0 or 1 used for augmenting path
   int scan;         // 0 or 1 used for augmenting path
   int frozen;       // If frozen=1, we don't distribute; If frozen=2, frozen forever
   int posFlow;      // possible flow that could be delivered to this vertex
   int existFlow;    // flow already absorbed by this vertex
   int predEdge;     // predecessor edge in the augmenting path
   int depth;        // depth==0 for original vertex
   bool ignore;      // when true vertex is ignored with output from solver

public:

   List<int> leftEndIncid; //points with incidence constraints on the left end of the line segment
   List<int> rightEndIncid; //same as above, but right end

   //Constructors
   
   //default initializes all values to zero
   Vertex();

   //sets the ID of vertex to theName, and theType to theType
   Vertex(int theName, int theType);

   //copy constructor
   Vertex(const Vertex &);

   void setName(int nm) { name=nm; }
   void setType(int tp) { type=tp; }
   void setWeight(int wt) { weight=wt; }
   void setPosFlow(int fl) { posFlow=fl; }
   void setEstFlow(int fl) { existFlow=fl; }
   void setPredEdg(int e) { predEdge=e; }
   void setDepth(int d) { depth=d; }
   void setIgnore(bool t) { ignore=t; }
   void notDefinedValue(int ix) { freeDegs[ix]=-2; }
   void initialValue(int ix, float value, int free) {
       degValues[ix]=value;
       freeDegs[ix]=free;
   }
   bool setValues(int ix, float value) {
      if(ix>=9 || freeDegs[ix] < 0 )  // solver cannot change degValues[ix]
         return false;
      freeDegs[ix]=1;
      degValues[ix]=value;
      return true; //!!!!!!!!! added this line
   }
   void appendIncid(int edge_name) { incid.append(edge_name); nIncid++; }
   void setLabel(int lb) { label=lb; }
   void setScan(int sc) { scan=sc; }
   void freeze() { if(frozen==0) frozen=1; } // can be defrosted
   void freezeForever() { frozen=2; }        // can't be defrosted
   void incrsWt(int wt) { weight=weight+wt; }
   void incrsEstFlow(int fl) { existFlow=existFlow+fl; }
   void incrsPosFlow(int fl) { posFlow=posFlow+fl; }
   void delIncid(int e) { if(incid.hasElem(e)) {incid.deleteElem(e);nIncid--;}}
   void defrost() { if(frozen==1) frozen=0; }  // if frozen=2, it can't be defrosted
   void unFrozen() {frozen=0;}
   int returnName() { return name; }
   int returnType() { return type; }
   int returnWeight() { return weight; }
   bool returnIgnore() { return ignore; }
   bool hasFreeDeg() {
       for(int i=0; i<9; i++) if(freeDegs[i]>=0) return true;
       return false;
   }
   int returnFreeDegs(int index) { return freeDegs[index]; }
   float returnDegValueByName(int index) { return degValues[index]; }
   int returnIncid(int index) { return incid.retrieve(index); }
   List<int>& returnIncid() {return incid;}
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
   Vertex & operator = (const Vertex &ver);

   //two vertices are equal if they have the same name and weight
   inline bool operator == (const Vertex &v) {
      if(name==v.name&&weight==v.weight) return true;
      else return false;
   }

   //stream output
   friend std::ostream& operator <<(std::ostream&, const Vertex&);
};




//class that stores one set of degvalues exactly as described in Class Vertex above
class DegValues
{

private:

   float degValues[9];

public:

   DegValues() {
                  int i;

                  for(i=0;i<9;i++) degValues[i]=0;
               }
   void setValue(int i, float value) { degValues[i]=value; }
   float returnValue(int i) { return degValues[i]; }
   DegValues & operator = (const DegValues &deg);
   //stream output
   friend std::ostream& operator <<(std::ostream&, const DegValues&);
};


#endif