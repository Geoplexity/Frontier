#include "Cluster.hpp"


Cluster::Cluster(const Cluster &clu)
{
	int i;

	core=clu.core;
	group=clu.group;
	type=clu.type;
	frontiers=clu.frontiers;
	innerE=clu.innerE;
	outerE=clu.outerE;
	originalV=clu.originalV;
	children=clu.children;
	bifurs=clu.bifurs;
	currBifur=clu.currBifur;
	numBifurs=clu.numBifurs;
	constrain=clu.constrain;
	solved=clu.solved;
	values=clu.values;
	currAlias=clu.currAlias;
	left=clu.left;
	right=clu.right;
	lines=clu.lines;
}

void Cluster::formCl(Vertex &c, List<int> &fV, List<Edge> &iE,
	List<Edge> &oE, List<int> &o)
{
	core=c;
	frontiers=fV;
	innerE=iE;
	outerE=oE;
	originalV=o;
}


void Cluster::output(std::ostream &os)
{
	int i,len, child;
	os<<"Cluster(core):"<<core.returnName()
		<<", depth="<<core.returnDepth()<<", type="<<type;
	if(constrain==1) os<<", well constrained";
	if(constrain>1) os<<", over constrained";
	if(constrain<1) os<<", under constrained";
	os<<std::endl<<"Frontiers: ";
	len=frontiers.returnLen();
	for(i=1;i<=len;i++)
		os<<frontiers.retrieve(i)<<' ';

	os<<std::endl<<"Previous clusters: ";
	len=children.returnLen();
	for(i=1;i<=len;i++)
	{
		child=children.retrieve(i).returnCore().returnName();
		if(child!=0)
			os<<child<<' ';
		else
			os<<"Grp"<<children.retrieve(i).returnGroup()<<' ';
	}

	os<<std::endl<<"Original Vertices: ";
	len=originalV.returnLen();
	for(i=1;i<=len;i++)
		os<<originalV.retrieve(i)<<' ';

	os<<std::endl<<"Inner Edge: ";
	len=innerE.returnLen();
	for(i=1;i<=len;i++)
		os<<innerE.retrieve(i).returnName()<<'('
			<<innerE.retrieve(i).returnWeight()<<','
			<<innerE.retrieve(i).returnEnd1()<<'-'
			<<innerE.retrieve(i).returnEnd2()<<") ";

	os<<std::endl<<"Onter Edge: ";
	len=outerE.returnLen();
	for(i=1;i<=len;i++)
		os<<outerE.retrieve(i).returnName()<<'('
			<<outerE.retrieve(i).returnWeight()<<','
			<<outerE.retrieve(i).returnEnd1()<<'-'
			<<outerE.retrieve(i).returnEnd2()<<") ";

	os<<std::endl<<"Bifurcations (Current Selection: ";
	if(currBifur<1) os<<"None):"<<std::endl;
	else os<<currBifur<<"):"<<std::endl;
	len=bifurs.returnLen();
	for(i=1;i<=len;i++)
		os<<"    "<<bifurs.retrieve(i)<<std::endl;


	//  printForest(children, os, 0);
	os << "The Degs are " << std::endl;
	for(i=1;i<=values.returnLen();i++)
		os << values.retrieve(i);
}



Cluster & Cluster::operator = (const Cluster &clu)
{
	int i;

	if( this != &clu)
	{
		core=clu.core;
		group=clu.group;
		type=clu.type;
		frontiers=clu.frontiers;
		innerE=clu.innerE;
		outerE=clu.outerE;
		originalV=clu.originalV;
		children=clu.children;
		bifurs=clu.bifurs;
		currBifur=clu.currBifur;
		numBifurs=clu.numBifurs;
		constrain=clu.constrain;
		solved=clu.solved;
		values=clu.values;
		currAlias=clu.currAlias;
		left=clu.left;
		right=clu.right;
		lines=clu.lines;
	}
	return *this;
}