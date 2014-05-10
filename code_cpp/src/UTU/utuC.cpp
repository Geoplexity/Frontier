/*The UTU (universal transfer unit)-- main controller program for
FRONTIER*/


#include <jni.h>
#include "utuJava.h"
#include <iostream.h>
#include <fstream.h>
#include <string>
#include <math.h>
#include <values.h>

int nextEdgeName=1, nextVerName=1, singleVertex=1, treeFlag;


#include "cData.h"
#include "mfa1List.h"
#include "mfa2Clas.h"
#include "mfa10Solver.h"
#include "mfa3Prnt.cpp"
#include "mfa4Grop.cpp"
#include "mfa5GetC.cpp"
#include "mfa6Dist.cpp"
#include "mfa7DRTree.cpp"
#include "mfa8Algo.cpp"
#include "mfaTree.cpp"

the_Tree Tree;

#include "mfa9Solver.cpp"

//tc4.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6, s7, s8;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(7);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(7);
	ori.append(7);
	s7.formCl(core, fro, ie, oe, ori);
	
	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
		fro.append(3);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	//depth 2
	Cluster tetra1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(14);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(7);
	fro.append(1);
	fro.append(3);
	fro.append(7);
	tetra1.formCl(core, fro, ie, oe, ori);
	tetra1.children.append(s7);
	tetra1.children.append(tri1);

	//triangle
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(15);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);
		fro.append(1);
		fro.append(5);
		fro.append(6);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s5);
	tri2.children.append(s6);
	//depth 2
	Cluster tetra2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(16);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);		
		ori.append(7);
	fro.append(1);
	fro.append(5);
	fro.append(7);
	tetra2.formCl(core, fro, ie, oe, ori);
	tetra2.children.append(s7);
	tetra2.children.append(tri2);

	//triangle
	Cluster tri3;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(17);
	core.setDepth(1);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(4);
		fro.append(3);
		fro.append(5);
		fro.append(4);
	tri3.formCl(core, fro, ie, oe, ori);
	tri3.children.append(s3);
	tri3.children.append(s5);
	tri3.children.append(s4);
	//depth 2
	Cluster tetra3;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(18);
	core.setDepth(2);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(4);
		ori.append(7);
		fro.append(3);
		fro.append(5);
		fro.append(7);
	tetra3.formCl(core, fro, ie, oe, ori);
	tetra3.children.append(s7);
	tetra3.children.append(tri3);
	//depth 3
	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(19);
	core.setDepth(3);
	core.setType(0);
	for(int i = 1; i <=7; i++)
		ori.append(i);
	fro.append(1);
	fro.append(4);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tetra1);
	final.children.append(tetra2);	
	final.children.append(tetra3);	
  
      solverTree.append(final);	

}
//create a new solverTree to test ESM module
/*****************
//esm2.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6, s7, s8;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);

	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
		fro.append(3);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	//depth 2
	Cluster tetra1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(14);
	core.setDepth(2);
	core.setType(0);
	for(int i = 1; i <=4; i++)
		ori.append(i);
	fro.append(1);
	fro.append(4);
	tetra1.formCl(core, fro, ie, oe, ori);
	tetra1.children.append(s4);
	tetra1.children.append(tri1);

	//triangle
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(15);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);
		fro.append(1);
		fro.append(5);
		fro.append(6);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s5);
	tri2.children.append(s6);
	//depth 2
	Cluster tetra2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(16);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);		
		ori.append(4);
	fro.append(1);
	fro.append(4);
	tetra2.formCl(core, fro, ie, oe, ori);
	tetra2.children.append(s4);
	tetra2.children.append(tri2);
	//depth 3
	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(17);
	core.setDepth(3);
	core.setType(0);
	for(int i = 1; i <=6; i++)
		ori.append(i);
	fro.append(1);
	fro.append(4);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tetra1);
	final.children.append(tetra2);	
  
      solverTree.append(final);	

}
*******************************/
/***********

//esm3.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6, s7, s8;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);
	core = graph1.returnVertByName(7);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(7);
	ori.append(7);
	s7.formCl(core, fro, ie, oe, ori);

	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
		fro.append(3);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	//depth 2
	Cluster tetra1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(14);
	core.setDepth(2);
	core.setType(0);
	for(int i = 1; i <=4; i++)
		ori.append(i);
	fro.append(2);
	fro.append(4);
	tetra1.formCl(core, fro, ie, oe, ori);
	tetra1.children.append(s4);
	tetra1.children.append(tri1);
	//depth 3
	Cluster dep;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(15);
	core.setDepth(3);
	core.setType(0);
	for(int i = 1; i <=5; i++)
		ori.append(i);
	fro.append(2);
	fro.append(4);
	fro.append(3);
	fro.append(5);
	dep.formCl(core, fro, ie, oe, ori);
	dep.children.append(s5);
	dep.children.append(tetra1);

	//depth 4
	Cluster depmore;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(16);
	core.setDepth(4);
	core.setType(0);
	for(int i = 1; i <=6; i++)
		ori.append(i);
	fro.append(6);
	fro.append(5);
	depmore.formCl(core, fro, ie, oe, ori);
	depmore.children.append(s6);
	depmore.children.append(dep);

	//triangle
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(17);
	core.setDepth(1);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(7);
		fro.append(3);
		fro.append(5);
		fro.append(7);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s3);
	tri2.children.append(s5);
	tri2.children.append(s7);
	//depth 2
	Cluster tetra2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(18);
	core.setDepth(2);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(7);
		ori.append(6);
		fro.append(3);
		fro.append(5);
		fro.append(6);
	tetra2.formCl(core, fro, ie, oe, ori);
	tetra2.children.append(s6);
	tetra2.children.append(tri2);
	//depth 3
	Cluster dep2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(19);
	core.setDepth(3);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(7);
		ori.append(6);
		ori.append(2);
		fro.append(3);
		fro.append(2);
		fro.append(6);
	dep2.formCl(core, fro, ie, oe, ori);
	dep2.children.append(s2);
	dep2.children.append(tetra2);
	//depth 4
	Cluster depmore2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(20);
	core.setDepth(4);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(7);
		ori.append(6);
		ori.append(2);
		ori.append(4);
		fro.append(3);
		fro.append(2);
		fro.append(4);
	depmore2.formCl(core, fro, ie, oe, ori);
	depmore2.children.append(s4);
	depmore2.children.append(dep2);
	//depth 3
	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(21);
	core.setDepth(5);
	core.setType(0);
	for(int i = 1; i <=7; i++)
		ori.append(i);
	fro.append(1);
	fro.append(7);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(depmore);
	final.children.append(depmore2);	
  
      solverTree.append(final);	

}
**********/

