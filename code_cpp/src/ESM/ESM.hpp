//file: mfa9Solver.h

/* header file for the equation solution manager (ESM) */


#ifndef ESM_HPP
#define ESM_HPP

#include <string>
#include <iostream>


#include "../GDS/GDS.hpp"


#include "Cluster_Data.hpp"

//#include "mfaTree.cpp"

//Constants
const float PI=3.141593;                 //PI

//Global variable declarations
List<int> keys;                          //records the keys pressed by the user
List<Edge> theImags;                     //theImaginary Constraints
List<std::string> vars;                       //holds the list of variable strings
List<Cluster> roots;                     //roots of the DRDAG
List<Cluster>* toSolverTrees;		 //points to SolverTrees
List<Cluster> DAGBackup;		 //Backup of the DRDAG
Graph graph0Backup;			 //Backup of graph0

Vertex testVertex1, testVertex2;         //sample vertices used in testing

std::string outFile="./Sketcher/testout.txt"; //path to output file
std::string varsToZero="";			 //list of all the zeroed variables in a set

int aOverlap;                            //an example constraint between two clusters
int equationCount;                       //the number of equations generated for a cluster
int withHeldCluster;                     //theCluster whose values are carried over in solving
int type1, type2;                        //used in the testEQ section
int startI;                              //starting point for int array read
int startF;                              //starting point for float array read
int currPosI;				 //current location in inputInts
int currPosF;				 //current location in inputFloats
int* inputInts;				 //global pointer to the int array
int* bifurAnswers;			 //answers to the current bifurs
int graphDimen;				 //dimension of the graph

float* inputFloats;                      //global pointer to the float array

char* inputChars;			 //global pointer to char array

bool toSolve;                            //stores a command to solve or to halt
bool first;                              //true if this is the first message to the sketcher
bool autoSolve;                          //automatically searches solution set if true
bool usingArrays;                        //true if the current data came from arrays
bool usedBifurs;			 //true if the bifurs have been placed

Graph gGraph1;				 //globalized graph1


//Function Headers
//For definitions and function documentation see the inidividual function definitions in mfa9Solver.cpp

//Printing Functions
void print(Graph &graph0, List<Cluster> &SolverTrees);
void print(Graph &graph0, Cluster &theCluster);
void printBifurcations(Cluster &theCluster);
void generateOutput(Graph &graph0, Cluster &theCluster, std::ostream &outfile, bool printNum);
void updateGraph(Graph &graph0, Cluster &theCluster);
void outputDRDAG(List<Cluster> &theCluster, std::ostream &output, bool first);
void outputFinState(Cluster &theCluster, int &startI, int*& theInts, int &count);

//Utility Functions
float getDistance(float x1, float y1, float x2, float y2);
float getAngle(float x1, float y1, float x2, float y2);
float getAngle(float theCos, float theSin);
int getChildIndexByName(Cluster &theCluster, int vName);
int getChildNameWithVertex(Cluster &theCluster, int theV);
bool inOriginalV(int Name, Cluster &theCluster);
void setValue(Vertex &theVertex, int index, float theValue);
// void getContainedChildList(Cluster &theCluster, int vName, List<int> &theList);
void setValue(Cluster &theCluster, int ID, float theValue, int index);
int getFreeDegIndex(char keyChar, Vertex &theVertex);
bool resetFinByClusterName(Cluster &theCluster, int name);
void resetTrees(Graph &graph0, List<Cluster> &SolverTrees);
void setValueReduction(Graph &graph0, Cluster &theCluster);
void fixCluster(Cluster &theCluster);
void fixTree(List<Cluster> &SolverTrees);
std::string processMatlab(std::string input);
void postProcessMatlab();
int countEdges(Graph &graph0, Cluster &theCluster);
ClustData& summCluster(Graph &graph0, Cluster &theCluster);
float findDist(Graph &graph0, Cluster &theCluster);
std::string getSpecialCaseEquations(Graph &graph0, Cluster &theCluster, int caseInt, std::string &outputString);
void getOverlapList(Graph graph0, Cluster &theCluster, List<int> &outputList, int child1 = 0, int child2 = 0 );
int buildGraphFromList(Graph &newGraph, Graph &graph0, List<int> &vertList);
void parseForExternalOverCons(Graph &graph0, List<Cluster> *SolverTrees);

//String Functions
std::string getSubString(std::string theString, int pos, int length);
std::string replaceAll(std::string toReplace, std::string theReplace, std::string replaceWith);
std::string getVarString();
std::string toString(long a);
std::string toString(int a);
std::string toString(double a);
std::string toString(float a);

//Intersection finders
void getIntersectionLineLine(Vertex &theVertex, float x1, float y1, float m1, float x2, float y2, float m2);
void getIntersectionLineCircle(List<Vertex> &thePoints, float x1, float y1, float m1, float x2, float y2, float r);
void getIntersectionCircleCircle(List<Vertex> &thePoints, float x1, float y1, float r1, float x2, float y2, float r2);

