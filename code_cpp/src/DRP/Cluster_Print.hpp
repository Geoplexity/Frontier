#ifndef CLUSTER_PRINT_HPP
#define CLUSTER_PRINT_HPP


#include <iostream>

#include "../GDS/GDS.hpp"


void printForest(List<Cluster> & DR_Trees, std::ostream &os, int indent);

void printTree(Cluster &DR_Tree, std::ostream &os, int indent)
{
	int i, numKids, kidName;
	List<Cluster> kids;

	kids = DR_Tree.children;
	numKids = kids.returnLen();



	for(i=0;i<indent;i++) os<<"**";
		os<<"Core: "<<DR_Tree.returnCore().returnName()
			<<",  Grp="<<DR_Tree.returnGroup();
	os<<",  Type="<<DR_Tree.returnType();
	os<<", CBifur="<<DR_Tree.returnCurrBifur();
	os<<", Solved="<<(DR_Tree.isSolved() ? "true" : "false");
	os<<", Value=";
	for(i=0;i<8;i++)
		os<<DR_Tree.returnValue(i)<<" ";

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
		os<<std::endl;


		//     DR_Tree.output(os);
		printForest(kids, os, indent);
	}
	os<<std::endl;
}

void printForest(List<Cluster> &DR_Trees, std::ostream &os, int indent)
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



#endif