/**************
//esm4.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);

	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
		fro.append(3);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	//depth 2
	Cluster tetra1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(14);
	core.setDepth(2);
	core.setType(0);
	for(int i = 1; i <=4; i++)
		ori.append(i);
	fro.append(1);
	fro.append(4);
	tetra1.formCl(core, fro, ie, oe, ori);
	tetra1.children.append(s4);
	tetra1.children.append(tri1);

	//triangle
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(15);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);
		fro.append(1);
		fro.append(5);
		fro.append(6);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s5);
	tri2.children.append(s6);
	//depth 2
	Cluster tetra2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(16);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(6);		
		ori.append(3);
	fro.append(1);
	fro.append(4);
	tetra2.formCl(core, fro, ie, oe, ori);
	tetra2.children.append(s3);
	tetra2.children.append(tri2);
	//depth 3
	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(17);
	core.setDepth(3);
	core.setType(0);
	for(int i = 1; i <=6; i++)
		ori.append(i);
	fro.append(1);
	fro.append(4);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tetra1);
	final.children.append(tetra2);	
  
      solverTree.append(final);	

}
**************/
/**********
//esm6.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(7);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(7);
	ori.append(7);
	s7.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(8);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(8);
	ori.append(8);
	s8.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(9);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(9);
	ori.append(9);
	s9.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(10);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(10);
	ori.append(10);
	s10.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(11);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(11);
	ori.append(11);
	s11.formCl(core, fro, ie, oe, ori);


	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(6);
		fro.append(1);
		fro.append(2);
		fro.append(6);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s6);
	
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(12);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(7);
		ori.append(3);
		fro.append(1);
		fro.append(7);
		fro.append(3);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s7);
	tri2.children.append(s3);
	
	Cluster tri3;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(13);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(4);
		ori.append(8);
		fro.append(1);
		fro.append(4);
		fro.append(8);
	tri3.formCl(core, fro, ie, oe, ori);
	tri3.children.append(s1);
	tri3.children.append(s4);
	tri3.children.append(s8);
	
	Cluster tri4;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(14);
	core.setDepth(1);
	core.setType(0);
		ori.append(4);
		ori.append(9);
		ori.append(3);
		fro.append(4);
		fro.append(9);
		fro.append(3);
	tri4.formCl(core, fro, ie, oe, ori);
	tri4.children.append(s4);
	tri4.children.append(s9);
	tri4.children.append(s3);
	
	Cluster tri5;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(15);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(10);
		fro.append(1);
		fro.append(5);
		fro.append(10);
	tri5.formCl(core, fro, ie, oe, ori);
	tri5.children.append(s1);
	tri5.children.append(s5);
	tri5.children.append(s10);
	
	Cluster tri6;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(16);
	core.setDepth(1);
	core.setType(0);
		ori.append(3);
		ori.append(5);
		ori.append(11);
		fro.append(3);
		fro.append(5);
		fro.append(11);
	tri6.formCl(core, fro, ie, oe, ori);
	tri6.children.append(s3);
	tri6.children.append(s5);
	tri6.children.append(s11);
	
	Cluster tetra1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(17);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(6);
		ori.append(7);
		fro.append(1);
		fro.append(2);
		fro.append(3);
	tetra1.formCl(core, fro, ie, oe, ori);
	tetra1.children.append(tri1);
	tetra1.children.append(tri2);

	Cluster tetra2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(18);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(4);
		ori.append(3);
		ori.append(8);
		ori.append(9);
		fro.append(1);
		fro.append(3);
		fro.append(4);
	tetra2.formCl(core, fro, ie, oe, ori);
	tetra2.children.append(tri3);
	tetra2.children.append(tri4);

	Cluster tetra3;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(19);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(5);
		ori.append(3);
		ori.append(10);
		ori.append(11);
		fro.append(1);
		fro.append(3);
		fro.append(5);
	tetra3.formCl(core, fro, ie, oe, ori);
	tetra3.children.append(tri5);
	tetra3.children.append(tri6);

	Cluster dep;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(20);
	core.setDepth(3);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(4);
		ori.append(6);
		ori.append(7);
		ori.append(8);
		ori.append(9);
		fro.append(1);
		fro.append(3);
		fro.append(4);
	dep.formCl(core, fro, ie, oe, ori);
	dep.children.append(tetra1);
	dep.children.append(tetra2);

	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(21);
	core.setDepth(4);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(4);
		ori.append(5);
		ori.append(6);
		ori.append(7);
		ori.append(8);
		ori.append(9);
		ori.append(10);
		ori.append(11);

		fro.append(5);
		fro.append(3);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tetra3);
	final.children.append(dep);

  
      solverTree.append(final);	

}
**************/
/*******
//esm7.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5, s6;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);

	core = graph1.returnVertByName(6);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(6);
	ori.append(6);
	s6.formCl(core, fro, ie, oe, ori);

	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(12);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(4);
		ori.append(5);
		fro.append(1);
		fro.append(4);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s4);
	tri2.children.append(s5);
	
	Cluster tri3;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(13);
	core.setDepth(1);
	core.setType(0);
		ori.append(2);
		ori.append(4);
		ori.append(6);
		fro.append(2);
		fro.append(4);
	tri3.formCl(core, fro, ie, oe, ori);
	tri3.children.append(s2);
	tri3.children.append(s4);
	tri3.children.append(s6);
	

	

	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(21);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(4);
		ori.append(5);
		ori.append(6);

		fro.append(5);
		fro.append(3);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tri1);
	final.children.append(tri2);
	final.children.append(tri3);

  
      solverTree.append(final);	

}
*****/

