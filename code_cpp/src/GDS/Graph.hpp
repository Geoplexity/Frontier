#ifndef GRAPH_HPP
#define GRAPH_HPP

// #include "../../ext/JNI/jni.h"

#include "List.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"


const int K_FOR_2D = 4;
const int K_FOR_3D = 7;



class Graph
{
private:
   int depth;           // depth of the largest cluster in the graph

   //see separate pseudocode and paper documentation of explanation
   void augment(Vertex *, Edge *);

public:
   // DO NOT WANT TO LEAVES THESE PUBLIC
   int NumVert;         // # of vertices
   List<Vertex> vertices;   // list of the vertices in the graph

   int NumEdge;         // # of edges
   List<Edge> edges;        // list of the edges in the graph

   int dimension;       // dimension=2 or 3





   //constructor
   Graph() { dimension=3; NumVert=0; NumEdge=0; depth=0; }

   void setDimen(int dimen) { dimension=dimen; }
   void setDepth(int d) { depth=d; }
   void setVlist(List<Vertex> &vl) { NumVert=vl.returnLen(); vertices=vl; }
   void setElist(List<Edge> &el) {  NumEdge=el.returnLen(); edges=el; }

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


   void appendVertex(Vertex &ver) { vertices.append(ver); NumVert++; }
   void appendEdge(Edge &edg) { edges.append(edg); NumEdge++; }

   void delAllVer() { vertices.makeEmpty(); NumVert=0; }
   void delAllEdg() { edges.makeEmpty(); NumEdge=0; }
   void makeEmpty() { delAllVer(); delAllEdg(); }
   void delVer(Vertex &v)
   {
      if( (NumVert>0) && hasVert(v.returnName()) )
      {
         vertices.deleteElem(v);
         NumVert--;
      }
   }
   void delEdg(Edge &e)
   {
      if( (NumEdge>0) && hasEdge(e.returnName()) )
      {
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

   List<Vertex> & returnVlist() { return vertices; }
   List<Edge>   & returnElist() { return edges; }

   Vertex & returnVertByIndex(int i) { return vertices.retrieve(i); }
   Edge   & returnEdgeByIndex(int i) { return edges.retrieve(i); }
   Vertex & returnVertByName(int vName);
   Edge   & returnEdgeByName(int eName);
   Edge   & returnEdgeByEnds(int v1, int v2);


   // return a Edge or Vertex point to the edge or vertex with the given ID
   Vertex * VertAddr(int vName);
   Edge   * EdgeAddr(int eName);



   // // reads data from the arrays sent by the sketcher
   // int sketchInput(int &idx, jint *inputData, int &indxDbl, jdouble *dbleData); //from sketch to graph



   // used for testing generates a random graph
   void randomGraph();

   //stream output
   void output(std::ostream&);

   //set all of the labels in both the edges and the vertices to the unlabeled state, 0
   void removeLabels();

   // returns the flow on an edge back to undistributed flow on vertices
   void restoreFlow(Edge *edgep);

   // see the attached paper for complete documentation
   int distribute0(Edge &edge, std::ostream &file2);

   // returns the difference between the total weight of the edges and the weight of the vertices
   // less the number of degrees of freedom of a rigid object in space, 2D = 3, 3D = 6
   int constrainDegree();





   ///////////////////////////
   // came from DRP originally

   // copy a graph and return K value
   int copyInto(Graph &g1)
   {
       int i, nVer, nEdg, dimen;

       g1.delAllVer();
       g1.delAllEdg();

       dimen=this->returnDimen();
       g1.setDimen(dimen);

       nVer=this->returnNumVer();
       nEdg=this->returnNumEdg();

       for(i=1;i<=nVer;i++)
          g1.appendVertex(this->returnVertByIndex(i));

       for(i=1;i<=nEdg;i++)
          g1.appendEdge(this->returnEdgeByIndex(i));

       if(dimen==3) return K_FOR_3D;  //K=7 for 3-D problem
       else return K_FOR_2D;          //K=4 for 2-D problem
   }
};


#endif
