//file: mfa4Grop.cpp

#ifndef GROUP_HPP
#define GROUP_HPP


#include "../GDS/GDS.hpp"



Cluster & oneGroup(jint *inputData, Graph &graph0, int &idx);

void getGroups(jint *inputData, Graph &graph0, int &idx,
  List<Cluster> &DR_Trees);


#endif