//Imaginary Constraint recognition functions
bool isValidPointIncidence(Vertex &thePoint, Vertex &theOther, int part1, int part2);
bool isValidLineIncidence(Vertex &theLine, Vertex &theOther, int part1, int part2);
bool isValidRayIncidence(Vertex &theRay, Vertex &theOther, int part1, int part2);
bool isValidLSIncidence(Vertex &theLS, Vertex &theOther, int part1, int part2);
bool isValidCircleIncidence(Vertex &theCircle, Vertex &theOther, int part1, int part2);
bool isValidArcIncidence(Vertex &theArc, Vertex &theOther, int part1, int part2);
bool isImaginary(Graph &graph0, Edge &theEdge);

//Bifurcation functions
bool isValidConstraint(Graph &graph0, Edge &theEdge, Cluster &theCluster);
bool validBifur(std::string theString);
void setBifurs(std::string filename, Graph graph0, Cluster& theCluster, bool usingfSolve);
void parseBifurString(Graph &graph0, Cluster &theCluster);
bool getNextBifurcation(Graph &graph0, Cluster &theCluster);
int selectBifurcation(Graph &graph0, Cluster &theCluster, bool &useFile);
void checkBifurs(Graph &graph0, Cluster &theCluster);

//Graph Update Functions
void rotate(Cluster &theCluster, Vertex &v0);
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex, int location);
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex);
void populateCluster(Graph &graph0, Cluster &theCluster);
void populateForest(Graph &graph0, List<Cluster> &SolverTrees);
void populateGraph(Graph &graph0, List<Cluster> &SolverTrees);
void updateGraph(Graph &graph0, Cluster &theCluster);

//Equation Generators
std::string getDistanceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, std::string theDist);
std::string getTangencyEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getIncidenceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getAngleEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getParallelEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getPerpendicularEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getDistance3DEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, std::string theDist);
int getEdgeCode(Edge &theEdge, Cluster &theCluster);
int getEdgeCodeDRPlanner(Edge &theEdge, Cluster &theCluster);
std::string switchString(Vertex &theVertex, int type, int shapeName, int clusterName, std::string toReplace, bool solveOrValue);
std::string getEquationBetweenClusters(Graph& graph0, Edge &theEdge, Cluster &theCluster);
std::string getEquationInCluster(Graph& graph0, Edge &theEdge, Cluster &theCluster);
std::string getEquation(Graph &graph0, Edge &theEdge, Cluster &theCluster);
std::string getEquation(Graph &graph0, Cluster &theCluster, std::ostream &inputFile);
std::string getSinCosConstraint();
std::string getOverlapConstraint(Graph &graph0, Cluster &theCluster, int
totalExtra);
std::string getLineConstraint(Graph &graph0, List<Cluster> &theChildren);
std::string getOriginEquations(Edge &theEdge, Edge &theEdge2, Graph &graph0, int clusterName, int clusterType);
std::string getRotationEquations(Cluster &theCluster);

//Solver Functions
void shellMaple();
void setWithHeldCluster(Cluster &theCluster);
int solveCluster(Graph &graph0, Cluster &theCluster, bool resolve);
int solveForest(Graph &graph0, List<Cluster> &theForest);

//Testing functions
void testEquations(Graph &graph0);
void getGraphFromFile(Graph &graph0, List<Cluster> &SolverTrees);

//Array IO Functions
void stringToArray(std::string theString, int*& theInts);
std::string readStringFromArray(int& start, int* theInts);
void vertexToArray(Vertex &theVertex, int*& theInts, float*& theFloats);
void readVertexFromArray(Vertex &theVertex, int &startI, int* theInts, int& startF, float* theFloats);
void edgeToArray(Edge &theEdge, int*& theInts, float*& theFloats);
void readEdgeFromArray(Edge &theEdge, int &startI, int* theInts, int &startF, float* theFloats);
void graphToArray(Graph &graph0, int*& theInts, float*& theFloats);
void readGraphFromArray(Graph &graph0, int &startI, int* theInts, int &startF, float* theFloats);
void clusterToArray(Cluster &theCluster, int*& theInts, float*& theFloats);
void readClusterFromArray(Cluster &theCluster, int &startI, int* theInts, int &startF, float* theFloats);
void outputArrays(int* theInts, float* theFloats);
void inputArrays(int*& theInts, float*& theFloats);
void saveState(Graph &graph0);
void saveState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, jint*& thejInts, jdouble*& thejDoubles);
void loadState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int &startI, jint* thejInts, int &startF, jdouble* thejDoubles, jchar *thejChars);
void setArraysForOutput(jint*& theJInts, jdouble*& theJDoubles);
void setArraysForInput(jint*& theJInts, jdouble*& theJDoubles);
void outputDRDAGToArray(List<Cluster> &theCluster, int& index, int*& theInts, bool first);
/*
void treeToArray(Node *Root, int*& theInts);
Node* readTreeFromArray(int*& start, int* theInts);
*/

//Things in UTU
bool checkEdge(Edge &theEdge, Graph &graph0, List<Cluster> &SolverTrees);
void deleteClusterWithEdge(Graph &graph0, List<Cluster> &SolverTrees, Edge &theEdge);
void Solver(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, jint* inputTheInts, jdouble* inputDouble, jchar* inputChar);

#endif
