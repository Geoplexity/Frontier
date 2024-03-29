/*The UTU (universal transfer unit)-- main controller program for
FRONTIER*/

/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html. 
The GNU general public license is also appended to the end of this file.

You should also have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <jni.h>
#include "utuJava.h"
#include<iostream.h>
#include<fstream.h>
#include<string>
#include<math.h>
#include<values.h>

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
/*	case 1:		dataInt[0]=2;
			break; */

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
		
	case 5:		startI=2;
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

/*
		    GNU GENERAL PUBLIC LICENSE
		       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.
                       59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

			    Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation's software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Library General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author's protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors' reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone's free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

		    GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The "Program", below,
refers to any such program or work, and a "work based on the Program"
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term "modification".)  Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program's
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) You must cause the modified files to carry prominent notices
    stating that you changed the files and the date of any change.

    b) You must cause any work that you distribute or publish, that in
    whole or in part contains or is derived from the Program or any
    part thereof, to be licensed as a whole at no charge to all third
    parties under the terms of this License.

    c) If the modified program normally reads commands interactively
    when run, you must cause it, when started running for such
    interactive use in the most ordinary way, to print or display an
    announcement including an appropriate copyright notice and a
    notice that there is no warranty (or else, saying that you provide
    a warranty) and that users may redistribute the program under
    these conditions, and telling the user how to view a copy of this
    License.  (Exception: if the Program itself is interactive but
    does not normally print such an announcement, your work based on
    the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

    a) Accompany it with the complete corresponding machine-readable
    source code, which must be distributed under the terms of Sections
    1 and 2 above on a medium customarily used for software interchange; or,

    b) Accompany it with a written offer, valid for at least three
    years, to give any third party, for a charge no more than your
    cost of physically performing source distribution, a complete
    machine-readable copy of the corresponding source code, to be
    distributed under the terms of Sections 1 and 2 above on a medium
    customarily used for software interchange; or,

    c) Accompany it with the information you received as to the offer
    to distribute corresponding source code.  (This alternative is
    allowed only for noncommercial distribution and only if you
    received the program in object code or executable form with such
    an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and "any
later version", you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

			    NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

		     END OF TERMS AND CONDITIONS
*/