/*********
//esm7b.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	cout << "create my own trees" << endl;
	List<Edge> ie;
	List<Edge> oe;
	List<int> fro;
	List<int> ori;
	Vertex core;
	Cluster s1, s2, s3, s4, s5;
	
	core = graph1.returnVertByName(1);
	fro.append(1);
	ori.append(1);
	s1.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(2);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(2);
	ori.append(2);
	s2.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(3);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(3);
	ori.append(3);
	s3.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(4);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(4);
	ori.append(4);
	s4.formCl(core, fro, ie, oe, ori);
	
	core = graph1.returnVertByName(5);
	fro.makeEmpty();
	ori.makeEmpty();
	fro.append(5);
	ori.append(5);
	s5.formCl(core, fro, ie, oe, ori);


	//triangle
	Cluster tri1;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(11);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		fro.append(1);
		fro.append(2);
	tri1.formCl(core, fro, ie, oe, ori);
	tri1.children.append(s1);
	tri1.children.append(s2);
	tri1.children.append(s3);
	
	Cluster tri2;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(12);
	core.setDepth(1);
	core.setType(0);
		ori.append(1);
		ori.append(4);
		ori.append(5);
		fro.append(1);
		fro.append(4);
	tri2.formCl(core, fro, ie, oe, ori);
	tri2.children.append(s1);
	tri2.children.append(s4);
	tri2.children.append(s5);
	


	

	Cluster final;
	fro.makeEmpty();
	ori.makeEmpty();
	core.setName(21);
	core.setDepth(2);
	core.setType(0);
		ori.append(1);
		ori.append(2);
		ori.append(3);
		ori.append(4);
		ori.append(5);


		fro.append(5);
		fro.append(3);
	final.formCl(core, fro, ie, oe, ori);
	final.children.append(tri1);
	final.children.append(tri2);

  
      solverTree.append(final);	

}
********/

