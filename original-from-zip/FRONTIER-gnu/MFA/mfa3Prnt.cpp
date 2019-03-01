//file:mfa3Prnt.cpp

/* These methods allow for the recursive output of DR_Trees to the given ostream */
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


void printForest(List<Cluster> & DR_Trees, ostream &os, int indent);

void printTree(Cluster &DR_Tree, ostream &os, int indent)
{
   int i, numKids, kidName;
   List<Cluster> kids;

   kids = DR_Tree.children;
   numKids = kids.returnLen();

   for(i=0;i<indent;i++) os<<"**";
   os<<"Core: "<<DR_Tree.returnCore().returnName()
     <<",  Grp="<<DR_Tree.returnGroup();
   os<<", CBifur="<<DR_Tree.returnCurrBifur();
   os<<", Solved="<<(DR_Tree.isSolved() ? "true" : "false");
   os<<", Value=";
   for(i=0;i<8;i++)  os<<DR_Tree.returnValue(i)<<" ";

   if(numKids>0)
   {
      os<<"   Children: ";
      for(i=1;i<=numKids;i++)
      {
         kidName=kids.retrieve(i).returnCore().returnName();
         if(kidName==0)
           os<<"Grp"<<kids.retrieve(i).returnGroup()<<' ';
         else
           os<<kidName<<' ';
      }
      os<<endl;

     printForest(kids, os, indent);
   }
   os<<endl;
}

void printForest(List<Cluster> &DR_Trees, ostream &os, int indent)
{
   int i, len;
   Cluster oneTree;

   indent=indent+1;
   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      oneTree = DR_Trees.retrieve(i);
      printTree(oneTree, os, indent);
   }
   return;
}
