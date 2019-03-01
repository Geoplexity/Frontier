//file: mfa4Grop.cpp


/*  These methods are called to make the input DR_Trees.  Groups are formed one for each of the original
    vertices and in addition groups are formed for each of the groups defined by the user and passed by the
    sketcher output to the DR_Planner.

    getGroups places all of the groups in DR_Trees, first the groups from the sketcher which are passed in
    inputdata, and processed by the oneGroup method.  Then, in the second part of the method, it generates
    groups for all of the vertices in graph0 and places them in DR_Trees. */


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


Cluster & oneGroup(jint *inputData, Graph &graph0, int &idx)
{
   Cluster *DR_Tree = new Cluster(), childCluster;

   int i, shapeNum, vName, subGroupNum;
   Vertex childCore;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;

   //out<<"Group="<<inputData[idx]<<endl;
   DR_Tree->setGroup(inputData[idx++]);
   shapeNum=inputData[idx++];
   for(i=0;i<shapeNum;i++)
   {
       vName=inputData[idx++];
       graph0.defrostOneVert(vName);
       childCore=graph0.returnVertByName(vName);
       frontiers.append(vName);
       originalV.append(vName);
       childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
       DR_Tree->children.append(childCluster);
       frontiers.makeEmpty();
       originalV.makeEmpty();
   }

   subGroupNum=inputData[idx++];
   for(i=0;i<subGroupNum;i++)
   {
       DR_Tree->children.append(oneGroup(inputData, graph0, idx));
   }
   return *DR_Tree;
}

void getGroups(jint *inputData, Graph &graph0, int &idx, List<Cluster> &DR_Trees)
{
   int i, vName, subGroupNum;
   Vertex childCore;
   Cluster childCluster;
   List<int> frontiers, originalV;
   List<Edge>  innerE, outerE;
   idx++;  // -1 default group ID
   idx++;  // 0 == number of shapes for group -1

   graph0.freeze();
   subGroupNum=inputData[idx++];
   for(i=0;i<subGroupNum;i++)
   {
       DR_Trees.append(oneGroup(inputData, graph0, idx));
   }

   for(vName=1;vName<=singleVertex;vName++)
   {
      if(graph0.hasVert(vName))
      {
         childCore=graph0.returnVertByName(vName);
         if(childCore.isFrozen()) 
         {
            graph0.defrostOneVert(vName);
            childCore.defrost();
            frontiers.append(vName);
            originalV.append(vName);
            childCluster.formCl(childCore,frontiers,innerE,outerE,originalV);
            DR_Trees.append(childCluster);
            frontiers.makeEmpty();
            originalV.makeEmpty();
         }
      }
   }
}