JNIEXPORT void JNICALL Java_utuJava_utuC(JNIEnv *env,jobject obj,jintArray arrInt,jdoubleArray arrDbl, jcharArray arrChar)
{
   ofstream outf("utuc.out", ios::app);
   outf<<"OPEN UTU"<<endl;
   
   // Get int and double arrays from Java
   jsize intLen = env->GetArrayLength(arrInt);
   jint *dataInt = env->GetIntArrayElements(arrInt, 0);

   outf<<"INTS"<<endl;

   jsize dblLen = env->GetArrayLength(arrDbl);
   jdouble *dataDouble = env->GetDoubleArrayElements(arrDbl, 0);

   outf<<"DOUBLES"<<endl;

   jsize strLen = env->GetArrayLength(arrChar);
   jchar *dataChar = env->GetCharArrayElements(arrChar, 0);

   outf<<"CHARS"<<endl;

//   const char* equ = env->GetStringUTFChars(equations, NULL);

 //  for(int i=0; i<100; i++)
   //   outf<<dataChar[i]<<endl;

//   outf<<"equation: "<<dataChar[0]<<" "<<strLen<<endl;
   
      
   Graph graph0;
   List<Cluster> SolverTrees;
   List<Cluster> DRTrees;

   ofstream outfile1; // output file
   ofstream outfile2; // detail file

   outfile1.open("mfa.out");
   outfile2.open("detmfa.out");
                        
   int startGroup, startI, startF;
   int type, name, numEnds, theEnd;
   double value;

   ofstream dataDump;

   int i, j, length;

   Edge newEdge, *theEdge;

   Graph graph1;
  
   int flag=dataInt[0];
   treeFlag=dataInt[1];
   treeFlag=1;
   int deleteEdgeName;
   bool autoS;

   autoS=false;

   outf<<"*FLAG: "<<flag<<endl;

   if(flag>10)
   {
      flag=flag-10;
      autoS=true;
   }

   //the_Tree Tree(treeFlag);
   
   switch(flag)
   {
    // case 1:
    //     dataInt[0]=2;
    //     break;
    case 6:
	case 7:
	case 0:         // Initialize graph0 and DR_Trees based on input file (utu.bin or utu.txt)
                        startI=2;
			startF=0;

			//the_Tree Tree(treeFlag);
			outf << "Tree Flag in utuC.cpp: " << treeFlag << endl;
			Tree.build_Tree(treeFlag); //build the Tree here

      			startGroup=graph0.sketchInput(startI, dataInt, startF, dataDouble);

                        outf<<"StartGroup: "<<startGroup<<endl;

			graph0.output(outf);

			outf<<"original graph:"<<endl;
      			graph0.output(outfile1);
			graph0.output(outfile2);

                        for(i=startGroup;i<startGroup+50; i++)
                        outf<<"data "<<startGroup<<" "<<dataInt[i]<<endl;


			dataDump.open("dataDump.txt");

                        for(i=startGroup;i<intLen;i++)
                                dataDump<<dataInt[i]<<endl;
                        
                        dataDump.close();			

      			// Initialize the DRTrees structure based on input data
      			getGroups(dataInt, graph0, startGroup, DRTrees);

                        print(graph0, DRTrees);
			
			outf<<"Groups read"<<endl;

      			copyG(graph0, graph1);  // copy Graph from graph0 to graph1
      			graph1.simplify();      // merge multi-edges and remove zero-weight edges

			outf<<"Graph Copied"<<endl;
	
			outf<<"Dimension="<<graph0.returnDimen()<<endl;
			graph0.output(outf);

			outf<<"SolverTrees called"<<endl;

      			// MFA Algorithm: input are graph0, DR_Trees; output is SolverTrees
      			SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			fixTree(SolverTrees);

    			outf<<"SolverTrees returned"<<endl;

			//the_Tree Tree(treeFlag);

			break;

	case 3:		startI=2;
			startF=1;	
		
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble, dataChar);
			
			theEdge=graph0.EdgeAddr(dataInt[startI++]);
		
			value=dataDouble[startF++];

			outf<<"Value"<<value<<endl;
			
			for(i=startF;i<startF+10;i++)
			   outf<<i<<" "<<dataDouble[i]<<endl;

			theEdge->setValue((float) value);
			
			length=SolverTrees.returnLen();

			for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd1());
                        for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd2());
			
                        dataInt[0]=0;
			break;

	case 4: 	outf<<"Start Add"<<endl;
			startI=2;
			startF=1;   
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble, dataChar);

			outf<<"State Loaded"<<endl;

			type=dataInt[startI++];
			name=dataInt[startI++];
			numEnds=dataInt[startI++];

			newEdge.setName(name);
			newEdge.setType(type);
			newEdge.setWeight(1);			

			for(i=0;i<numEnds;i++)
			{
			   theEnd=dataInt[startI++];
			   newEdge.setEnd(i,theEnd);
			   graph0.VertAddr(theEnd)->appendIncid(name);
			   if(type<=1 || type==4)
			     newEdge.setPart(i, dataInt[startI++]);
			   else newEdge.setPart(i, 0);
			}
		
			if(type==1 && newEdge.returnPart1()==0 && newEdge.returnPart2()==0)
			  newEdge.setWeight(0);
                        if(type==1 && newEdge.returnPart1()!=0 && newEdge.returnPart2()!=0)
                          newEdge.setWeight(2);
			
			if(type==0 || type==4)
			  newEdge.setValue(dataDouble[startF++]);
			else newEdge.setValue(-2.0);

			graph0.appendEdge(newEdge);

			if(checkEdge(newEdge, graph0, SolverTrees))
                        {
			  dataInt[0]=0;
			  break;
		        }

			copyG(graph0, graph1);
                        graph1.simplify();

                        resetTrees(graph0, DRTrees);

			singleVertex=10;
			
                        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			outf<<"AFTER ADDING CONSTRAINT"<<endl;

			graph0.output(outf);

			print(graph0, SolverTrees);
			
			dataInt[0]=0;

                        break;	
		
    case 5:	
        startI=2;
        startF=1;
        loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble, dataChar);
        graph0.output(outf);
        print(graph0, DRTrees);
        deleteEdgeName=dataInt[startI++];
        deleteClusterWithEdge(graph0, SolverTrees, graph0.returnEdgeByName(deleteEdgeName));
        graph0.delEdgeByName(deleteEdgeName);
        dataInt[0]=0;
        copyG(graph0, graph1);
        graph1.simplify();
        resetTrees(graph0, DRTrees);
        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);
        outf<<"AFTER REMOVING CONSTRAINT"<<endl;
        graph0.output(outf);
        print(graph0, SolverTrees);
        dataInt[0]=0;
        break;	
