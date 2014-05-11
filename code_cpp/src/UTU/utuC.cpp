#include <iostream>
#include <fstream>

// #include "../../ext/JNI/jni.h"
#include "utuJava.h"

#include "../GDS/GDS.hpp"
#include "../DRP/DRP.hpp"

#include "../ESM/ESM.hpp"
#include "../ESM/mfaTree.hpp"

the_Tree Tree;
int treeFlag;

JNIEXPORT void JNICALL Java_utuJava_utuC(JNIEnv *env, jobject obj, jintArray arrInt, jdoubleArray arrDbl, jcharArray arrChar)
{
	std::ofstream outf("utuc.out", std::ios::app);
	outf<<"OPEN UTU"<<std::endl;

	// Get int and double arrays from Java
	jsize intLen = env->GetArrayLength(arrInt);
	jint *dataInt = env->GetIntArrayElements(arrInt, 0);

	outf<<"INTS"<<std::endl;

	jsize dblLen = env->GetArrayLength(arrDbl);
	jdouble *dataDouble = env->GetDoubleArrayElements(arrDbl, 0);

	outf<<"DOUBLES"<<std::endl;

	jsize strLen = env->GetArrayLength(arrChar);
	jchar *dataChar = env->GetCharArrayElements(arrChar, 0);

	outf<<"CHARS"<<std::endl;

	//   const char* equ = env->GetStringUTFChars(equations, NULL);

	//  for(int i=0; i<100; i++)
	//   outf<<dataChar[i]<<std::endl;

	//   outf<<"equation: "<<dataChar[0]<<" "<<strLen<<std::endl;


	Graph graph0;
	List<Cluster> SolverTrees;
	List<Cluster> DRTrees;

	std::ofstream outfile1; // output file
	std::ofstream outfile2; // detail file

	outfile1.open("mfa.out");
	outfile2.open("detmfa.out");

	int startGroup, startI, startF;
	int type, name, numEnds, theEnd;
	double value;

	std::ofstream dataDump;

	int i, j, length;

	Edge newEdge, *theEdge;

	Graph graph1;

	int flag=dataInt[0];
	treeFlag=dataInt[1];
	treeFlag=1;
	int deleteEdgeName;
	bool autoS;

	autoS=false;

	outf<<"*FLAG: "<<flag<<std::endl;

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
			outf << "Tree Flag in utuC.cpp: " << treeFlag << std::endl;
			Tree.build_Tree(treeFlag); //build the Tree here

			startGroup=graph0.sketchInput(startI, dataInt, startF, dataDouble);

			outf<<"StartGroup: "<<startGroup<<std::endl;

			graph0.output(outf);

			outf<<"original graph:"<<std::endl;
			graph0.output(outfile1);
			graph0.output(outfile2);

			for(i=startGroup;i<startGroup+50; i++)
				outf<<"data "<<startGroup<<" "<<dataInt[i]<<std::endl;


			dataDump.open("dataDump.txt");

			for(i=startGroup;i<intLen;i++)
				dataDump<<dataInt[i]<<std::endl;

			dataDump.close();

			// Initialize the DRTrees structure based on input data
			getGroups(dataInt, graph0, startGroup, DRTrees);

			print(graph0, DRTrees);

			outf<<"Groups read"<<std::endl;

			copyG(graph0, graph1);  // copy Graph from graph0 to graph1
			graph1.simplify();      // merge multi-edges and remove zero-weight edges

			outf<<"Graph Copied"<<std::endl;

			outf<<"Dimension="<<graph0.returnDimen()<<std::endl;
			graph0.output(outf);

			outf<<"SolverTrees called"<<std::endl;

			// MFA Algorithm: input are graph0, DR_Trees; output is SolverTrees
			SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			fixTree(SolverTrees);

			outf<<"SolverTrees returned"<<std::endl;

			//the_Tree Tree(treeFlag);

			break;

		case 3:
			startI=2;
			startF=1;

			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble, dataChar);

			theEdge=graph0.EdgeAddr(dataInt[startI++]);

			value=dataDouble[startF++];

			outf<<"Value"<<value<<std::endl;

			for(i=startF;i<startF+10;i++)
				outf<<i<<" "<<dataDouble[i]<<std::endl;

			theEdge->setValue((float) value);

			length=SolverTrees.returnLen();

			for(i=1;i<=length;i++)
				resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd1());
			for(i=1;i<=length;i++)
				resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd2());

			dataInt[0]=0;
			break;

		case 4:
			outf<<"Start Add"<<std::endl;
			startI=2;
			startF=1;
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble, dataChar);

			outf<<"State Loaded"<<std::endl;

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

			outf<<"AFTER ADDING CONSTRAINT"<<std::endl;

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
			outf<<"AFTER REMOVING CONSTRAINT"<<std::endl;
			graph0.output(outf);
			print(graph0, SolverTrees);
			dataInt[0]=0;
			break;
		/*
		case 7:
			startI=2;
			startF=1;
			loadState(graph1, graph0, SolverTrees,  startI, dataInt, startF, dataDouble, dataChar);

			graph0.sketchInput(startI, dataInt, startF, dataDouble);

			copyG(graph0, graph1);
			graph1.simplify();

			resetTrees(graph0, DRTrees);

			SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			outf<<"AFTER ADDING OBJECT & CONSTRAINT"<<std::endl;

			graph0.output(outf);

			print(graph0, SolverTrees);

			dataInt[0]=0;

			break;
		*/
	}


	//create a new DR_Trees
	//std::ofstream outf("utuc.out", ios::app);
	outf << " The old sloverTrees is: " << std::endl;
	printForest(SolverTrees, outf, 0);
	outf << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

	//if(flag!=2)  createTrees(graph1, SolverTrees);

	outf << " The new sloverTrees is: " << std::endl;
	printForest(SolverTrees, outf, 0);
	outf << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

	if(autoS) dataInt[0]=1;


	outf << "The DRTrees are: " << std::endl;
	printForest(DRTrees, outf, 0);

	// Solver subroutine
	Solver(graph1, graph0, DRTrees, dataInt, dataDouble, dataChar);

	outf<<"The graph0 after solver:" << std::endl;
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
