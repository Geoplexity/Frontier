/*The UTU (universal transfer unit)*/

/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
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

int nextEdgeName=1, nextVerName=1, singleVertex=1;

#include "mfa1List.h"
#include "mfa2Clas.h"
#include "mfa3Prnt.cpp"
#include "mfa4Grop.cpp"
#include "mfa5GetC.cpp"
#include "mfa6Dist.cpp"
#include "mfa7DRTree.cpp"
#include "mfa8Algo.cpp"
#include "mfa9Solver.cpp"
#include "mfa10Solver.h"

JNIEXPORT void JNICALL Java_utuJava_utuC(JNIEnv *env,jobject obj,jintArray arrInt,jdoubleArray arrDbl)
{
   cout<<"OPEN UTU"<<endl;
   
   // Get int and double arrays from Java
   jsize intLen = env->GetArrayLength(arrInt);
   jint *dataInt = env->GetIntArrayElements(arrInt, 0);

   jsize dblLen = env->GetArrayLength(arrDbl);
   jdouble *dataDouble = env->GetDoubleArrayElements(arrDbl, 0);

   Graph graph0;
   List<Cluster> SolverTrees;
   List<Cluster> DRTrees;

   ofstream outfile1; // output file
   ofstream outfile2; // detail file

   outfile1.open("mfa.out");
   outfile1.open("mfa.dtl");
                        
   int startGroup, startI, startF;
   int type, name, numEnds, theEnd;
   double value;


   int i, j, length;

   Edge newEdge, *theEdge;

   Graph graph1;
  
   int flag=dataInt[0];
   bool autoS;

   autoS=false;

   cout<<"FLAG: "<<flag<<endl;

   if(flag>10)
   {
      flag=flag-10;
      autoS=true;
   }

   switch(flag)
   {
/*	case 1:		dataInt[0]=2;
			break; */

	case 0:         // Initialize graph0 and DR_Trees based on input file (utu.bin or utu.txt)
                        startI=1;
			startF=0;
      			startGroup=graph0.sketchInput(startI, dataInt, startF, dataDouble);

			cout<<"original graph:"<<endl;
      			//graph0.output(outfile1);

      			// Initialize the DRTrees structure based on input data
      			getGroups(dataInt, graph0, startGroup, DRTrees);
			
			cout<<"Groups read"<<endl;

      			copyG(graph0, graph1);  // copy Graph from graph0 to graph1
      			graph1.simplify();      // merge multi-edges and remove zero-weight edges

			cout<<"Graph Copied"<<endl;

			graph0.output(cout);

			cout<<"SolverTrees called"<<endl;

      			// MFA Algorithm: input are graph0, DR_Trees; output is SolverTrees
      			SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			fixTree(SolverTrees);

    			cout<<"SolverTrees returned"<<endl;

			break;

	case 3:		startI=startF=1;	
		
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
			
			theEdge=graph0.EdgeAddr(dataInt[startI++]);
		
			value=dataDouble[startF++];

			cout<<"Value"<<value<<endl;
			
			for(i=startF;i<startF+10;i++)
			   cout<<i<<" "<<dataDouble[i]<<endl;

			theEdge->setValue((float) value);
			
			length=SolverTrees.returnLen();

			for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd1());
                        for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd2());
			
                        dataInt[0]=0;
			break;

	case 4: 	cout<<"Start Add"<<endl;
			startI=startF=1;   
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);

			cout<<"State Loaded"<<endl;

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

			copyG(graph0, graph1);
                        graph1.simplify();

                        resetTrees(graph0, DRTrees);

			singleVertex=10;
			
                        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			cout<<"AFTER ADDING CONSTRAINT"<<endl;

			graph0.output(cout);

			print(graph0, SolverTrees);
			
			dataInt[0]=0;

                        break;	
		
	case 5:		startI=startF=1;
                        loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
	
			graph0.output(cout);

			print(graph0, DRTrees);
		
			graph0.delEdgeByName(dataInt[startI++]);

                        copyG(graph0, graph1);
                        graph1.simplify();

                        resetTrees(graph0, DRTrees);

                        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			cout<<"AFTER REMOVING CONSTRAINT"<<endl;

                        graph0.output(cout);

                        print(graph0, SolverTrees);

			dataInt[0]=0;		

                        break;	

	case 7:		startI=startF=1;
 			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
			
			graph0.sketchInput(startI, dataInt, startF, dataDouble);

			copyG(graph0, graph1); 
                        graph1.simplify();
                          
                        resetTrees(graph0, DRTrees);
                        
                        SolverTrees = mfaAlgo(graph1, DRTrees, outfile1, outfile2);

			cout<<"AFTER ADDING OBJECT & CONSTRAINT"<<endl;

                        graph0.output(cout);

                        print(graph0, SolverTrees);

                        dataInt[0]=0;
                        
                        break;
   }			

   if(autoS) dataInt[0]=1;

   // Solver subroutine
   Solver(graph1, graph0, SolverTrees, dataInt, dataDouble);

   checkEdge(graph0.returnEdgeByName(1), graph0, SolverTrees);

   env->SetIntArrayRegion(arrInt, 0, intLen, dataInt);
   env->SetDoubleArrayRegion(arrDbl, 0, dblLen, dataDouble);
   env->ReleaseIntArrayElements(arrInt, dataInt, 0);
   env->ReleaseDoubleArrayElements(arrDbl, dataDouble, 0);
}