/*
	case 7:		startI=2;
			startF=1;
 			loadState(graph1, graph0, SolverTrees,  startI, dataInt, startF, dataDouble, dataChar);
			
			graph0.sketchInput(startI, dataInt, startF, dataDouble);

			copyG(graph0, graph1); 
                        graph1.simplify();
                          
                        resetTrees(graph0, DRTrees);
                        
                        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			outf<<"AFTER ADDING OBJECT & CONSTRAINT"<<endl;

                        graph0.output(outf);

                        print(graph0, SolverTrees);

                        dataInt[0]=0;
                        
                        break;
*/
   }			


   //create a new DR_Trees
   //ofstream outf("utuc.out", ios::app);
   outf << " The old sloverTrees is: " << endl;
   printForest(SolverTrees, outf, 0);
   outf << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
   
   //if(flag!=2)  createTrees(graph1, SolverTrees);

   outf << " The new sloverTrees is: " << endl;
   printForest(SolverTrees, outf, 0);
   outf << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
   
   if(autoS) dataInt[0]=1;


	outf << "The DRTrees are: " << endl;
	printForest(DRTrees, outf, 0);

   // Solver subroutine
   Solver(graph1, graph0, DRTrees, dataInt, dataDouble, dataChar);

	outf<<"The graph0 after solver:" << endl;
	graph0.output(outf);

   outfile1.close();
   outfile2.close();

   //dataChar[0]='t';
   //dataChar[1]='y';
   env->SetIntArrayRegion(arrInt, 0, intLen, dataInt);
   env->SetDoubleArrayRegion(arrDbl, 0, dblLen, dataDouble);
   env->SetCharArrayRegion(arrChar, 0, strLen, dataChar);
   env->ReleaseIntArrayElements(arrInt, dataInt, 0);
   env->ReleaseDoubleArrayElements(arrDbl, dataDouble, 0);
   env->ReleaseCharArrayElements(arrChar, dataChar, 0);
//   env->ReleaseStringUTFChars(equations, equ);
}