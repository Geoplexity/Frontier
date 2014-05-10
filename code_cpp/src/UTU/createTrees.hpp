/*The UTU (universal transfer unit)-- main controller program for
FRONTIER*/


// #include <jni.h>
// #include "utuJava.h"
// #include <iostream.h>
// #include <fstream.h>
// #include <string>
// #include <math.h>
// #include <values.h>

// int nextEdgeName=1, nextVerName=1, singleVertex=1, treeFlag;


// #include "cData.h"
// #include "mfa1List.h"
// #include "mfa2Clas.h"
// #include "mfa10Solver.h"
// #include "mfa3Prnt.cpp"
// #include "mfa4Grop.cpp"
// #include "mfa5GetC.cpp"
// #include "mfa6Dist.cpp"
// #include "mfa7DRTree.cpp"
// #include "mfa8Algo.cpp"
// #include "mfaTree.cpp"

// the_Tree Tree;

// #include "mfa9Solver.cpp"





#include <iostream>

#include "../GDS/GDS.hpp"




//tc4.skr
void createTrees(Graph &graph1, List<Cluster> &solverTree)
{
	solverTree.makeEmpty();
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
	std::cout << "create my own trees" << std::endl;
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
