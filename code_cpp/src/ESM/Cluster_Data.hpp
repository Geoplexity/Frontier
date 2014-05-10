#ifndef CLUSTER_DATA_HPP
#define CLUSTER_DATA_HPP


#define CDATALEN	8

#ifndef CDATA_H
#define CDATA_H

#include <iostream>

class ClustData
{
public:
	int length;
	bool *inOrBetween;
	float *values;
	int *edgeID;
	int	*edgeType;
	int	*e1ID;
	int	*e2ID;
	int	*e1Type;
	int	*e2Type;
	int	*e1Part;
	int	*e2Part;

	void create(int len);
	void init();
	void destroy();
	void copy(const ClustData &oldClustData);

	ClustData()
	{
		create(0);
		init();
	}

	ClustData(int len)
	{
		create(len);
		init();
	}

	ClustData(const ClustData &oldClustData)
	{
		create(oldClustData.length);
		copy(oldClustData);
	}

	~ClustData()
	{
		destroy();
	}

	ClustData& operator = (const ClustData &oldClustData)
	{
		if(this != &oldClustData)
		{
			destroy();
			create(oldClustData.length);
			copy(oldClustData);
		}
		return *this;
	}

	friend std::ostream& operator <<(std::ostream&, const ClustData&);
};

void ClustData::create(int len)
{
	length=len;
	if(length==0) return;
	inOrBetween = new bool[length];
	values = new float[length];
	edgeID = new int[length];
	edgeType = new int[length];
	e1ID = new int[length];
	e2ID = new int[length];
	e1Type = new int[length];
	e2Type = new int[length];
	e1Part = new int[length];
	e2Part = new int[length];
}

void ClustData::init()
{
	int i;
	for(i=0; i<length; i++)
	{
		inOrBetween[i]=false;
		values[i]=0.0;
		edgeID[i]=0;
		edgeType[i]=0;
		e1ID[i]=0;
		e2ID[i]=0;
		e1Type[i]=0;
		e2Type[i]=0;
		e1Part[i]=0;
		e2Part[i]=0;
	}
}

void ClustData::destroy()
{
	if(length==0) return;
		length=0;
	delete [] inOrBetween;
	delete [] values;
	delete [] edgeID;
	delete [] edgeType;
	delete [] e1ID;
	delete [] e2ID;
	delete [] e1Type;
	delete [] e2Type;
	delete [] e1Part;
	delete [] e2Part;
}

void ClustData::copy(const ClustData &oldClustData)
{
	int i;
	for(i=0; i<length; i++)
	{
		inOrBetween[i]=oldClustData.inOrBetween[i];
		values[i]=oldClustData.values[i];
		edgeID[i]=oldClustData.edgeID[i];
		edgeType[i]=oldClustData.edgeType[i];
		e1ID[i]=oldClustData.e1ID[i];
		e2ID[i]=oldClustData.e2ID[i];
		e1Type[i]=oldClustData.e1Type[i];
		e2Type[i]=oldClustData.e2Type[i];
		e1Part[i]=oldClustData.e1Part[i];
		e2Part[i]=oldClustData.e2Part[i];
	}
}

std::ostream& operator <<(std::ostream& out, const ClustData &printData)
{
	int i,j;

	for(i=0; i<printData.length; i++)
	{
		if(printData.inOrBetween[i])
			out<<"True ";
		else
			out<<"False";

		out.width(5);
		out<<printData.edgeID[i];
		out.width(5);
		out<<printData.edgeType[i];
		out.width(5);
		out<<printData.e1ID[i];
		out.width(5);
		out<<printData.e1Type[i];
		out.width(5);
		out<<printData.e1Part[i];
		out.width(5);
		out<<printData.e2ID[i];
		out.width(5);
		out<<printData.e2Type[i];
		out.width(5);
		out<<printData.e2Part[i];

		out.width(5);
		out.precision(2);
		out<<printData.values[i];
		if(i<printData.length-1) out<<"\n";
	}

	return out;
}

#endif



#endif
