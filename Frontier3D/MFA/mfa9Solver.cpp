//file: mfa9Solver.cpp

/*The entire equation and solution manager ESM*/

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


//#include "mfa10Solver.h"
//#include "mfaTree.cpp"


#define INTSIZE		16000
#define FLOATSIZE	4000
#define CHARSIZE	10000

//prints the a cluster and all its children starting with a single cluster
void print(Graph &graph0, Cluster &theCluster)
{
   theCluster.output(cout);

   int i, length;

   length=theCluster.children.returnLen();

   if(theCluster.returnName()==0) return;

   for(i=1;i<=length;i++)
      print(graph0, theCluster.children.retrieve(i));
}

//for every cluster in SolverTrees, this method prints contents of the cluster and all its children
void print(Graph &graph0, List<Cluster> &SolverTrees)
{
   int i, numTrees;
   numTrees=SolverTrees.returnLen();

   cout<<endl<<numTrees<<" Solver Input Trees"<<endl;
   for(i=1;i<=numTrees;i++)
   {
      cout<<"---------"<<endl;
      cout<<"Tree "<<i<<endl;
      printTree(SolverTrees.retrieve(i), cout, 1);
   }
}

//prints the string corresponding to the bifurcations of theCluster
void printBifurcations(Cluster &theCluster)
{
   int i, length;
   List<string> theList;

   theList=theCluster.returnBifurs();

   length=theCluster.returnNumBifurs();

   for(i=1;i<=length;i++)
      cout<<"       "<<theList.retrieve(i)<<endl;
}

//returns the distance between (x1,y1) and (x2,y2)
float getDistance(float x1, float y1, float x2, float y2)
{
    return(sqrt(pow((x1-x2),2)+pow((y1-y2),2)));
}

//returns the angle from (x1,y1) to (x2,y2)
float getAngle(float x1, float y1, float x2, float y2)
{
    float cos1, sin1, PI=3.141592;

    sin1=(y2-y1)/getDistance(x1,y1,x2,y2);
    cos1=(x2-x1)/getDistance(x1,y1,x2,y2);

    if((sin1>=0 && cos1>=0) || (sin1>=0 && cos1<0)) return acos(cos1);
    if(sin1<0 && cos1<=0) return 2*PI-acos(cos1);
    return 2*PI+asin(sin1);
}

//returns the angle that correspones to a given sine and cosine value
float getAngle(float theCos, float theSin)
{
    float PI=3.141592;

    if((theSin>=0 && theCos>=0) || (theSin>=0 && theCos<0)) return acos(theCos);
    if(theSin<0 && theCos<=0) return 2*PI-acos(theCos);
    return 2*PI+asin(theSin);
}

//returns in Vertex, the point of intersection between line1 and line2
void getIntersectionLineLine(Vertex &theVertex, float x1, float y1, float m1, float x2, float y2, float m2)
{
     float Xout, Yout;
     float Xnew, Ynew;

     Xnew=x2-x1;
     Ynew=y2-y1;

     Xout=(m2*Xnew+Ynew)/(m1-m2);
     Yout=Xout*m1;
     
     Xout+=x1;
     Yout+=y1;

     theVertex.setValues(0,Xout);
     theVertex.setValues(1,Yout);
     theVertex.setType(0);
}

//returns in thePoints the two point of intersection between a line and a circle
void getIntersectionLineCircle(List<Vertex> &thePoints, float x1, float y1, float m1, float x2, float y2, float r)
{
    float Xout1, Yout1, Xout2, Yout2;
    float Xnew, Ynew;
    float deter;

    Xnew=x2-x1;
    Ynew=y2-y1;

    deter=-y2-pow(m1,2)*pow(x1,2)+2*y1*m1*x1+pow(r,2)+pow(m1,2)*pow(r,2);

    if(deter<0) 
    {
      thePoints.retrieve(1).setType(-1);
      return;
    }

    Xout1=pow(m1,2)*x1+y1*m1+sqrt(deter)+x1;
    Xout2=pow(m1,2)*x1+y1*m1-sqrt(deter)+x2;
    Yout1=m1*Xout1;
    Yout2=m1*Xout2;

    thePoints.retrieve(1).setType(0);
    thePoints.retrieve(1).setValues(0,Xout1);
    thePoints.retrieve(1).setValues(1,Yout1);
    thePoints.retrieve(2).setType(0);
    thePoints.retrieve(2).setValues(0,Xout2);
    thePoints.retrieve(2).setValues(1,Yout2);
}

//returns a vertex list containing the two points of intersection between 2 circles
void getIntersectionCircleCircle(List<Vertex> &thePoints, float x1, float y1, float r1, float x2, float y2, float r2)
{
    float Xout1, Yout1, Xout2, Yout2;
    float Xnew, Ynew;
    float deter, d, e, f;

    Xnew=x2-x1;
    Ynew=y2-y1;

    d=pow(Xnew,2)+pow(Ynew,2);
    e=pow(r1,2)-pow(r2,2);
    f=pow(r1,2)+pow(r2,2);

    deter=-pow(d,2)+2*d*f-pow(e,2);

    if(deter<0) 
    {
      thePoints.retrieve(1).setType(-1);
      return;
    }

    Xout1=(Xnew*(e+d)-Ynew*sqrt(deter))/(2*d)+x1;
    Xout2=(Xnew*(e+d)+Ynew*sqrt(deter))/(2*d)+x1;
    Yout1=(Ynew*(e+d)-Xnew*sqrt(deter))/(2*d)+y1;
    Yout2=(Ynew*(e+d)-Xnew*sqrt(deter))/(2*d)+x1;

    thePoints.retrieve(1).setType(0);
    thePoints.retrieve(1).setValues(0,Xout1);
    thePoints.retrieve(1).setValues(1,Yout1);
    thePoints.retrieve(2).setType(0);
    thePoints.retrieve(2).setValues(0,Xout2);
    thePoints.retrieve(2).setValues(1,Yout2);
}    

//given an point and another object, determines if the point has incidence with the object
bool isValidPointIncidence(Vertex &thePoint, Vertex &theOther, int part1, int part2)
{
    int type;
    float angle1a, angle1b, angle2a, angle2b, x1, y1, x2, y2, x3, y3;
    float pointAngle, length1, length2, length;

    if(part1!=1) return false;
    if(part2!=0) return true;   

    type=theOther.returnType();
    
    x1=thePoint.returnDegValueByName(0);
    y1=thePoint.returnDegValueByName(1);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
          case 2:   angle1a=theOther.returnDegValueByName(4);
                    angle1b=theOther.returnDegValueByName(5);
                    return (getAngle(angle1a,angle1b)==getAngle(x2,y2,x1,y2));
  	                break;
          case 3:   x3=theOther.returnDegValueByName(2);
  		            y3=theOther.returnDegValueByName(3);
                    length=getDistance(x2,y2,x3,y3);
                    length1=getDistance(x1,y1,x2,y2);
                    length2=getDistance(x1,y1,x3,y3);
                    if(length1<length && length2<length) return true;
                    else return false;
                    break;
          case 5:   pointAngle=getAngle(x2,y2,x1,y2);
                    angle1a=theOther.returnDegValueByName(4);
                    angle1b=theOther.returnDegValueByName(5);
                    angle2a=theOther.returnDegValueByName(6);
                    angle2b=theOther.returnDegValueByName(7);
                    if(pointAngle>=getAngle(angle1a,angle1b) && pointAngle<=getAngle(angle2a,angle2b)) return true;
                    else return false;
          default:  return true;
	                break;
    }
}

//given a line and another object, returns true if the object is incidence about the line
bool isValidLineIncidence(Vertex &theLine, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, m1, x2, y2, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    if(part1!=0) return false;
    if(part2!=0) return true;   

    x1=theLine.returnDegValueByName(0);
    y1=theLine.returnDegValueByName(1);
    m1=theLine.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
          case 1:   m2=theOther.returnDegValueByName(4);
                    return(m1!=m2);
                    break;
          case 2:
          case 3:   if(type==2) m2=(theOther.returnDegValueByName(5)/theOther.returnDegValueByName(4));
                    else m2=theOther.returnDegValueByName(4);
                    getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
                    return(isValidPointIncidence(theInter, theOther,1,0));
                    break;
          case 4:   theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                    return(theInters.retrieve(1).returnType()!=-1);
                    break;
          case 5:   theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                    out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                    return(out1 || out2);
                    break;
          default:  return true;
    }
}

//given a ray and another object, returns true if the object is incident upon the ray
bool isValidRayIncidence(Vertex &theRay, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, x2, y2, x3, y3, m1, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theRay.returnDegValueByName(0);
    y1=theRay.returnDegValueByName(1);
    m1=theRay.returnDegValueByName(5)/theRay.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
          case 2:   switch(part1*10+part2)
                    {
                          case 0:  m2=theOther.returnDegValueByName(5)/theOther.returnDegValueByName(4);
                                   getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
                                   out1=isValidPointIncidence(theInter, theOther,1,0);
                                   out2=isValidPointIncidence(theInter, theRay,1,0);
                                   return(out1 && out2);
                                   break;
                          case 1:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 10: Vert1.setType(0);
                                   Vert1.setValues(0,x1);
                                   Vert1.setValues(1,y1);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 11: return true;
                                   break;
                    }
                    break;
          case 3:   switch(part1*10+part2)
                    {
                          case 0:  m2=theOther.returnDegValueByName(4);
                                   getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
                                   out1=isValidPointIncidence(theInter, theOther,1,0);
                                   out2=isValidPointIncidence(theInter, theRay,1,0);
                                   return(out1 && out2);
                                   break;
                          case 1:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 2:  x3=theOther.returnDegValueByName(2);
                                   y3=theOther.returnDegValueByName(3);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 10: Vert1.setType(0);
                                   Vert1.setValues(0,x1);
                                   Vert1.setValues(1,y1);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 11:
                          case 12: return true;
                                   break;
                    }
                    break;
          case 4:   switch(part1*10+part2)
                    {
                           case 0:  theInters.append(Vert1);
                                    theInters.append(Vert2);
                                    r2=theOther.returnDegValueByName(2);
                                    getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                                    if(theInters.retrieve(1).returnType()==-1)  return false;
                                    out1=isValidPointIncidence(theInters.retrieve(1), theRay,1,0);
                                    out2=isValidPointIncidence(theInters.retrieve(2), theRay,1,0);
                                    return(out1 || out2);
                           case 3:  Vert1.setType(0);
                                    Vert1.setValues(0,x2);
                                    Vert1.setValues(1,y2);
                                    return(isValidPointIncidence(Vert1,theRay,1,0));
                                    break;
                           case 10:
                           case 13: return true;
                                    break;
                    }
                    break;
          case 5:   switch(part1*10+part2)
                    {
                          case 0:  theInters.append(Vert1);
                                   theInters.append(Vert2);
                                   r2=theOther.returnDegValueByName(2);
                                   getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                                   if(theInters.retrieve(1).returnType()==-1)  return false;
                                   out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                                   out1=(out1 && isValidPointIncidence(theInters.retrieve(1), theRay,1,0));
                                   out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                                   out2=(out2 && isValidPointIncidence(theInters.retrieve(2), theRay,1,0));
                                   return(out1 || out2);
                                   break;
                          case 1:  r2=theOther.returnDegValueByName(2);
                                   x3=x2+r2*theOther.returnDegValueByName(4);
                                   y3=y2+r2*theOther.returnDegValueByName(5);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 2:  r2=theOther.returnDegValueByName(2);
                                   x3=x2+r2*theOther.returnDegValueByName(6);
                                   y3=y2+r2*theOther.returnDegValueByName(7);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 3:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theRay,1,0));
                                   break;
                          case 10: Vert1.setType(0);
                                   Vert1.setValues(0,x1);
                                   Vert1.setValues(1,y1);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 11:
                          case 12:
                          case 13: return true;
                                   break;
                    }
          default:  return true;
    }
}

//given a line segment and another object, returns true if the object is incident with the line segment
bool isValidLSIncidence(Vertex &theLS, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1a, y1a, x1b, y1b, x2, y2, x3, y3, m1, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1a=theLS.returnDegValueByName(0);
    y1a=theLS.returnDegValueByName(1);
    x1b=theLS.returnDegValueByName(2);
    y1b=theLS.returnDegValueByName(3);
    m1=theLS.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
          case 3:   switch(part1*10+part2)
                    {
                          case 0:  m2=theOther.returnDegValueByName(4);
                                   getIntersectionLineLine(theInter, x1a, y1a, m1, x2, y2, m2);
                                   out1=isValidPointIncidence(theInter, theOther,1,0);
                                   out2=isValidPointIncidence(theInter, theLS,1,0);
                                   return(out1 && out2);
                                   break;
                          case 1:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 2:  x3=theOther.returnDegValueByName(2);
                                   y3=theOther.returnDegValueByName(3);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 10: Vert1.setType(0);
                                   Vert1.setValues(0,x1a);
                                   Vert1.setValues(1,y1a);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 20: Vert1.setType(0);
                                   Vert1.setValues(0,x1b);
                                   Vert1.setValues(1,y1b);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 21:
                          case 11:
                          case 12: return true;
                                   break;
                    }
                    break;
          case 4:   switch(part1*10+part2)
                    {
                          case 0:  theInters.append(Vert1);
                                   theInters.append(Vert2);
                                   r2=theOther.returnDegValueByName(2);
                                   getIntersectionLineCircle(theInters, x1a, y1a, m1, x2, y2, r2);
                                   if(theInters.retrieve(1).returnType()==-1)  return false;
                                   out1=isValidPointIncidence(theInters.retrieve(1), theLS,1,0);
                                   out2=isValidPointIncidence(theInters.retrieve(2), theLS,1,0);
                                   return(out1 || out2);
                          case 3:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 10:
                          case 20:
                          case 13:
                          case 23: return true;
                                   break;
                    }
                    break;
          case 5:   switch(part1*10+part2)
                    {
                          case 0:  theInters.append(Vert1);
                                   theInters.append(Vert2);
                                   r2=theOther.returnDegValueByName(2);
                                   getIntersectionLineCircle(theInters, x1a, y1a, m1, x2, y2, r2);
                                   if(theInters.retrieve(1).returnType()==-1)  return false;
                                   out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                                   out1=(out1 && isValidPointIncidence(theInters.retrieve(1), theLS,1,0));
                                   out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                                   out2=(out2 && isValidPointIncidence(theInters.retrieve(2), theLS,1,0));
                                   return(out1 || out2);
                                   break;
                          case 1:  r2=theOther.returnDegValueByName(2);
                                   x3=x2+r2*theOther.returnDegValueByName(4);
                                   y3=y2+r2*theOther.returnDegValueByName(5);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 2:  r2=theOther.returnDegValueByName(2);
                                   x3=x2+r2*theOther.returnDegValueByName(6);
                                   y3=y2+r2*theOther.returnDegValueByName(7);
                                   Vert1.setType(0);
                                   Vert1.setValues(0,x3);
                                   Vert1.setValues(1,y3);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 3:  Vert1.setType(0);
                                   Vert1.setValues(0,x2);
                                   Vert1.setValues(1,y2);
                                   return(isValidPointIncidence(Vert1,theLS,1,0));
                                   break;
                          case 10: Vert1.setType(0);
                                   Vert1.setValues(0,x1a);
                                   Vert1.setValues(1,y1a);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 20: Vert1.setType(0);
                                   Vert1.setValues(0,x1b);
                                   Vert1.setValues(1,y1b);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 11:
                          case 12:
                          case 13:
                          case 21:
                          case 22:
                          case 23: return true;
                                   break;
                    }
          default:  return true;
    }
}

//given a circle and another object, returns true if the object is incident upon the circle
bool isValidCircleIncidence(Vertex &theCircle, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, r1, x2, y2, x3, y3, m2, r2, sep;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theCircle.returnDegValueByName(0);
    y1=theCircle.returnDegValueByName(1);
    r1=theCircle.returnDegValueByName(2);  
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);
    r2=theOther.returnDegValueByName(2);
    
    switch(type)
    {
          case 4:   switch(part1*10+part2)
                    {
                          case 0:  sep=getDistance(x1,y1,x2,y2);
                                   if((sep+r1)<r2 || (sep+r2)<r1) return false;
                                   else return(getDistance(x1,y1,x2,y2)<=(r1+r2));
                                   break;
                          case 3:
                          case 10:
                          case 30: return true;
                                   break;
                    }
                    break;
          case 5:   switch(part1*10+part2)
                    {
                          case 0:  theInters.append(Vert1);
                                   theInters.append(Vert2);
                                   getIntersectionCircleCircle(theInters, x1, y1, r1, x2, y2, r2);
                                   if(theInters.retrieve(1).returnType()==-1)  return false;
                                   out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                                   out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                                   return(out1 || out2);
                                   break;
                          case 30: Vert1.setType(0);
                                   Vert1.setValues(0,x1);
                                   Vert1.setValues(1,y1);
                                   return(isValidPointIncidence(Vert1,theOther,1,0));
                                   break;
                          case 1:
                          case 2:
                          case 3:
                          case 31:
                          case 32:
                          case 33: return true;
                                   break;
                    }
          default:  return true;
    }
}

//given an arc and another object, returns true if the object is incident on the arc
bool isValidArcIncidence(Vertex &theArc, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, r1, x2, y2, x3, y3, m2, r2, sep;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theArc.returnDegValueByName(0);
    y1=theArc.returnDegValueByName(1);
    r1=theArc.returnDegValueByName(2);  
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);
    r2=theOther.returnDegValueByName(2);
    
    switch(part1*10+part2)
    {
          case 0:  sep=getDistance(x1,y1,x2,y2);
                   if((sep+r1)<r2 || (sep+r2)<r1) return false;
                   if(sep>(r1+r2)) return false;
                   theInters.append(Vert1);
                   theInters.append(Vert2);
                   getIntersectionCircleCircle(theInters, x1, y1, r1, x2, y2, r2);
                   if(theInters.retrieve(1).returnType()==-1)  return false;
                   out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                   out1=out1 && isValidPointIncidence(theInters.retrieve(1), theArc,1,0);
                   out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                   out2=out2 && isValidPointIncidence(theInters.retrieve(2), theArc,1,0);
                   return(out1 || out2);
                   break;
          case 1:  x3=x2+r2*theOther.returnDegValueByName(4);
                   y3=y2+r2*theOther.returnDegValueByName(5);
                   Vert1.setType(0);
                   Vert1.setValues(0,x3);
                   Vert1.setValues(1,y3);
                   return(isValidPointIncidence(Vert1,theArc,1,0));
                   break;
          case 2:  x3=x2+r2*theOther.returnDegValueByName(6);
                   y3=y2+r2*theOther.returnDegValueByName(7);
                   Vert1.setType(0);
                   Vert1.setValues(0,x3);
                   Vert1.setValues(1,y3);
                   return(isValidPointIncidence(Vert1,theArc,1,0));
                   break;
          case 3:  Vert1.setType(0);
                   Vert1.setValues(0,x2);
                   Vert1.setValues(1,y2);
                   return(isValidPointIncidence(Vert1,theArc,1,0));
                   break;
          case 10: x3=x1+r1*theOther.returnDegValueByName(4);
                   y3=y1+r1*theOther.returnDegValueByName(5);
                   Vert1.setType(0);
                   Vert1.setValues(0,x3);
                   Vert1.setValues(1,y3);
                   return(isValidPointIncidence(Vert1,theOther,1,0));
                   break;
          case 20: x3=x1+r1*theOther.returnDegValueByName(6);
                   y3=y1+r1*theOther.returnDegValueByName(7);
                   Vert1.setType(0);
                   Vert1.setValues(0,x3);
                   Vert1.setValues(1,y3);
                   return(isValidPointIncidence(Vert1,theOther,1,0));
                   break;
          case 30: Vert1.setType(0);
                   Vert1.setValues(0,x1);
                   Vert1.setValues(1,y1);
                   return(isValidPointIncidence(Vert1,theOther,1,0));
                   break;
          case 11:
          case 12:
          case 13:
          case 21:
          case 22:
          case 23:
          case 31:
          case 32:
          case 33: return true;
                   break;
    }
}

// given the edge, if it is not imaginary returns true (if the constraint
// is not imaginary if must be satisfied after solution, otherwise the 
// method calls the correct isValidIncidence method from above and returns 
// true only if the incidence is met with the current values
bool isValidConstraint(Graph &graph0, Edge &theEdge, Cluster &theCluster)
{
    Vertex end1, end2;
    int v1, v2, type1, type2, part1, part2;

    v1=theEdge.returnEnd1();
    v2=theEdge.returnEnd2();

    part1=theEdge.returnPart1();
    part2=theEdge.returnPart2();
    
    end1=graph0.returnVertByName(v1);
    end2=graph0.returnVertByName(v2);

    type1=end1.returnType();
    type2=end2.returnType();

    if(!isImaginary(graph0, theEdge)) return true;

    switch(type1*10+type2)
    {
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:  return(isValidPointIncidence(end1,end2,part1,part2));
                   break;
          case 10:
          case 20:
          case 30:
          case 40:
          case 50: return(isValidPointIncidence(end2,end1,part2,part1));
                   break;
          case 11:
          case 12:
          case 13:
          case 14:
          case 15: return(isValidLineIncidence(end1,end2,part1,part2));
                   break;
          case 21:
          case 31:
          case 41:
          case 51: return(isValidLineIncidence(end2,end1,part2,part1));
                   break;
          case 22:
          case 23:
          case 24:
          case 25: return(isValidRayIncidence(end1,end2,part1,part2));
                   break;
          case 32:
          case 42:
          case 52: return(isValidRayIncidence(end2,end1,part2,part1));
                   break;
          case 33:
          case 34:
          case 35: return(isValidLSIncidence(end1,end2,part1,part2));
                   break;
          case 43:
          case 53: return(isValidLSIncidence(end2,end1,part2,part1));
                   break;
          case 44:
          case 45: return(isValidCircleIncidence(end1,end2,part1,part2));
                   break;
          case 54: return(isValidCircleIncidence(end2,end1,part2,part1));
                   break;
          case 55: return(isValidArcIncidence(end1,end2,part1,part2));
                   break;
    }
}

//returns true if the constraint theEdge, is imaginary and must be checked to see if it is satisfied
bool isImaginary(Graph &graph0, Edge &theEdge)
{
   int end1Type, end2Type;
   int part1, part2;

   if(theEdge.returnType()!=1) return false;

   if(theEdge.returnName()==999)
   {
     end1Type=type1;
     end2Type=type2;
   }
   else
   {
     end1Type=graph0.returnVertByName(theEdge.returnEnd1()).returnType();
     end2Type=graph0.returnVertByName(theEdge.returnEnd2()).returnType();
   }
   part1=theEdge.returnPart1();
   part2=theEdge.returnPart2();

   if(part1!=0 && part2!=0) return false;

   switch(end1Type*10+end2Type)
   {
         case 0:
         case 2:
         case 3:
         case 5:
         case 20:
         case 22:
         case 23:
         case 25:
         case 30:
         case 32:
         case 33:
         case 35:
         case 50:
         case 52:
         case 53:
         case 55:  return(part1==0 || part2==0);
                   break;
         case 15:
         case 42:
         case 43:
         case 45:  return(part2==0);
                   break;
         case 51:
         case 24:
         case 34:
         case 54:  return(part1==0);
                   break;
         default:  return(part1==0 && part2==0);
                   break;
   }
   return false;
}

//for the given cluster, returns the index of its first child with the name vName,
//it no such child exists returns -1
int getChildIndexByName(Cluster &theCluster, int vName)
{
    int i, length;

    length=theCluster.children.returnLen();
    for(i=1;i<=length;i++)
       if(theCluster.children.retrieve(i).returnName()==vName) return i;

    return -1;
}

//returns true if Name is a clild of theCluster
bool inOriginalV(int Name, Cluster &theCluster)
{
    List<int> theOrig;

    theOrig=theCluster.returnOrig();

    if (theOrig.hasElem(Name)) return true;

    return false;
}

//returns the ID of the first immediate child of theCluster to contain the given original vertex ID
int getChildNameWithVertex(Cluster &theCluster, int theV)
{

    int length, i;
    List<int> theOrig;

    length=theCluster.children.returnLen();

    for(i=1;i<=length;i++)
    {
       if(inOriginalV(theV,theCluster.children.retrieve(i)))
       {
          return theCluster.children.retrieve(i).returnName();
       }
    }
    return -1;

}

//rotates the object stored in v0, and rotates using the rotation matrix stored in theCluster
void rotate(Cluster &theCluster, Vertex &v0)
{
    int type;
    float x, y, z, c, d, e, m, x2, y2, x2New, y2New, xNew, yNew, zNew, mNew;
    float sin, cos, p, q, f, siny, cosy, sinz, cosz, cNew, dNew, eNew;


    type=v0.returnType();

/*
    if(theCluster.returnType()==2 && v0.returnType()==6)
    {
	  if(v0.returnName()==theCluster.returnValue(6))
	  {
		v0.setValues(0,theCluster.returnValue(0));
		v0.setValues(1,theCluster.returnValue(1));
                v0.setValues(2,theCluster.returnValue(2));
		return;
	  }		
	  if(v0.returnName()==theCluster.returnValue(7))
	  {
		v0.setValues(0,theCluster.returnValue(3));
                v0.setValues(1,theCluster.returnValue(4));
                v0.setValues(2,theCluster.returnValue(5));
		return;
	  }
    }

*/
	
    switch(type)
    {
          case 0:
          case 4:   x=v0.returnDegValueByName(0);
                    y=v0.returnDegValueByName(1);
                    p=theCluster.returnValue(0);
                    q=theCluster.returnValue(1);
                    cos=theCluster.returnValue(3);
                    sin=theCluster.returnValue(4);
                    xNew=(x*cos)-(y*sin)+p;
                    yNew=(y*cos)+(x*sin)+q;
                    v0.setValues(0,xNew);
                    v0.setValues(1,yNew);
                    break;
          case 1:   x=v0.returnDegValueByName(0);
                    y=v0.returnDegValueByName(1);
                    m=v0.returnDegValueByName(4);
                    x2=x+1.0;
                    y2=y+m;
                    p=theCluster.returnValue(0);
                    q=theCluster.returnValue(1);
                    cos=theCluster.returnValue(3);
                    sin=theCluster.returnValue(4);
                    xNew=(x*cos)-(y*sin)+p;
                    yNew=(y*cos)+(x*sin)+q;
                    x2New=(x2*cos)-(y2*sin)+p;
                    y2New=(y2*cos)+(x2*sin)+q;
                    mNew=(y2New-yNew)/(x2New-xNew);
                    v0.setValues(0,xNew);
                    v0.setValues(1,yNew);
                    v0.setValues(4,mNew);
                    break;
	  case 2:
          case 3:   x=v0.returnDegValueByName(0);
                    y=v0.returnDegValueByName(1);
	            x2=v0.returnDegValueByName(2);
                    y2=v0.returnDegValueByName(3);
		    p=theCluster.returnValue(0);
                    q=theCluster.returnValue(1);
                    cos=theCluster.returnValue(3);
                    sin=theCluster.returnValue(4);
 		    xNew=(x*cos)-(y*sin)+p;
                    yNew=(y*cos)+(x*sin)+q;
                    x2New=(x2*cos)-(y2*sin)+p;
                    y2New=(y2*cos)+(x2*sin)+q;
		    v0.setValues(0,xNew);
                    v0.setValues(1,yNew);
		    v0.setValues(2,x2New);
                    v0.setValues(3,y2New);
		    break;
	  case 6:   x=v0.returnDegValueByName(0); 
                    y=v0.returnDegValueByName(1);
		    z=v0.returnDegValueByName(2);
		    p=theCluster.returnValue(0);
                    q=theCluster.returnValue(1);
                    f=theCluster.returnValue(2);
                    cos=theCluster.returnValue(3);
                    sin=theCluster.returnValue(4);
                    cosy=theCluster.returnValue(5);
                    siny=theCluster.returnValue(6);
                    cosz=theCluster.returnValue(7);
                    sinz=theCluster.returnValue(8);
		    xNew=(cosy*cosz*x)+(cosy*sinz*y)-(siny*z)+p;
                    yNew=((sin*siny*cosz-cos*sinz)*x)+((cos*cosz+sin*siny*sinz)*y)+(sin*cosy*z)+q;
                    zNew=((sin*sinz+cos*siny*cosz)*x)+((cos*siny*sinz-sin*cosz)*y)+(cos*cosy*z)+f;
                    v0.setValues(0,xNew);
                    v0.setValues(1,yNew);
                    v0.setValues(2,zNew);
		    break;
	   case 7:  x=v0.returnDegValueByName(0);
                    y=v0.returnDegValueByName(1); 
                    z=v0.returnDegValueByName(2);
		    c=v0.returnDegValueByName(3);
                    d=v0.returnDegValueByName(4);
                    e=v0.returnDegValueByName(5);
		    p=theCluster.returnValue(0);
                    q=theCluster.returnValue(1);
                    f=theCluster.returnValue(2);
                    cos=theCluster.returnValue(3);
                    sin=theCluster.returnValue(4); 
                    cosy=theCluster.returnValue(5);
                    siny=theCluster.returnValue(6);
                    cosz=theCluster.returnValue(7);
                    sinz=theCluster.returnValue(8);
                    xNew=(cosy*cosz*x)+(cosy*sinz*y)-(siny*z)+p;
                    yNew=((sin*siny*cosz-cos*sinz)*x)+((cos*cosz+sin*siny*sinz)*y)+(sin*cosy*z)+q;
                    zNew=((sin*sinz+cos*siny*cosz)*x)+((cos*siny*sinz-sin*cosz)*y)+(cos*cosy*z)+f;
                    cNew=(cosy*cosz*c)+(cosy*sinz*d)-(siny*e)+p;
                    dNew=((sin*siny*cosz-cos*sinz)*c)+((cos*cosz+sin*siny*sinz)*d)+(sin*cosy*e)+q;
                    eNew=((sin*sinz+cos*siny*cosz)*c)+((cos*siny*sinz-sin*cosz)*d)+(cos*cosy*e)+f;
		    v0.setValues(0,xNew);
                    v0.setValues(1,yNew);
                    v0.setValues(2,zNew);
		    v0.setValues(3,cNew);
                    v0.setValues(4,dNew);
                    v0.setValues(5,eNew);
                    break;
    }
}

//recursively determines the position of any object in the given cluster,
//by rotating it through all the past rotations up from the children
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex, int location)
{
   int i;
   int childIndex;

   if(theCluster.returnName()==theVertex.returnName())
   {
     theVertex=graph0.returnVertByName(theVertex.returnName());
     for(i=0;i<9;i++)
        theVertex.setValues(i,theCluster.returnValue(i));

   }
   else
   {
     childIndex=getChildIndexByName(theCluster, getChildNameWithVertex(theCluster, theVertex.returnName()));
     //theCluster.children.retrieve(childIndex).setToCluster(theCluster);
     setValueInCluster(graph0, theCluster.children.retrieve(childIndex), theVertex, location+1);
     if(location!=1) rotate(theCluster, theVertex);
   }
}

//the setValueInCluster method is interpreted to set the value given vertex in the currnet cluster
//this second method prevents the recursive method from rotating the vertex through the cluster
//in which its called
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex)
{
    setValueInCluster(graph0, theCluster, theVertex, 1);
}

//assigns a given value to a freedeg in Vertex
void setValue(Vertex &theVertex, int index, float theValue)
{
   theVertex.setValues(index, theValue);
}

//if the cluster is singleton, sets the values in the corresponding vertex in graph0
//to the values stored in theCluster, otherwise recursively calls itself on every one 
//of its children with populateForest
void populateCluster(Graph &graph0, Cluster &theCluster)
{
   int length=theCluster.children.returnLen();
   int name;
   int i;

   if(theCluster.returnOrig().returnLen()==1)
   {
      name=theCluster.returnName();
      for(i=0;i<9;i++)
         setValue(graph0.returnVertByName(name),i,theCluster.returnValue(i));
   }
   if(length>0) populateForest(graph0, theCluster.children);
}

//calls populate cluster for every cluster in SolverTrees
void populateForest(Graph &graph0, List<Cluster> &SolverTrees)
{
    int i, numTrees;

    numTrees=SolverTrees.returnLen();
    for(i=1; i<=numTrees; i++)
       populateCluster(graph0, SolverTrees.retrieve(i));
}

//begins the recursion completed in populateForest and PopulateCluster
void populateGraph(Graph &graph0, List<Cluster> &SolverTrees)
{
   populateForest(graph0, SolverTrees);
}

//THIS METHOD HAS BEEN OUTDATED BY THE USE OF THE JAVA NATIVE INTERFACE
//prints the graph data into a text file in a format that can be read by the sketcher
//when printNum is true the ID of theCluster is printed first
void generateOutput(Graph &graph0, Cluster &theCluster, ostream &outfile, bool printNum)
{
   int i, length;
   Vertex currVert;
   float slope, angle1, angle2;
   float x,y,tempValue, tempValue2;

   if(printNum) outfile<<theCluster.returnOrigLen()<<endl;
   length=graph0.returnNumVer();
   for(i=1;i<=length;i++)
   {

      currVert=graph0.returnVertByIndex(i);
      if(inOriginalV(currVert.returnName(),theCluster))
      {
        outfile<<currVert.returnName()<<endl;
        x=currVert.returnDegValueByName(0);
        y=currVert.returnDegValueByName(1);
        outfile<<x<<endl;
        outfile<<y<<endl;
        switch(currVert.returnType())
        {
              case 0:   break;
              case 1:   tempValue=currVert.returnDegValueByName(4);
                        outfile<<x+1<<endl;
			outfile<<y+tempValue<<endl;
                        break;
              case 2:   tempValue=currVert.returnDegValueByName(4);
			tempValue2=currVert.returnDegValueByName(5);
                        outfile<<x+tempValue<<endl;
			outfile<<y+tempValue2<<endl;
                        break;
              case 3:   outfile<<currVert.returnDegValueByName(2)<<endl;
                        outfile<<currVert.returnDegValueByName(3)<<endl;
                        break;
              case 4:   tempValue=currVert.returnDegValueByName(2);
 			if(tempValue<0) tempValue=-tempValue;
                        outfile<<tempValue<<endl;
                        break;
              case 5:   angle1=getAngle(currVert.returnDegValueByName(4),currVert.returnDegValueByName(5));
                        angle1=getAngle(currVert.returnDegValueByName(6),currVert.returnDegValueByName(7));
                        outfile<<currVert.returnDegValueByName(2)<<endl;
                        outfile<<currVert.returnDegValueByName(3)<<endl;
                        outfile<<angle1<<endl;
                        outfile<<angle2<<endl;
                        break;
        }
      }
   }
}

/*  Same as above but places the output values in the transfer arrays, inputInts and 
    inputFloats.  The format is as follows, if printnum, the ID of theCluster is output,
    otherwise nothing.  Then for every original vertex in theCluster, the ID is printed,
    then the following information, for every point the x then y coordinate, followed by
    information depending on the type of the object:

       points - nothing
       lines - second point on line
       rays - second point on ray
       line segment - second endpoint
       circle - radius
       arc - points of the start and end of the arc, x coordinates first
       point3D - z coord
*/
void generateOutputToArray(Graph &graph0, Cluster &theCluster, bool printNum)
{   
   int i, length;
   Vertex currVert;   
   float slope, angle1, angle2;
   float x,y,tempValue, tempValue2,x2,y2;
   
   graph0.output(cout);

   int memPos=currPosI;
   int outCount=0;

   if(printNum) currPosI++;

   int temp;

   cout<<"Memory location of count="<<memPos<<endl;
   
//   if(printNum) inputInts[currPosI++]=theCluster.returnOrigLen();
   length=graph0.returnNumVer();
   for(i=1;i<=length;i++)
   {
    
      currVert=graph0.returnVertByIndex(i);
      if(currVert.returnIgnore())
 	cout<<"Ignore=true for vertex="<<currVert.returnName()<<endl;
      else cout<<"Ignore=false for vertex="<<currVert.returnName()<<endl;

      if(currVert.returnIgnore()) continue;
      if(inOriginalV(currVert.returnName(),theCluster))
      {
	outCount++;
        inputInts[currPosI++]=currVert.returnName();
        x=currVert.returnDegValueByName(0);
        y=currVert.returnDegValueByName(1);
	cout<<"Name: "<<currVert.returnName();
        inputFloats[currPosF++]=x;
        inputFloats[currPosF++]=y;
	cout<<", x="<<x;
	cout<<", y="<<y;
        switch(currVert.returnType())
        {
              case 0:   break;
              case 1:   tempValue=currVert.returnDegValueByName(4);
			x2=inputFloats[currPosF++]=x+1;
                        y2=inputFloats[currPosF++]=y+tempValue;
			cout<<", x2="<<x2;
			cout<<", y2="<<y2;
                        break;
              case 2:   tempValue=currVert.returnDegValueByName(4);
                        tempValue2=currVert.returnDegValueByName(5);
                        inputFloats[currPosF++]=x+tempValue;
                        inputFloats[currPosF++]=y+tempValue2;
                        break;
              case 3:   inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(3);
                        break;
              case 4:   tempValue=currVert.returnDegValueByName(2);
                        if(tempValue<0) tempValue=-tempValue;
                        inputFloats[currPosF++]=tempValue;
                        break;
              case 5:   angle1=getAngle(currVert.returnDegValueByName(4),currVert.returnDegValueByName(5));
                        angle2=getAngle(currVert.returnDegValueByName(6),currVert.returnDegValueByName(7));
                        inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(3);
	                inputFloats[currPosF++]=angle1;
                        inputFloats[currPosF++]=angle2;
                        break;
	      case 6:   inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        break;
              case 7:   inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(3);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(4);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(5);
                        break;

        }
	cout<<endl;
      }  
   }

   cout<<"Count of bifurcations sent="<<outCount<<endl;

   if(printNum) inputInts[memPos]=outCount;
}

//returns the substring of theString starting at pos with length length
string getSubString(string theString, int pos, int length)
{
    int i;
    string output;

    output="";

    if((pos+length)>theString.size()) return "";

    for(i=1;i<=length;i++)
    {
       output+=theString[pos];
       pos++;
    }

    return output;
}

//replaces all occurances of toReplace with replaceWith in theReplace
string replaceAll(string toReplace, string theReplace, string replaceWith)
{
    string output=toReplace;
    int theReplaceLen, replaceWithLen, pos=0;

    theReplaceLen=theReplace.size();
    replaceWithLen=replaceWith.size();

    while((pos+theReplaceLen)<=output.size())
    {
         if(getSubString(output,pos,theReplaceLen)==theReplace)
         {
           output.replace(pos,theReplaceLen,replaceWith);
           pos+=replaceWithLen-1;
         }
         pos++;
    }

    return output;
}

//returns a string in Maple format of the variable list
string getVarString()
{
   string output="{";
   int length, i;

   length=vars.returnLen();

   for(i=1;i<=length;i++)
   {
      if(i!=1) output+=",";
      output+=vars.retrieve(i);
   }

   output+="}";
   return output;
}

//converts a long to a string
string toString(long a)
{
   long temp;
   string theNumber, outString;

   if(a==0) return "0";

   temp = a;
   outString = "";

   while (temp>=1)
   {
      switch (temp%10)
      {
          case 1: theNumber = "1";
                  break;
          case 2: theNumber = "2";
                  break;
          case 3: theNumber = "3";
                  break;
          case 4: theNumber = "4";
                  break;
          case 5: theNumber = "5";
                  break;
          case 6: theNumber = "6";
                  break;
          case 7: theNumber = "7";
                  break;
          case 8: theNumber = "8";
                  break;
          case 9: theNumber = "9";
                  break;
          case 0: theNumber = "0";
                  break;
      }
      outString=theNumber+outString;
      temp/=10;
   }
   if(a>=0) return outString;
   else return("-"+outString);
}

//converts a int to a string
string toString(int a)
{
    return toString((long) a);
}

//converts a double to a string
string toString(double a)
{ 
    string output;
    char temp[100];
    
    sprintf(temp,"%f",a);   

    output.assign(temp);
 
    return output;
}

//converts a float to a string
string toString(float a)
{
    return toString((double) a);
}

//shells to maple and calls the solver
void shellMaple()
{
    string outputString;

    outputString="maple <input.txt> output.txt";
    remove("output.txt");
    system(outputString.c_str());   
}

/*
//shells to maple and calls the solver
void shellMaple()
{ 
    ofstream output;
    string outputString;
    
    output.open("startup.m");
    output<<"xStart=zeros("+toString(vars.returnLen())+",1);\n";
    output<<"fun=@dplanFun;\nx=fsolve(fun, xStart);\nsave data.out xStart -ascii;\nquit;";
    output.close();

    outputString="matlab -nojvm -nosplash";
    //remove("output.txt");
    system(outputString.c_str());
}
*/

//checks that a given bifurcation string is valid by checking that it contains
//only the valid characters below
bool validBifur(string theString)
{
    int i, length;
    bool output;

    output=true;

    length=theString.size();

    for(i=0;i<length;i++)
    {
       switch(theString[i])
       {
             case '0':
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
             case '6':
             case '7':
             case '8':
             case '9':
             case 'q':
             case 'p':
             case 's':
             case 't':
             case 'x':
             case 'y':
             case 'r':
             case 'm':
             case 'a':
             case 'b':
             case 'c':
             case 'd':
             case 'w':
             case 'v':
             case '.':
             case '=':
             case ',':
             case 'e':
	     case 'l':
	     case 'g':
             case 'f':
             case 'h':
             case 'j':
             case 'k':
             case 'n':
	     case 'z':
             case '-':  break;
             default:   output=false;
                        break;
       }
    }
    return output;
}

//reads the bifurcations from filename and sets them in theCluster, fsolve is used to
//differentiate the two different cases.  When true the data is read as if the output comes
//Maple's numeric solver, otherwise as if the symbolic solver was used.
void setBifurs(string filename, Cluster& theCluster, bool usingfSolve)
{
    ifstream infile;
    int caretCount=0;
    string output="";
    char in;
    int tag=0;
    string temp;
    List<string> theList;

    theList.makeEmpty();

    infile.open(filename.c_str());

    if(usingfSolve)
    {
      while(caretCount!=3)
      {
            if(infile.get()=='>') caretCount++;
            if(infile.eof()) return;
      }
      while(infile.get()!=';') if(infile.eof()) return;
      
      infile>>in;
      while(in!='{')
      {
        if(infile.eof()) return;
        if(in=='f') return;
        infile>>in;
      }

      infile>>in;  
      while(tag==0)
      {
         while(in!='}')
         {
             temp="";  
             temp+=in; 
             temp+=" ";
             if(temp!="\\ ") output+=in;
             infile>>in;
             if(infile.eof()) return;
         }
         if(validBifur(output)) theList.append(output);
         output="";
         while(in!='{' && in!='q')
         {
             infile>>in;
             if(infile.eof()) return;
         }
         if(in=='q') tag=1;
         if(in=='{') infile>>in;
      }
      infile.close();
      theCluster.setBifurs(theList);
      return;      
    }

    while(caretCount!=2)
    {
          if(infile.get()=='%') caretCount++;
          if(infile.eof()) return;
    }

    while(infile.get()!='{') if(infile.eof()) return;

    infile>>in;
    while(tag==0)
    {
       while(in!='}')
       {   
           temp="";
           temp+=in;
           temp+=" ";
           if(temp!="\\ ") output+=in;
           infile>>in;
	   if(infile.eof()) return;
       }
       if(validBifur(output)) theList.append(output);
       if(theList.returnLen()==5) break;
       output="";
       while(in!='{' && in!='q')
       {
           infile>>in;
           if(infile.eof()) return;
       }
       if(in=='q') tag=1;
       if(in=='{') infile>>in;
    }

    infile.close();
    theCluster.setBifurs(theList);
}

/*  All of the following methods generate the equation strings used to solve the equations in 
    Maple.  For each constraint type, distance, incidence, tangent, parallel, angle and
    perpendicular, there is a corresponding method of the form getDistanceEQ etc.  Each method
    simply a long case statement that considers the type of object at each endpoint of the edge
    and generates the appropriate equation.  In addition, whenever a constraint is generated
    any variables used are added to the global list of variables, vars, and are used later to
    generate a list of variables required as input for Maple.
*/
string getDistanceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, string theDist)
{
    string x1,x2,y1,y2,coeff,m,v,w,output, firstVal;
    int v1Name, v2Name;
    int type1, type2;
    float value;

    value=theEdge.returnValue();
    if(value<0) value=-value;
    if (theDist=="") firstVal=toString(sqrt(value));
    else firstVal=theDist;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    x1="x"+toString(v1Name);
    y1="y"+toString(v1Name);
    x2="x"+toString(v2Name);
    y2="y"+toString(v2Name);
    switch(type1*10+type2)
    {
          case 2:
          case 12:
          case 20:
          case 21:
          case 22:
          case 23:
          case 24:
          case 25:
          case 32:
          case 42:
          case 52: if(type1==2)
                   {
                     v="v"+toString(v1Name);
                     w="w"+toString(v1Name);
                     m="("+w+"/"+v+")";
                     output="("+firstVal+")^2=(("+y1+"+"+y2+"-"+m+"*("+x1+"-"+x2+"))/(1+"+m+"^2))";
                   }
                   else
                   {
                     v="v"+toString(v2Name);
                     w="w"+toString(v2Name);
                     m="("+w+"/"+v+")";
                     output="("+firstVal+")^2=(("+y2+"+"+y1+"-"+m+"*("+x2+"-"+x1+"))/(1+"+m+"^2))";
                   }
                   if(!vars.hasElem(v)) vars.append(v);
                   if(!vars.hasElem(w)) vars.append(w);
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;

          case 1:
          case 3:
          case 10:
          case 11:
          case 13:
          case 14:
          case 15:
          case 30:
          case 31:
          case 33:
          case 34:
          case 35:
          case 41:
          case 43:
          case 51:
          case 53: if(type1==1 || type1==3)
                   {
                     m="m"+toString(v1Name);
                     output="("+firstVal+")^2=(("+y1+"+"+y2+"-"+m+"*("+x1+"-"+x2+"))/(1+"+m+"^2))";
                   }
                   else
                   {
                     m="m"+toString(v2Name);
                     output="("+firstVal+")^2=(("+y2+"+"+y1+"-"+m+"*("+x2+"-"+x1+"))/(1+"+m+"^2))";
                   }
                   if(!vars.hasElem(m)) vars.append(m);
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;
          default: if(theDist=="") firstVal=toString(value);
		   else firstVal=theDist;
		   output="("+firstVal+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;
    }
    return output;
}

string getDistance3DEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, string theDist)
{
    string x1,x2,y1,y2,z1,z2,output,firstVal;
    int v1Name, v2Name;
    int type1, type2;
    float value;

    value=theEdge.returnValue();
    if(value<0) value=-value;
    if (theDist=="") firstVal=toString(sqrt(value));
    else firstVal=theDist;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    x1="x"+toString(v1Name);
    y1="y"+toString(v1Name);
    z1="z"+toString(v1Name);
    x2="x"+toString(v2Name);
    y2="y"+toString(v2Name);
    z2="z"+toString(v2Name);
    switch(type1*10+type2)
    {
          default: if(theDist=="") firstVal=toString(value);
                   else firstVal=theDist;
		   output="("+firstVal+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2+("+z1+"-"+z2+")^2)";
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(z1)) vars.append(z1);       
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   if(!vars.hasElem(z2)) vars.append(z2);       
                   break;
    }
    return output;
}


string getAngle3DEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge) //, string theDist)
{
    string x1,x2,y1,y2,z1,z2,output,firstVal,c1,c2,d1,d2,e1,e2;
    int v1Name, v2Name;
    int type1, type2;
    float value;

    value=theEdge.returnValue();
    if(value<0) value=-value;
    value=value*PI/180.0;
    firstVal=toString(cos(value));

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    x1="x"+toString(v1Name);
    y1="y"+toString(v1Name);
    z1="z"+toString(v1Name);
    x2="x"+toString(v2Name);
    y2="y"+toString(v2Name);
    z2="z"+toString(v2Name);
    c1="c"+toString(v1Name);
    d1="d"+toString(v1Name);
    e1="e"+toString(v1Name);
    c2="c"+toString(v2Name);
    d2="d"+toString(v2Name);
    e2="e"+toString(v2Name);


    switch(type1*10+type2)
    {
        default: output=firstVal+"=("+x1+"-"+c1+")*("+x2+"-"+c2+")+";
		 output+="("+y1+"-"+d1+")*("+y2+"-"+d2+")+";
                 output+="("+z1+"-"+e1+")*("+z2+"-"+e2+")";

                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(z1)) vars.append(z1);       
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   if(!vars.hasElem(z2)) vars.append(z2);       
                   break;
    }
    return output;
}

//returns an equation for a tangency constraint
string getTangencyEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    string output="", dist="", r1, r2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    switch(type1*10+type2)
    {
           case 35:
           case 25:
           case 15:
           case 34:
           case 24:
           case 14:  dist="r"+toString(v2Name);
                     if(!vars.hasElem(dist)) vars.append(dist);
                     break;
           case 53:
           case 52:
           case 51:
           case 43:
           case 42:
           case 41:  dist="r"+toString(v1Name);
                     if(!vars.hasElem(dist)) vars.append(dist);
                     break;
           case 55:
           case 44:  dist="r"+toString(v1Name)+"+r"+toString(v2Name);
                     r1="r"+toString(v1Name);
                     r2="r"+toString(v2Name);
                     if(!vars.hasElem(r1)) vars.append(r1);
                     if(!vars.hasElem(r2)) vars.append(r2);
                     break;
    }
    if(dist!="") output=getDistanceEQ(vEnd1, vEnd2, theEdge, dist);
    return output;
}

//returns an equation corresponding to an incidence constraint
string getIncidenceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name, part1, part2;;
    string output="", dist;
    string x1, y1, a1, b1, c1, d1, e1, zz1, g1, l1, m1, r1, v1, w1;
    string x2, y2, a2, b2, c2, d2, e2, zz2, g2, l2, m2, r2, v2, w2;
    string z1, z2;


    type1=vEnd1.returnType();
    type2=vEnd2.returnType();
    part1=theEdge.returnPart1();
    part2=theEdge.returnPart2();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    z1=toString(v1Name);
    z2=toString(v2Name);
    x1="x"+z1;
    y1="y"+z1;
    zz1="z"+z1;
    a1="a"+z1;
    b1="b"+z1;
    c1="c"+z1;
    d1="d"+z1; 
    e1="e"+z1;
    g1="g"+z1;
    l1="l"+z1;
    m1="m"+z1;
    r1="r"+z1;
    v1="v"+z1;
    w1="w"+z1;
    x2="x"+z2;
    y2="y"+z2;
    zz2="z"+z2;
    a2="a"+z2;
    b2="b"+z2;
    c2="c"+z2;
    d2="d"+z2;
    e2="e"+z2;
    g2="g"+z2;
    l2="l"+z2;
    m2="m"+z2;
    r2="r"+z2;
    v2="v"+z2;
    w2="w"+z2;
    /*  Key to above lettering:

        (x,y) - base point for all shapes
        (c,d) - second point
        (p,q) - translation point for clusters
        (v,w) - first angle
        (a,b) - second angle
        (t,s) - translation angle for clusters
          r   - radius
          g   - separtion angle
          l   - length
    */

    switch(type1*10+type2)
    {
          case 67: if(part2==1)
		   {
		     output=x1+"="+x2+","+y1+"="+y2+","+zz1+"="+zz2;
		     if(!vars.hasElem(x1)) vars.append(x1);
                     if(!vars.hasElem(y1)) vars.append(y1);
                     if(!vars.hasElem(x2)) vars.append(x2);
                     if(!vars.hasElem(y2)) vars.append(y2);
		     if(!vars.hasElem(zz1)) vars.append(zz1);
                     if(!vars.hasElem(zz2)) vars.append(zz2);
		   }
		   else if(part2==2)
		   {
                     output=x1+"="+c2+","+y1+"="+d2+","+zz1+"="+e2;
                     if(!vars.hasElem(x1)) vars.append(x1);
                     if(!vars.hasElem(y1)) vars.append(y1);
                     if(!vars.hasElem(c2)) vars.append(c2);
                     if(!vars.hasElem(d2)) vars.append(d2);  
                     if(!vars.hasElem(zz1)) vars.append(zz1);
                     if(!vars.hasElem(e2)) vars.append(e2);
		   }		
		   break;
          case 76: if(part1==1)
                   {
                     output=x1+"="+x2+","+y1+"="+y2+","+zz1+"="+zz2;
                     if(!vars.hasElem(x1)) vars.append(x1);
                     if(!vars.hasElem(y1)) vars.append(y1);
                     if(!vars.hasElem(x2)) vars.append(x2);
                     if(!vars.hasElem(y2)) vars.append(y2);  
                     if(!vars.hasElem(zz1)) vars.append(zz1);
                     if(!vars.hasElem(zz2)) vars.append(zz2);
                   }
                   else if(part1==2)
                   {
                     output=c1+"="+x2+","+d1+"="+y2+","+e1+"="+zz2; 
                     if(!vars.hasElem(c1)) vars.append(c1);
                     if(!vars.hasElem(d1)) vars.append(d1);
                     if(!vars.hasElem(x2)) vars.append(x2);
                     if(!vars.hasElem(y2)) vars.append(y2);
                     if(!vars.hasElem(e1)) vars.append(e1);
                     if(!vars.hasElem(zz2)) vars.append(zz2);
                   }
                   break;
	  case 0:  output=x1+"="+x2+","+y1+"="+y2;
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;
          case 1:  output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                   if(!vars.hasElem(m2)) vars.append(m2);
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;
          case 10: output="("+y2+"-"+y1+")="+m1+"*("+x2+"-"+x1+")";
                   if(!vars.hasElem(m1)) vars.append(m1);
                   if(!vars.hasElem(x1)) vars.append(x1);
                   if(!vars.hasElem(y1)) vars.append(y1);
                   if(!vars.hasElem(x2)) vars.append(x2);
                   if(!vars.hasElem(y2)) vars.append(y2);
                   break;
          case 2:  if(part2==0)
                   {
                     output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                     if(!vars.hasElem(m2)) vars.append(m2);
                     if(!vars.hasElem(x1)) vars.append(x1);
                     if(!vars.hasElem(y1)) vars.append(y1);
                     if(!vars.hasElem(x2)) vars.append(x2);
                     if(!vars.hasElem(y2)) vars.append(y2);
                   }
                   else
                   {
                      output=x1+"="+x2+","+y1+"="+y2;
                      if(!vars.hasElem(x1)) vars.append(x1);
                      if(!vars.hasElem(y1)) vars.append(y1);
                      if(!vars.hasElem(x2)) vars.append(x2);
                      if(!vars.hasElem(y2)) vars.append(y2);
                   }
                   break;
          case 20: if(part1==0)
                   {
                     output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                     if(!vars.hasElem(v1)) vars.append(v1);
                     if(!vars.hasElem(w1)) vars.append(w1);
                     if(!vars.hasElem(x1)) vars.append(x1);
                     if(!vars.hasElem(y1)) vars.append(y1);
                     if(!vars.hasElem(x2)) vars.append(x2);
                     if(!vars.hasElem(y2)) vars.append(y2);
                   }
                   else
                   {
                      output=x1+"="+x2+","+y1+"="+y2;
                      if(!vars.hasElem(x1)) vars.append(x1);
                      if(!vars.hasElem(y1)) vars.append(y1);
                      if(!vars.hasElem(x2)) vars.append(x2);
                      if(!vars.hasElem(y2)) vars.append(y2);
                   }
                   break;
          case 3:  switch (part1*10+part2)
                   {
                          case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 12: output=x1+"="+c2+","+y1+"="+d2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                   }
                   break;
          case 30: switch (part1*10+part2)
                   {
                          case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 21: output=c1+"="+x2+","+d1+"="+y2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 4:  switch (part1*10+part2)
                   {
                          case 10: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 40: switch (part1*10+part2)
                   {
                          case 1:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 5:  switch (part1*10+part2)
                   {
                          case 10: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
          case 50: switch (part1*10+part2)
                   {
                          case 1: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(x2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(x2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 11: output="";
                   break;
          case 12: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 21: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 10: output="("+y2+"-"+y1+")="+m2+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 13: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 2:  output="("+y1+"-"+d2+")="+m1+"*("+x1+"-"+c2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                   }
                   break;
          case 31: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 20: output="("+d1+"-"+y2+")="+m2+"*("+c1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 14: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 41: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 15: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   break;
                          case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   break;
                          case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 51: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   break;
                          case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 22: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 23: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 2:  output="("+d2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+c2+"-"+x1+")";
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                          case 10: output="("+y2+"-"+y1+")="+m2+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 12: output=x1+"="+c2+","+y1+"="+d2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                   }
                   break;
          case 32: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y2+"-"+y1+")="+m1+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 20: output="("+d1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+c1+"-"+x2+")";
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 21: output=x2+"="+c1+","+y2+"="+d1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   break;
                   }
                   break;
          case 24: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 3:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 42: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 25: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+"("+w1+"/"+v1+")"+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   break;
                          case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+"("+w1+"/"+v1+")"+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   break;
                          case 3:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 52: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+"("+w2+"/"+v2+")"+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   break;
                          case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+"("+w2+"/"+v2+")"+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(y1)) vars.append(r1);
                                   break;
                          case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 33: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 2:  output="("+y1+"-"+d2+")="+m1+"*("+x1+"-"+c2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                          case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 20: output="("+y1+"-"+d2+")="+m2+"*("+x1+"-"+c2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 12: output=x1+"="+c2+","+y1+"="+d2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                          case 21: output=c1+"="+x2+","+d1+"="+y2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 22: output=c1+"="+c2+","+d1+"="+d2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   break;
                   }
                   break;
          case 34: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 20: output="("+r2+")^2=(("+c1+"-"+x2+")^2+("+d1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 23: output=c1+"="+x2+","+d1+"="+y2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 43: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 2:  output="("+r1+")^2=(("+c2+"-"+x1+")^2+("+d2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 32: output=c2+"="+x1+","+d2+"="+y1;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 35: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   break;
                          case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   break;
                          case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m1)) vars.append(m1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 20: output="("+r2+")^2=(("+c1+"-"+x2+")^2+("+d1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 21: output=c1+"="+x2+"+"+v2+"*"+r2+","+d1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 22: output=c1+"="+x2+"+"+a2+"*"+r2+","+d1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 23: output=c1+"="+x2+","+d1+"="+y2;
                                   if(!vars.hasElem(c1)) vars.append(c1);
                                   if(!vars.hasElem(d1)) vars.append(d1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 53: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 2:  output="("+r1+")^2=(("+c2+"-"+x1+")^2+("+d2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   break;
                          case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   break;
                          case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                                   if(!vars.hasElem(m2)) vars.append(m2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(x2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(x2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 31: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 12: output=c2+"="+x1+"+"+v1+"*"+r1+","+d2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(y1)) vars.append(r1);
                                   break;
                          case 22: output=c2+"="+x1+"+"+a1+"*"+r1+","+d2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 32: output=c2+"="+x1+","+d2+"="+y1;
                                   if(!vars.hasElem(c2)) vars.append(c2);
                                   if(!vars.hasElem(d2)) vars.append(d2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                   }
                   break;
          case 44: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 33: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 45: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 1:  output="("+r1+")^2=(("+x2+"+"+v2+"*"+r2+"-"+x1+")^2+("+y2+"+"+w2+"*"+r2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 2:  output="("+r1+")^2=(("+x2+"+"+a2+"*"+r2+"-"+x1+")^2+("+y2+"+"+b2+"*"+r2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 31: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 32: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 33: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 54: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"+"+v1+"*"+r1+"-"+x2+")^2+("+y1+"+"+w1+"*"+r1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 20: output="("+r2+")^2=(("+x1+"+"+a1+"*"+r1+"-"+x2+")^2+("+y1+"+"+b1+"*"+r1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(y1)) vars.append(r1);
                                   break;
                          case 23: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 33: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
          case 55: switch (part1*10+part2)
                   {
                          case 0:  output="";
                                   break;
                          case 1:  output="("+r1+")^2=(("+x2+"+"+v2+"*"+r2+"-"+x1+")^2+("+y2+"+"+w2+"*"+r2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 2:  output="("+r1+")^2=(("+x2+"+"+a2+"*"+r2+"-"+x1+")^2+("+y2+"+"+b2+"*"+r2+"-"+y1+")^2)";
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   break;
                          case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 10: output="("+r2+")^2=(("+x1+"+"+v1+"*"+r1+"-"+x2+")^2+("+y1+"+"+w1+"*"+r1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 20: output="("+r2+")^2=(("+x1+"+"+a1+"*"+r1+"-"+x2+")^2+("+y1+"+"+b1+"*"+r1+"-"+y2+")^2)";
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                          case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 11: output=x2+"+"+v2+"*"+r2+"="+x1+"+"+v1+"*"+r1+","+y2+"+"+w2+"*"+r2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 21: output=x2+"+"+v2+"*"+r2+"="+x1+"+"+a1+"*"+r1+","+y2+"+"+w2+"*"+r2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 31: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(v2)) vars.append(v2);
                                   if(!vars.hasElem(w2)) vars.append(w2);
                                   if(!vars.hasElem(y2)) vars.append(r2);
                                   break;
                          case 12: output=x1+"+"+v1+"*"+r1+"="+x2+"+"+a2+"*"+r2+","+y1+"+"+w1+"*"+r1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 22: output=x1+"+"+a1+"*"+r1+"="+x2+"+"+a2+"*"+r2+","+y1+"+"+b1+"*"+r1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 32: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(a2)) vars.append(a2);
                                   if(!vars.hasElem(b2)) vars.append(b2);
                                   if(!vars.hasElem(r2)) vars.append(r2);
                                   break;
                          case 13: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(v1)) vars.append(v1);
                                   if(!vars.hasElem(w1)) vars.append(w1);
                                   if(!vars.hasElem(y1)) vars.append(r1);
                                   break;
                          case 23: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(a1)) vars.append(a1);
                                   if(!vars.hasElem(b1)) vars.append(b1);
                                   if(!vars.hasElem(r1)) vars.append(r1);
                                   break;
                          case 33: output=x1+"="+x2+","+y1+"="+y2;
                                   if(!vars.hasElem(x1)) vars.append(x1);
                                   if(!vars.hasElem(y1)) vars.append(y1);
                                   if(!vars.hasElem(x2)) vars.append(x2);
                                   if(!vars.hasElem(y2)) vars.append(y2);
                                   break;
                   }
                   break;
    }
    return output;
}

//returns an equation corresponding to a angle constraint
string getAngleEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    string output="", tanAngleStr, m1, m2, mA, mB, v1, v2, w1, w2, a1, a2, b1, b2;
    string n1, n2;
    float tanAngle;


    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    a1="a"+n1;
    b1="b"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;
    a2="a"+n2;
    b2="b"+n2;

    tanAngle=tan(theEdge.returnValue()*PI/180.0);
    tanAngle=tanAngle*tanAngle*10000;
    tanAngle=floor(tanAngle);
    tanAngle=tanAngle/10000;
    tanAngleStr=toString(tanAngle);

    switch(type1*10+type2)
    {
          case 11:
          case 13:
          case 31:
          case 33:  mA=m1;
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(m1)) vars.append(m1);
                    break;
          case 21:
          case 23:  mA="("+w1+"/"+v1+")";
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
          case 22:  mA="("+w1+"/"+v1+")";
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
          case 12:
          case 32:  mA=m1;
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
          case 51:
          case 53:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
                    mB=m2;
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(m2)) vars.append(m2);
                    break;
          case 35:
          case 15:  mA=m1;
                    mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(m1)) vars.append(m1);
                    break;
          case 52:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    break;
          case 25:  mA="("+w1+"/"+v1+")";
                    mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    break;
          case 55:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
                    mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    break;
   }
    if(mA!="" || mB!="") output="("+tanAngleStr+")=(("+mA+"-"+mB+")/(1+"+mA+"*"+mB+"))^2";
    else output="";
    return output;
}

//returns an equation for a parallel constraint
string getParallelEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    string output="",mB,mA,m1,v1,w1,m2,v2,w2,n1,n2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;

    switch(type1*10+type2)
    {
          case 11:
          case 13:
          case 31:
          case 33:  mA=m1;
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(m1)) vars.append(m1);
                    break;
          case 21:
          case 23:  mA="("+w1+"/"+v1+")";
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
          case 22:  mA="("+w1+"/"+v1+")";
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
          case 12:
          case 32:  mA=m1;
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
    }
    output="";
    if(mA!="" || mB!="") output=mA+"="+mB;
    return output;
}

//returns an equation corresponding to a perpendicularity constraint
string getPerpendicularEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    string output="",mB,mA,m1,v1,w1,m2,v2,w2,n1,n2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;

    switch(type1*10+type2)
    {
          case 11:
          case 13:
          case 31:
          case 33:  mA=m1;
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(m1)) vars.append(m1);
                    break;
          case 21:
          case 23:  mA="("+w1+"/"+v1+")";
                    mB=m2;
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
          case 22:  mA="("+w1+"/"+v1+")";
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
          case 12:
          case 32:  mA=m1;
                    mB="("+w2+"/"+v2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
    }
    output="";
    if(mA!="" || mB!="") output="(-1)="+mA+"*"+mB;
    return output;}

//returns -1 if the edge is not in the cluster
//returns  0 if the edge is in the cluster, but doesn't contain a subcluster
//returns  1 if the edge is between two clusters or between a cluster and original V
int getEdgeCode(Edge &theEdge, Cluster &theCluster)
{
    int i, length, childLength, origLength;
    bool isEnd;
    List<int> childrenWithEnd1, childrenWithEnd2;

    childrenWithEnd1.makeEmpty();
    childrenWithEnd2.makeEmpty();

    childLength=(theCluster.children).returnLen();
    origLength=theCluster.returnOrigLen();

    isEnd=(childLength==origLength);

    length=(theCluster.children).returnLen();

    if(length==0) return -1;

    if(!inOriginalV(theEdge.returnEnd1(), theCluster)||!inOriginalV(theEdge.returnEnd2(),theCluster))
      return -1;

    if(isEnd) return 0;

    for(i=1;i<=length;i++)
    {
       if(inOriginalV(theEdge.returnEnd1(), (theCluster.children).retrieve(i)))
         childrenWithEnd1.append(i);
       if(inOriginalV(theEdge.returnEnd2(), (theCluster.children).retrieve(i)))
         childrenWithEnd2.append(i);
    }
    for(i=1;i<=childrenWithEnd1.returnLen();i++)
       if(childrenWithEnd2.hasElem(childrenWithEnd1.retrieve(i))) return -1;

    if(childrenWithEnd1.returnLen()==0 && childrenWithEnd2.returnLen()==0) return 0;
    else return 1;

}

/* switchString takes an string as input then replaces all of the variables from a given cluster
   with an expression for that variable times the 2D rotation matrix.
   The boolean allows the method to also replace the variables with strings representing there real
   solved positions. */
string switchString(Vertex &theVertex, int type, int shapeName, int clusterName, string toReplace, bool solveOrValue)
{
    string repStr1, repStr2, repStr3, repStr4, repStr5, repStr6;
    string repStrb1, repStrb2, repStrb3, repStrb4, repStrb5, repStrb6;
    string newStr1, newStr2, newStr3, newStr4, newStr5, newStr6;
    string clusterV1, clusterV2, clusterV3, clusterV4;
    string clusterV5, clusterV6, clusterV7, clusterV8, clusterV9;
    string output;

    if(solveOrValue)
    {
      clusterV1="p"+toString(clusterName);
      clusterV2="q"+toString(clusterName);
      clusterV3="t"+toString(clusterName);
      clusterV4="s"+toString(clusterName);

      if(graphDimen==3)
      {	
        clusterV5="f"+toString(clusterName);
        clusterV6="h"+toString(clusterName);
        clusterV7="j"+toString(clusterName);
        clusterV8="k"+toString(clusterName);
        clusterV9="n"+toString(clusterName);
	if(!vars.hasElem(clusterV5)) vars.append(clusterV5);
        if(!vars.hasElem(clusterV6)) vars.append(clusterV6);
        if(!vars.hasElem(clusterV7)) vars.append(clusterV7);
        if(!vars.hasElem(clusterV8)) vars.append(clusterV8);
        if(!vars.hasElem(clusterV9)) vars.append(clusterV9);
      }

      if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
      if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
      if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
      if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
    }

    output=toReplace;


    switch(type)
    {
          case 4:  
          case 0:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                     newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
		     if(type==4)
	             {
			newStr3="("+toString(theVertex.returnDegValueByName(3))+")";
			repStr3="r"+toString(shapeName);
			vars.deleteElem(repStr3);
		     }
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   break;
          case 1:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="m"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                     newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                     newStr3="(("+repStrb3+"*"+clusterV3+"+"+clusterV4+")/("+repStrb3+"*"+clusterV4+"+"+clusterV3+"))";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
                   break;
          case 2:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="v"+toString(shapeName);
                   repStr4="w"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb4=repStr4;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     repStrb4.insert(1,"?");
                     newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                     newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                     newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+")";
                     newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+")";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                     newStr4="("+toString(theVertex.returnDegValueByName(5))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                     vars.deleteElem(repStr4);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
                   output=replaceAll(output, repStr4, newStr4);
                   break;
          case 3:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="c"+toString(shapeName);
                   repStr4="d"+toString(shapeName);
                   repStr5="m"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb4=repStr4;
                     repStrb5=repStr5;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     repStrb4.insert(1,"?");
                     repStrb5.insert(1,"?");
                     newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                     newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                     newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+"+"+clusterV1+")";
                     newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+"+"+clusterV2+")";
                     newStr5="(("+repStrb5+"*"+clusterV3+"+"+clusterV4+")/("+repStrb5+"*"+clusterV4+"+"+clusterV3+"))";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(2))+")";
                     newStr4="("+toString(theVertex.returnDegValueByName(3))+")";
                     newStr5="("+toString(theVertex.returnDegValueByName(4))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                     vars.deleteElem(repStr4);
                     vars.deleteElem(repStr5);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
                   output=replaceAll(output, repStr4, newStr4);
                   output=replaceAll(output, repStr5, newStr5);
                   break;
          case 5:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="v"+toString(shapeName);
                   repStr4="w"+toString(shapeName);
                   repStr5="a"+toString(shapeName);
                   repStr6="b"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb4=repStr4;
                     repStrb5=repStr5;
                     repStrb6=repStr6;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     repStrb4.insert(1,"?");
                     repStrb5.insert(1,"?");
                     repStrb6.insert(1,"?");
                     newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                     newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                     newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+")";
                     newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+")";
                     newStr5="("+repStrb5+"*"+clusterV3+"-"+repStrb6+"*"+clusterV4+")";
                     newStr6="("+repStrb6+"*"+clusterV3+"+"+repStrb5+"*"+clusterV4+")";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                     newStr4="("+toString(theVertex.returnDegValueByName(5))+")";
                     newStr5="("+toString(theVertex.returnDegValueByName(6))+")";
                     newStr6="("+toString(theVertex.returnDegValueByName(7))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                     vars.deleteElem(repStr4);
                     vars.deleteElem(repStr5);
                     vars.deleteElem(repStr6);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
                   output=replaceAll(output, repStr4, newStr4);
                   output=replaceAll(output, repStr5, newStr5);
                   output=replaceAll(output, repStr6, newStr6);
                   break;
          case 6:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="z"+toString(shapeName);
                   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     newStr1="(("+repStrb1+"*"+clusterV6+"*"+clusterV8+")+";
                     newStr1+="("+repStrb2+"*"+clusterV6+"*"+clusterV9+")-";
                     newStr1+="("+repStrb3+"*"+clusterV7+")+"+clusterV1+")";
                     newStr2="(("+repStrb1+"*("+clusterV4+"*"+clusterV7+"*"+clusterV8+"-"+clusterV3+"*"+clusterV9+"))+";
                     newStr2+="("+repStrb2+"*("+clusterV3+"*"+clusterV8+"+"+clusterV4+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr2+="("+repStrb3+"*"+clusterV4+"*"+clusterV6+")+"+clusterV2+")";
                     newStr3="(("+repStrb1+"*("+clusterV4+"*"+clusterV9+"+"+clusterV3+"*"+clusterV7+"*"+clusterV8+"))+";
                     newStr3+="("+repStrb2+"*(-"+clusterV4+"*"+clusterV8+"+"+clusterV3+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr3+="("+repStrb3+"*"+clusterV3+"*"+clusterV6+")+"+clusterV5+")"; 
		   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(2))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
		   break;

          case 7:  repStr1="x"+toString(shapeName);
                   repStr2="y"+toString(shapeName);
                   repStr3="z"+toString(shapeName);
                   repStr4="c"+toString(shapeName);
                   repStr5="d"+toString(shapeName);
                   repStr6="e"+toString(shapeName);
		   if(solveOrValue)
                   {
                     repStrb1=repStr1;
                     repStrb2=repStr2;
                     repStrb3=repStr3;
                     repStrb4=repStr4;
                     repStrb5=repStr5;
                     repStrb6=repStr6;
                     repStrb1.insert(1,"?");
                     repStrb2.insert(1,"?");
                     repStrb3.insert(1,"?");
                     repStrb4.insert(1,"?");
                     repStrb5.insert(1,"?");
                     repStrb6.insert(1,"?");
                     newStr1="(("+repStrb1+"*"+clusterV6+"*"+clusterV8+")+";
                     newStr1+="("+repStrb2+"*"+clusterV6+"*"+clusterV9+")-";
                     newStr1+="("+repStrb3+"*"+clusterV7+")+"+clusterV1+")";
                     newStr2="(("+repStrb1+"*("+clusterV4+"*"+clusterV7+"*"+clusterV8+"-"+clusterV3+"*"+clusterV9+"))+";
                     newStr2+="("+repStrb2+"*("+clusterV3+"*"+clusterV8+"+"+clusterV4+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr2+="("+repStrb3+"*"+clusterV4+"*"+clusterV6+")+"+clusterV2+")";
                     newStr3="(("+repStrb1+"*("+clusterV4+"*"+clusterV9+"+"+clusterV3+"*"+clusterV7+"*"+clusterV8+"))+";
                     newStr3+="("+repStrb2+"*(-"+clusterV4+"*"+clusterV8+"+"+clusterV3+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr3+="("+repStrb3+"*"+clusterV3+"*"+clusterV6+")+"+clusterV5+")"; 
                     newStr4="(("+repStrb4+"*"+clusterV6+"*"+clusterV8+")+";
                     newStr4+="("+repStrb5+"*"+clusterV6+"*"+clusterV9+")-";
                     newStr4+="("+repStrb6+"*"+clusterV7+")+"+clusterV1+")";
                     newStr5="(("+repStrb4+"*("+clusterV4+"*"+clusterV7+"*"+clusterV8+"-"+clusterV3+"*"+clusterV9+"))+";
                     newStr5+="("+repStrb5+"*("+clusterV3+"*"+clusterV8+"+"+clusterV4+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr5+="("+repStrb6+"*"+clusterV4+"*"+clusterV6+")+"+clusterV2+")";
                     newStr6="(("+repStrb4+"*("+clusterV4+"*"+clusterV9+"+"+clusterV3+"*"+clusterV7+"*"+clusterV8+"))+";
                     newStr6+="("+repStrb5+"*(-"+clusterV4+"*"+clusterV8+"+"+clusterV3+"*"+clusterV7+"*"+clusterV9+"))+";
                     newStr6+="("+repStrb6+"*"+clusterV3+"*"+clusterV6+")+"+clusterV5+")";
                   }
                   else
                   {
                     newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                     newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                     newStr3="("+toString(theVertex.returnDegValueByName(2))+")";
                     newStr4="("+toString(theVertex.returnDegValueByName(3))+")";
                     newStr5="("+toString(theVertex.returnDegValueByName(4))+")";
                     newStr6="("+toString(theVertex.returnDegValueByName(5))+")";
                     vars.deleteElem(repStr1);
                     vars.deleteElem(repStr2);
                     vars.deleteElem(repStr3);
                     vars.deleteElem(repStr4);
                     vars.deleteElem(repStr5);
                     vars.deleteElem(repStr6);
                   }
                   output=replaceAll(output, repStr1, newStr1);
                   output=replaceAll(output, repStr2, newStr2);
                   output=replaceAll(output, repStr3, newStr3);
                   output=replaceAll(output, repStr4, newStr4);
                   output=replaceAll(output, repStr5, newStr5);
                   output=replaceAll(output, repStr6, newStr6);
		   break;

    }
    output=replaceAll(output, "?", "");

    return output;
}

/* Generates an equation for a constraint between two vertices in different clusters.
   It gets an equation as if the vertices were in one cluster, then replaces
   all of the variables for each cluste; either with the 2D arbitrary rotation
   or with the previously solved values of that vertex if its parent cluster
   will not be rotated. */
string getEquationBetweenClusters(Graph& graph0, Edge &theEdge, Cluster &theCluster)
{
    string outString;
    Vertex vEnd1, vEnd2;
    int v1Name, v2Name;
    int end1C, end2C;

    outString="";

    if(isImaginary(graph0, theEdge)) theImags.append(theEdge);

    vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
    vEnd2=graph0.returnVertByName(theEdge.returnEnd2());

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    switch(theEdge.returnType()) {

          case 0: outString=getDistanceEQ(vEnd1,vEnd2,theEdge, "");
                  break;
          case 1: outString=getIncidenceEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 4: outString=getAngleEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 3: outString=getParallelEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 2: outString=getPerpendicularEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 5: outString=getTangencyEQ(vEnd1,vEnd2,theEdge);
                  break;
	  case 6: outString=getDistance3DEQ(vEnd1, vEnd2, theEdge, "");
		  break;
          case 8: outString=getAngle3DEQ(vEnd1,vEnd2,theEdge);
	          break;    
    }
    end1C=getChildNameWithVertex(theCluster, v1Name);
    end2C=getChildNameWithVertex(theCluster, v2Name);
    setValueInCluster(graph0,theCluster.children.retrieve(getChildIndexByName(theCluster, end1C)), vEnd1);
    setValueInCluster(graph0,theCluster.children.retrieve(getChildIndexByName(theCluster, end2C)), vEnd2);
    if (end1C==-1 || end2C==-1) return "";
    if (end1C!=v1Name) 
    {
       outString=switchString(vEnd1, vEnd1.returnType(), v1Name, end1C, outString, end1C!=withHeldCluster);
       if(end1C!=withHeldCluster)
          outString=switchString(vEnd1, vEnd1.returnType(), v1Name, end1C, outString, false);
    }
    if (end2C!=v2Name)
    {
       outString=switchString(vEnd2, vEnd2.returnType(), v2Name, end2C, outString, end2C!=withHeldCluster);
       if(end2C!=withHeldCluster)
          outString=switchString(vEnd2, vEnd2.returnType(), v2Name, end2C, outString, false);
    }
    return(outString);
}

//gets a equation for a edge where both endpoints fall in the same cluster, it simply calls the
//corresponding getEquation method above for each particular edge type.
string getEquationInCluster(Graph& graph0, Edge &theEdge, Cluster &theCluster)
{
    string outString;
    Vertex vEnd1, vEnd2;

    outString="";

    if(isImaginary(graph0, theEdge)) theImags.append(theEdge);

    if(theEdge.returnName()==999)
    {
      vEnd1=testVertex1;
      vEnd2=testVertex2;
    }
    else
    {
      vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
      vEnd2=graph0.returnVertByName(theEdge.returnEnd2());
    }
    switch(theEdge.returnType()) {

          case 0: outString=getDistanceEQ(vEnd1,vEnd2,theEdge, "");
                  break;
          case 1: outString=getIncidenceEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 4: outString=getAngleEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 3: outString=getParallelEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 2: outString=getPerpendicularEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 5: outString=getTangencyEQ(vEnd1,vEnd2,theEdge);
                  break;
          case 6: outString=getDistance3DEQ(vEnd1,vEnd2,theEdge,"");
		  break;
          case 8: outString=getAngle3DEQ(vEnd1,vEnd2,theEdge);
	          break;	  
    }

    return(outString);
}

//for every edge in graph0 in theCluster, gets a corresponding equation string
string getEquation(Graph &graph0, Edge &theEdge, Cluster &theCluster)
{
    int temp;
    string output;

    temp = getEdgeCode(theEdge, theCluster);

    if(temp==1) output+=getEquationBetweenClusters(graph0, theEdge, theCluster);
    if(temp==0) output+=getEquationInCluster(graph0, theEdge, theCluster);
    return output;
}

string getEquation(Graph &graph0, Cluster &theCluster, ostream &inputFile)
{
  //the_Tree Tree;

  ClustData test;
  test=summCluster(graph0, theCluster);
  int length=test.length,i;
  cout << "Length : " << length << endl;
  
  string tempString, newString, outputString;
  string stringArray[500];
  Vertex vEnd1, vEnd2;
  int end1C, end2C;    

  cout<<"search_flag: "<<search_flag<<endl;
	
  if (search_flag==1)
    outputString+=Tree.search_Tree(graph0, theCluster, inputFile);
  if (search_flag==2)
    outputString+=Tree.search_Tree(graph0, theCluster, inputFile, search_flag);

  cout<<"Vars right after search"<<endl;
  cout<<getVarString()<<endl;


  //additional constraints are generated, see the appropriate methods for more details
	
  int tag;
  Edge forOriginSet, forOriginSet2;

  tag=1;
  
  equationCount=test.length;

  forOriginSet=graph0.returnEdgeByName(test.edgeID[0]);

  while(tag<test.length && test.edgeType[tag++]==1)
  {
	cout<<forOriginSet<<endl;
	forOriginSet=graph0.returnEdgeByName(test.edgeID[tag]);
  }	

  if(test.length>1)
    forOriginSet2=graph0.returnEdgeByName(test.edgeID[1]);

  if(tag>=test.length && test.length>1)
        if(forOriginSet.returnName()==test.edgeID[0])
		forOriginSet2=graph0.returnEdgeByName(test.edgeID[1]);
	else    forOriginSet2=graph0.returnEdgeByName(test.edgeID[0]);

  while(tag<test.length && test.edgeType[tag++]==1)
        forOriginSet2=graph0.returnEdgeByName(test.edgeID[tag]);

  if(graph0.returnNumVer()==3 && graph0.returnNumEdg()==3)
	forOriginSet=graph0.returnEdgeByName(test.edgeID[1]);

  if(graph0.returnNumVer()==4 && graph0.returnNumEdg()==8)
        forOriginSet=graph0.returnEdgeByName(test.edgeID[1]);

  tag=0;

  string over;

  over=getOverlapConstraint(graph0, theCluster);

  if(test.length==0)
	over.erase(0,1);
  
  outputString+=over;
  outputString+=getRotationEquations(theCluster);
  outputString+=getLineConstraint(graph0,theCluster.children);
  outputString+=getSinCosConstraint();

  cout<<"Var Length = "<<vars.returnLen()<<", Eq Count = "<<equationCount<<endl;

  string originTemp;

  if(vars.returnLen()>equationCount)
  {
    originTemp=getOriginEquations(forOriginSet, forOriginSet2, graph0, 0, theCluster.returnType());
    if(originTemp.size()>0) outputString+=","+originTemp;
  }

  for(int i=1;i<=theCluster.children.returnLen();i++)
  {
	if(theCluster.children.retrieve(i).returnType()!=2) continue;

	int cName=theCluster.children.retrieve(i).returnName();
	int repName=theCluster.children.retrieve(i).currAlias;
	string r1,r2,r3,r4,r5,r6,s1,s2,s3,s4,s5,s6;

	r1="x"+toString(cName);
	r2="y"+toString(cName);
	r3="z"+toString(cName);
	r4="c"+toString(cName);
        r5="d"+toString(cName);
        r6="e"+toString(cName);

	for(int q=0;q<=theCluster.children.retrieve(i).lines.returnLen();q++)
	{
		if(q==0)
			repName=theCluster.children.retrieve(i).currAlias*1000+cName;
		else
			repName=theCluster.children.retrieve(i).lines.retrieve(q)*1000+cName;

		s1="x"+toString(repName);
	        s2="y"+toString(repName);
       		s3="z"+toString(repName);
		s4="c"+toString(repName);
	        s5="d"+toString(repName);
        	s6="e"+toString(repName);
        
	        outputString=replaceAll(outputString,s1,r1);
        	outputString=replaceAll(outputString,s2,r2);
        	outputString=replaceAll(outputString,s3,r3);
        	outputString=replaceAll(outputString,s4,r4);
        	outputString=replaceAll(outputString,s5,r5);
        	outputString=replaceAll(outputString,s6,r6);

		if(!vars.hasElem(r1)) vars.append(r1);
		if(!vars.hasElem(r2)) vars.append(r2);       
		if(!vars.hasElem(r3)) vars.append(r3);       
		if(!vars.hasElem(r4)) vars.append(r4);       
		if(!vars.hasElem(r5)) vars.append(r5);       
		if(!vars.hasElem(r6)) vars.append(r6);

		if(vars.hasElem(s1)) vars.deleteElem(s1);
                if(vars.hasElem(s2)) vars.deleteElem(s2);
                if(vars.hasElem(s3)) vars.deleteElem(s3);
                if(vars.hasElem(s4)) vars.deleteElem(s4);
                if(vars.hasElem(s5)) vars.deleteElem(s5);
                if(vars.hasElem(s6)) vars.deleteElem(s6);
	}
			

	for(int q=1;q<=theCluster.children.retrieve(i).left.returnLen();q++)
	{
		repName=theCluster.children.retrieve(i).left.retrieve(q)*1000+cName;
		
		s1="x"+toString(repName);
	        s2="y"+toString(repName);
        	s3="z"+toString(repName);

		outputString=replaceAll(outputString,s1,r1);
                outputString=replaceAll(outputString,s2,r2);
                outputString=replaceAll(outputString,s3,r3);

                if(!vars.hasElem(r1)) vars.append(r1);
                if(!vars.hasElem(r2)) vars.append(r2);
                if(!vars.hasElem(r3)) vars.append(r3);
        
                if(vars.hasElem(s1)) vars.deleteElem(s1);
                if(vars.hasElem(s2)) vars.deleteElem(s2);
                if(vars.hasElem(s3)) vars.deleteElem(s3);
 
	}

        for(int q=1;q<=theCluster.children.retrieve(i).right.returnLen();q++)
        {
                repName=theCluster.children.retrieve(i).right.retrieve(q)*1000+cName;
        
                s1="x"+toString(repName);
                s2="y"+toString(repName);
                s3="z"+toString(repName);
   
                outputString=replaceAll(outputString,s1,r4);
                outputString=replaceAll(outputString,s2,r5);
                outputString=replaceAll(outputString,s3,r6);

                if(!vars.hasElem(r1)) vars.append(r4);
                if(!vars.hasElem(r2)) vars.append(r5);
                if(!vars.hasElem(r3)) vars.append(r6);
         
                if(vars.hasElem(s1)) vars.deleteElem(s1);
                if(vars.hasElem(s2)) vars.deleteElem(s2);
                if(vars.hasElem(s3)) vars.deleteElem(s3);
 
        }

	float theValue=findDist(graph0,theCluster.children.retrieve(i));
	theValue=rint(theValue);	

	outputString+=",("+toString(theValue)+")^2";
	outputString+="=("+r1+"-"+r4+")^2+("+r2+"-"+r5+")^2+("+r3+"-"+r6+")^2";
   }
		

  if(vars.returnLen()>equationCount && originTemp=="")
  	outputString+=","+getSpecialCaseEquations(graph0, theCluster, 1);

  if(vars.returnLen()==9 && equationCount==7 && originTemp=="")
  {
	int i=1, count=0;

	while(count<2)
	{
		switch(vars.retrieve(i)[0])
		{
			case	'p':
			case	'q':
			case	'f':	break;

			case	's':
			case	'j':
			case	'n':	outputString+=vars.retrieve(i)+"=0";
					count++;
					if(count==1) outputString+=",";
					break;
			case	't':
			case 	'h':
			case 	'k':	outputString+=vars.retrieve(i)+"=1"; 
                                        count++;
					if(count==1) outputString+=",";
                                        break;
		}
		i++;
	}

  }			

  outputString+="},";
  outputString+=getVarString();
  outputString+="));\n";
  outputString+="allvalues(%);\n";
  outputString+="evalf(%);\n";
  cout<<"OUTPUT FILE WRITTEN"<<endl;

  int loc=60;
  
  while(loc<outputString.size())
  {
	loc=outputString.find_first_of("*+-/",loc);
	if(loc<0 || loc>outputString.size())
		break;
	outputString.insert(loc,"\n");
	loc+=60;
  }

  cout<<outputString<<endl;
	
  return outputString;
}


//returns in theList, the names of all the children in theCluster that contain
//vName as an original Vertex
void getContainedChildList(Cluster &theCluster, int vName, List<int> &theList)
{
    List<int> output;
    int i, length;

    length=theCluster.children.returnLen();
    for(i=1; i<=length; i++)
       if(inOriginalV(vName, theCluster.children.retrieve(i)))
         theList.append(theCluster.children.retrieve(i).returnName());
}

//Ray objects use the sine and cosine of the angle of the ray compared to the positive x
//axis as degrees of freedom in there equation sets.  This method generates additional equations
//that insure that the squares of these sine and cosine values alway sum to 1
string getSinCosConstraint()
{
    int i, length;
    string temp, temp2, clust;
    string output="";

    length=vars.returnLen();

    for(i=1;i<=length;i++)
    {
       temp=vars.retrieve(i);
       temp2=temp;
       clust=temp2.erase(0,1);
       if(temp[0]=='w') output+=",1=(w"+clust+"^2+v"+clust+"^2)";
       if(temp[0]=='a') output+=",1=(a"+clust+"^2+b"+clust+"^2)";
    }
    return output;
}

//Within the DRTree used by this solver, original vertices in more than one cluster have 
//duplicate datastructures in each of the clusters that contain them.  At the time of solution,
//the implied constaint that each of these duplicated objects, must actually be the same, must be
//resolved.  To assure this, additional equations are added, stating that for each of these 
//overlaps, some rotation and translation of one cluster must bring all these duplicate points
//into the correct positions.
string getOverlapConstraint(Graph &graph0, Cluster &theCluster)
{
    int i, j, length, type, currOrig;
    int clustint1, clustint2;
    string cluster1, cluster2, origStr, nameStr;
    string newStr1a, newStr2a, newStr3a, newStr1b, newStr2b, newStr3b;
    string clusterV1, clusterV2, clusterV3, clusterV4;
    string clusterV5, clusterV6, clusterV7, clusterV8, clusterV9;
    string repStrb1, repStrb2, repStrb3;
    List<int> theOrig, in, theClust;
    Cluster tempCluster1, tempCluster2;
    Vertex tempVertex1, tempVertex2;
    string output="";
    string temp="", valueName="";

    if(theCluster.returnOrig().returnLen()==theCluster.children.returnLen()) return "";

    theOrig=theCluster.returnOrig();

    cout<<"Vars before getOverlap"<<endl;
    cout<<getVarString()<<endl;

    length=theOrig.returnLen();

    for(i=1; i<=length; i++)
    {
       currOrig=theOrig.retrieve(i);
       getContainedChildList(theCluster,currOrig,in);
       for(j=1; j<in.returnLen();j++)
       {
          type=graph0.returnVertByName(currOrig).returnType();
          clustint1=in.retrieve(j);
          clustint2=in.retrieve(j+1);
          if(clustint1!=withHeldCluster) if(!theClust.hasElem(clustint1)) theClust.append(clustint1);
          if(clustint2!=withHeldCluster) if(!theClust.hasElem(clustint2)) theClust.append(clustint2);
          tempCluster1=theCluster.children.retrieve(getChildIndexByName(theCluster, clustint1));
          tempCluster2=theCluster.children.retrieve(getChildIndexByName(theCluster, clustint2));
          tempVertex1.setName(currOrig);
          tempVertex2.setName(currOrig);
          setValueInCluster(graph0, tempCluster1, tempVertex1);
          setValueInCluster(graph0, tempCluster2, tempVertex2);
          if(aOverlap==0 && clustint1!=withHeldCluster) aOverlap=clustint1;
          else if(aOverlap==0) aOverlap=clustint2;
          cluster1=toString(clustint1);
          cluster2=toString(clustint2);
          origStr=toString(currOrig);
          clusterV1="p"+cluster1;
          clusterV2="q"+cluster1;
          clusterV3="t"+cluster1;
          clusterV4="s"+cluster1;
          clusterV5="f"+cluster1;
          clusterV6="h"+cluster1;
          clusterV7="j"+cluster1;
          clusterV8="k"+cluster1;
          clusterV9="n"+cluster1;

	  cout<<"THIS IS IN OVERLAP"<<endl;
	  cout<<tempVertex1<<endl;
	  cout<<tempVertex2<<endl;

	  bool end1Over=(tempCluster1.returnType()==2);
	  bool end2Over=(tempCluster2.returnType()==2);

	  if(end1Over)
	  {
	    valueName=toString(tempVertex1.returnName()*1000+tempCluster1.returnName());
	    newStr1a="x"+valueName;
	    newStr2a="y"+valueName;
	    newStr3a="z"+valueName;           
	  }

          if(clustint1==withHeldCluster && !end1Over)
          {
            newStr1a="("+toString(tempVertex1.returnDegValueByName(0))+")";
            newStr2a="("+toString(tempVertex1.returnDegValueByName(1))+")";
            newStr3a="("+toString(tempVertex1.returnDegValueByName(2))+")";
          }

	  if(clustint1!=withHeldCluster && !end1Over)
          {
            if(type<6)
            {
              if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
              if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
              if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
              if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
              newStr1a="(("+toString(tempVertex1.returnDegValueByName(0))+")*"+clusterV3+"-("+toString(tempVertex1.returnDegValueByName(1))+")*"+clusterV4+"+"+clusterV1+")";
              newStr2a="(("+toString(tempVertex1.returnDegValueByName(1))+")*"+clusterV3+"+("+toString(tempVertex1.returnDegValueByName(0))+")*"+clusterV4+"+"+clusterV2+")";
              newStr3a="((("+toString(tempVertex1.returnDegValueByName(2))+")*"+clusterV3+"+"+clusterV4+")/(("+toString(tempVertex1.returnDegValueByName(2))+")*"+clusterV4+"+"+clusterV3+"))";
            }
            else
            {
              if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
              if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
              if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
              if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
              if(!vars.hasElem(clusterV5)) vars.append(clusterV5);
              if(!vars.hasElem(clusterV6)) vars.append(clusterV6);
              if(!vars.hasElem(clusterV7)) vars.append(clusterV7);
              if(!vars.hasElem(clusterV8)) vars.append(clusterV8);
              if(!vars.hasElem(clusterV9)) vars.append(clusterV9);
              repStrb1=toString(tempVertex1.returnDegValueByName(0));
              repStrb2=toString(tempVertex1.returnDegValueByName(1));
              repStrb3=toString(tempVertex1.returnDegValueByName(2));
              newStr1a="(("+repStrb1+"*"+clusterV6+"*"+clusterV8+")-";
              newStr1a+="("+repStrb2+"*"+clusterV6+"*"+clusterV9+")+";
              newStr1a+="("+repStrb3+"*"+clusterV7+")"+clusterV1+")";
              newStr2a="(("+repStrb1+"*("+clusterV4+"*"+clusterV7+"*"+clusterV8+"+"+clusterV3+"*"+clusterV9+"))+";
              newStr2a+="("+repStrb2+"*("+clusterV3+"*"+clusterV8+"-"+clusterV4+"*"+clusterV7+"*"+clusterV9+"))-";
              newStr2a+="("+repStrb3+"*"+clusterV4+"*"+clusterV6+")+"+clusterV2+")";
              newStr3a="(("+repStrb1+"*("+clusterV4+"*"+clusterV9+"-"+clusterV3+"*"+clusterV7+"*"+clusterV8+"))+";
              newStr3a+="("+repStrb2+"*("+clusterV4+"*"+clusterV8+"+"+clusterV3+"*"+clusterV7+"*"+clusterV9+"))+";
              newStr3a+="("+repStrb3+"*"+clusterV3+"*"+clusterV6+")+"+clusterV5+")";              
            }
          }
          clusterV1="p"+cluster2;
          clusterV2="q"+cluster2;
          clusterV3="t"+cluster2;
          clusterV4="s"+cluster2;
          clusterV5="f"+cluster2;
          clusterV6="h"+cluster2;
          clusterV7="j"+cluster2;
          clusterV8="k"+cluster2;
          clusterV9="n"+cluster2;

          if(end2Over)
          {
	    valueName=toString(tempVertex2.returnName()*1000+tempCluster2.returnName());
            newStr1b="x"+valueName;
            newStr2b="y"+valueName;
            newStr3b="z"+valueName;
          }	  

          if(clustint2==withHeldCluster && !end2Over)
          {
            newStr1b="("+toString(tempVertex2.returnDegValueByName(0))+")";
            newStr2b="("+toString(tempVertex2.returnDegValueByName(1))+")";
            newStr3b="("+toString(tempVertex2.returnDegValueByName(2))+")";
          }
	  if(clustint2!=withHeldCluster && !end2Over)
          {
            if(type<6)
            {
              if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
              if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
              if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
              if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
              newStr1b="(("+toString(tempVertex2.returnDegValueByName(0))+")*"+clusterV3+"-("+toString(tempVertex2.returnDegValueByName(1))+")*"+clusterV4+"+"+clusterV1+")";
              newStr2b="(("+toString(tempVertex2.returnDegValueByName(1))+")*"+clusterV3+"+("+toString(tempVertex2.returnDegValueByName(0))+")*"+clusterV4+"+"+clusterV2+")";
              newStr3b="((("+toString(tempVertex2.returnDegValueByName(2))+")*"+clusterV3+"+"+clusterV4+")/(("+toString(tempVertex2.returnDegValueByName(2))+")*"+clusterV4+"+"+clusterV3+"))";
            }   
            else
            {
              if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
              if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
              if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
              if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
              if(!vars.hasElem(clusterV5)) vars.append(clusterV5);
              if(!vars.hasElem(clusterV6)) vars.append(clusterV6);
              if(!vars.hasElem(clusterV7)) vars.append(clusterV7);
              if(!vars.hasElem(clusterV8)) vars.append(clusterV8);
              if(!vars.hasElem(clusterV9)) vars.append(clusterV9);
              repStrb1=toString(tempVertex2.returnDegValueByName(0));
              repStrb2=toString(tempVertex2.returnDegValueByName(1));
              repStrb3=toString(tempVertex2.returnDegValueByName(2));
              newStr1b="(("+repStrb1+"*"+clusterV6+"*"+clusterV8+")-";
              newStr1b+="("+repStrb2+"*"+clusterV6+"*"+clusterV9+")+";
              newStr1b+="("+repStrb3+"*"+clusterV7+")+"+clusterV1+")";
              newStr2b="(("+repStrb1+"*("+clusterV4+"*"+clusterV7+"*"+clusterV8+"+"+clusterV3+"*"+clusterV9+"))+";
              newStr2b+="("+repStrb2+"*("+clusterV3+"*"+clusterV8+"-"+clusterV4+"*"+clusterV7+"*"+clusterV9+"))-";
              newStr2b+="("+repStrb3+"*"+clusterV4+"*"+clusterV6+")+"+clusterV2+")";
              newStr3b="(("+repStrb1+"*("+clusterV4+"*"+clusterV9+"-"+clusterV3+"*"+clusterV7+"*"+clusterV8+"))+";
              newStr3b+="("+repStrb2+"*("+clusterV4+"*"+clusterV8+"+"+clusterV3+"*"+clusterV7+"*"+clusterV9+"))+";
              newStr3b+="("+repStrb3+"*"+clusterV3+"*"+clusterV6+")+"+clusterV5+")";
            }
          }
          switch(type)
          {
                case 4:
                case 0:  temp+=newStr1a+"="+newStr1b+",";
                         temp+=newStr2a+"="+newStr2b;
                         equationCount+=2;
                         break;
                case 1:  temp+=newStr1a+"="+newStr1b+",";
                         temp+=newStr2a+"="+newStr2b+",";
                         temp+=newStr3a+"="+newStr3b;
                         equationCount+=3;
                         break;
                case 6:  temp+=newStr1a+"="+newStr1b+",";
                         temp+=newStr2a+"="+newStr2b+",";
                         temp+=newStr3a+"="+newStr3b;
			 equationCount+=3;
			 break;
                default: temp="";
                         break;
          }
          output+=","+temp;
          temp="";
       }
       in.makeEmpty();
    }
    if(equationCount==0) output.erase(0,1);
/*    for(i=1;i<=theClust.returnLen();i++)
    {
       temp=toString(theClust.retrieve(i));
       output+=",s"+temp+"^2+t"+temp+"^2=1";
       equationCount++;
    } */
    return output;
}

//the line object, has only two degrees of freedom, its y axis intercept and its slope; however,
//the equations generated for the line object use three degrees of freedom a point and a slope. 
//in order to eliminate this additional added degree of freedom, this method generates simple equations
//setting the x coordinate of any line to zero
string getLineConstraint(Graph &graph0, List<Cluster> &theChildren)
{
    string outString;
    Cluster theCluster;
    Vertex theVert;
    int i, length;
    string name;
    string intName;
    string slopeVar;

    outString="";

    length=theChildren.returnLen();

    for(i=1;i<=length;i++)
    {
       theCluster=theChildren.retrieve(i);
       name="x"+toString(theCluster.returnName());
       if(graph0.hasVert(theCluster.returnName()))
       {
         theVert=graph0.returnVertByName(theCluster.returnName());
         if(theVert.returnType()>0 && theVert.returnType()<3 && vars.hasElem(name))
         {
	   outString+=",x"+toString(theCluster.returnName())+"=0";
	   equationCount++;       
	 }

         if(theVert.returnType()==3)
         {
	   cout<<getVarString()<<endl;
           intName=toString(theCluster.returnName());
	   slopeVar="m"+intName;
	   if(!vars.hasElem(slopeVar)) return outString;
           outString+=",(y"+intName+"-d"+intName+")=m"+intName+"*(x"+intName+"-c"+intName+")";
           if(!vars.hasElem(slopeVar)) vars.append(slopeVar);
         }
       }
    }
    return outString;
}

//Each solved cluster has three remaining degrees of freedom, two translational degrees and one 
//rotation, this program expresses these degrees of freedom in four variables, x translation,
//y translation, sine of rotation angle, and cosine of rotation angle.  To insure that when the
//equations are solved, the relationship between the sine and cosine is maintained, this method
//generated additional equations stating that the sum of the squares of the sine and cosine must
//be 1
string getRotationEquations(Cluster &theCluster)
{
    string outString, temp, temp2;
    int i, length, childName;

    outString="";

    if(theCluster.children.returnLen()==0) return outString;
    if(theCluster.children.returnLen()==theCluster.returnOrigLen()) return outString;

    length=theCluster.children.returnLen();

    for(i=1;i<=length;i++)
    {
       childName=theCluster.children.retrieve(i).returnName();
       if(theCluster.children.retrieve(i).children.returnLen()!=0 && childName!=withHeldCluster &&
	  theCluster.children.retrieve(i).returnType()!=2)
       {
         temp=toString(childName);
	 temp2="f"+temp;
         if(vars.hasElem(temp2))
         {   
	   outString+=",s"+temp+"^2+t"+temp+"^2=1,h"+temp+"^2+j"+temp+"^2=1,k"+temp+"^2+n"+temp+"^2=1";        
           equationCount+=3;
	 }
	 else
         { 
	   outString+=",s"+temp+"^2+t"+temp+"^2=1";
	   equationCount++;
         }
       }
    }
   
    return outString;       
} 

//after all of the equations for a cluster have been generated, if the cluster is rigid, exactly
//three degrees of freedom will still remain.  This method generates simple equations which fix those
//degrees of freedom, so Maple will generate exact answers
string getOriginEquations(Edge &theEdge, Edge &theEdge2, Graph &graph0, int clusterName, int clusterType)
{
    string outString, tempString, secondName;
    Vertex vEnd1, vEnd2, vEnd3, vEnd4;
    int v1Name, v2Name, v3Name, v4Name, thirdName;
    int type1, type2, type3, type4;

    cout<<"Start getOrigin"<<endl;

    outString="";

    if(theEdge2.returnName()==0) theEdge2=theEdge;

    if(clusterName>0)
    {
      tempString=toString(clusterName);
      outString="p"+tempString+"=0,q"+tempString+"=0,t"+tempString+"=0";
      return outString;
    }

    cout<<theEdge<<endl;
    cout<<theEdge2<<endl;
    graph0.output(cout);

    vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
    vEnd2=graph0.returnVertByName(theEdge.returnEnd2());
    vEnd3=graph0.returnVertByName(theEdge2.returnEnd1());
    vEnd4=graph0.returnVertByName(theEdge2.returnEnd2());

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();
    v3Name=vEnd3.returnName();
    v4Name=vEnd4.returnName();

    if(v1Name==v3Name || v2Name==v3Name)
	thirdName=v4Name;
    else thirdName=v3Name;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();
    type3=vEnd3.returnType();
    type4=vEnd4.returnType();

    if(type2==2 || type2==1)
      secondName="y";
    else secondName="x";

    cout<<"in getOrigin after getting all data"<<endl;

    switch(type1)
    {
	case 5:
	case 4:
	case 0: if(type2==2 || type2==1)
		  secondName="m";
                else secondName="x";
		outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
		break;
	case 3: outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,d"+toString(v1Name)+"=0.0001";
                break;
	case 1: outString="y"+toString(v1Name)+"=0,m"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
		break;
	case 2: outString="y"+toString(v1Name)+"=0,w"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
		break;		
        case 6: if(clusterType!=2)
		{
		  outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,z"+toString(v1Name)+"=0";
                  outString+=",x"+toString(v2Name)+"=0,z"+toString(v2Name)+"=0,z"+toString(thirdName)+"=0";
		}
		else
		{
		  outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,z"+toString(v1Name)+"=0";
                  outString+=",x"+toString(v2Name)+"=0,z"+toString(v2Name)+"=0";
		}
                break;
    }
    varsToZero=outString;
    varsToZero=replaceAll(varsToZero,"=0","");
    varsToZero=replaceAll(varsToZero,",","");	

    return(outString);
}

//within the child of theCluster named ID, sets the indexth solved value to setValue
void setValue(Cluster &theCluster, int ID, float theValue, int index)
{
    int i=1;

    while(theCluster.children.retrieve(i).returnName()!=ID) i++;

    theCluster.children.retrieve(i).setValue(index, theValue);

    cout<<"Set Value in Cluster "<<ID<<endl;
    cout<<"with value "<<theValue<<" at index "<<index<<endl;

}

//given the letter in a variable name returns the index of the freedom value
//it corresponds to (a given type of value is always stored in the same index, see chart in
//mfa2Clas.h
int getFreeDegIndex(char keyChar, Vertex &theVertex)
{
    int out;

    if(theVertex.returnType()!=7)
    	switch(keyChar) {
	
        	  case 'p':
	          case 'x': out=0;
                	    break;
        	  case 'q':
	          case 'y': out=1;
        	            break;
	          case 'c':
        	  case 'r': 
	          case 'f': 
        	  case 'z': out=2;
                	    break;
	          case 'd':
        	  case 'g':
		  case 't': out=3;
                       	    break;
	          case 'v':
	          case 'm': 
        	  case 's': 
	          case 'e': out=4;
        	            break;
	          case 'w':
        	  case 'l': 
		  case 'h': out=5;
        	            break;
	          case 'a': 
		  case 'j': out=6;
                	    break;
	          case 'b':
        	  case 'k': out=7;
                	    break;
	          case 'n': out=8;
			    break;
          }
    else
          switch(keyChar) {
                  
                  case 'p':
                  case 'c': out=0;
                            break;
                  case 'q':
                  case 'd': out=1;
                            break;
                  case 'r':
                  case 'f':
                  case 'e': out=2;
                            break;
                  case 'x':
                  case 'g':
                  case 't': out=3;
                            break;
                  case 'v':
                  case 'm':
                  case 's':
                  case 'y': out=4;
                            break;
		  case 'z':
                  case 'w':
                  case 'l':
                  case 'h': out=5;
                            break;
                  case 'a':
                  case 'j': out=6;
                            break;
                  case 'b':
                  case 'k': out=7;
                            break;
                  case 'n': out=8;
                            break;
          }
	
    return out;
}

//sets the degrees of freedom in theCluster to the values stored in the given
//bifurcation string
void parseBifurString(Graph &graph0, Cluster &theCluster)
{
    int i, length, inputLength;
    int pos=0, ID, childLen;
    float value;
    char varID, temp;
    string currParse, IDString, input;

    childLen=theCluster.children.returnLen();
/*    for(i=1;i<=childLen;i++)
       if(theCluster.children.retrieve(i).children.returnLen()!=0)
         theCluster.children.retrieve(i).setToCluster(theCluster); */

    input=theCluster.returnCurrBifurString();
    length=0;
    inputLength=input.size();
    for(i=0; i<inputLength; i++)
       if(input[i]=='=') length++;
    temp=input[pos];
    for(i=0; i<length; i++)
    {
       currParse="";
       while(temp!=',' && pos<inputLength)
       {
          currParse=currParse+temp;
          pos++;
          temp=input[pos];
       }
       IDString="";
       if(currParse[currParse.size()-1]=='.') currParse.erase(currParse.size()-1);
       varID=currParse[0];
       currParse.erase(0,1);
       while(currParse[0]!='=')
       {
          IDString+=currParse[0];
          currParse.erase(0,1);
       }
       currParse.erase(0,1);
       value=atof(currParse.c_str());
       if(fabs(value)<1e-5) value=0;
       ID=atoi(IDString.c_str());
       setValue(theCluster,ID,value,getFreeDegIndex(varID,graph0.returnVertByName(ID)));
       pos++;
       if(pos<inputLength) temp=input[pos];
    }
}

//updates the graph to contain the degree values stored in the children of
//theCluster, also performs all necessary rotations to bring the the vertex to
//the coordinate system of theCluster
void updateGraph(Graph &graph0, Cluster &theCluster)
{
    int i, j, length=theCluster.returnOrigLen();
    int vName, cName;
    float aValue;

    cout<<"Cluster Being Updated: "<<theCluster.returnName()<<endl;

    for(i=1;i<=length;i++)
    {
       vName=theCluster.returnOrigV(i);
       cName=getChildNameWithVertex(theCluster, vName);
       if(vName==cName)
         for(j=0; j<9; j++)
         {
            aValue=theCluster.children.retrieve(getChildIndexByName(theCluster,cName)).returnValue(j);
            setValue(graph0.returnVertByName(vName),j,aValue);
         }
       else setValueInCluster(graph0, theCluster, graph0.returnVertByName(vName));
    }
}

//NO LONGER USED SINCE THE IMPLEMENTATION OF THE JAVA NATIVE INTERFACE
//outputs the DRDAG to a file
void outputDRDAG(List<Cluster> &theCluster, ostream &output, bool first)
{
    int i, j, length, lengthOrig, lengthChild;

    length=theCluster.returnLen();
    
    if(first)
    {
      output<<-2<<endl;
      output<<length<<endl;
    }

    for(int i=1;i<=length;i++)
    {
       output<<theCluster.retrieve(i).returnName()<<endl;
       lengthOrig=theCluster.retrieve(i).returnOrigLen();
       output<<lengthOrig<<endl;
       for(j=1;j<=lengthOrig;j++)
          output<<theCluster.retrieve(i).returnOrigV(j)<<endl;
       lengthChild=theCluster.retrieve(i).children.returnLen();
       output<<lengthChild<<endl;
       for(j=1;j<=lengthChild;j++)
	  output<<theCluster.retrieve(i).children.retrieve(j).returnName()<<endl;
       outputDRDAG(theCluster.retrieve(i).children,output,false);
    }
    if(first) output<<-1<<endl;
}

/*  Ouputs the DRDag to an array, the following format is used:

	-2			-- signal to sketcher that the DRDag is coming
	<number of trees in the DAG>
	
	then recursively, each node before its children the following is output:

      <cluster ID>
      <number of original vertices>
      <list of original vertices>
      <number of children>
      <list of children>

      (Note that all values are always printed even if they are 0);
*/
void outputDRDAGToArray(List<Cluster> &theCluster, int &index, int*& theInts, bool first)
{
    int i, j, length, lengthOrig, lengthChild, overConst;

    length=theCluster.returnLen();

    if(first)
    {
       theInts[index++]=-2;
       theInts[index++]=length;
    }

    for(int i=1;i<=length;i++)
    {
       theInts[index++]=theCluster.retrieve(i).returnName();
       if(theCluster.retrieve(i).returnType()>0 || theCluster.retrieve(i).returnConst()>1)
	  	overConst=1;
       else	overConst=0;
       theInts[index++]=overConst;
       lengthOrig=theCluster.retrieve(i).returnOrigLen();
       theInts[index++]=lengthOrig;
       for(j=1;j<=lengthOrig;j++)
          theInts[index++]=theCluster.retrieve(i).returnOrigV(j);
       lengthChild=theCluster.retrieve(i).children.returnLen();
       theInts[index++]=lengthChild;
       for(j=1;j<=lengthChild;j++)
          theInts[index++]=theCluster.retrieve(i).children.retrieve(j).returnName();
       outputDRDAGToArray(theCluster.retrieve(i).children, index, theInts, false);
    }
    if(first) theInts[index]=-1;
}

//outputs the fin state of every cluster.  Fin flags are used to tell the solver which 
//clusters have and have not been solved.  This method visits each node in the tree, and
//at each visit outputs the clusters name, then fin start 0 for solved, 1 for not solved,
//before recursively continuing for each of its children
void outputFinState(Cluster &theCluster, int &startI, int*& theInts, int &count)
{
    int i, length;

    count++;
    theInts[startI++]=theCluster.returnName();
    if(theCluster.isSolved()) theInts[startI++]=0;
    else theInts[startI++]=1;

    length=theCluster.children.returnLen();
    for(i=1;i<=length;i++)
       outputFinState(theCluster.children.retrieve(i), startI, theInts, count);
}

//sets the children of theCluster to the next distinct set of bifurcations
bool getNextBifurcation(Graph &graph0, Cluster &theCluster)
{
    int i, childLength=theCluster.children.returnLen();

    if(theCluster.children.retrieve(1).returnCurrBifur()==-1)
    {
      for(i=1;i<=childLength;i++)
      {
         theCluster.children.retrieve(i).setCurrBifur(1);
         parseBifurString(graph0, theCluster.children.retrieve(i));
         updateGraph(graph0, theCluster.children.retrieve(i));
      }	     
      return(true);
    }

    int numBifur, currBifur;

    for(i=1;i<=childLength;i++)
    {
       numBifur=theCluster.children.retrieve(i).returnNumBifurs();
       currBifur=theCluster.children.retrieve(i).returnCurrBifur();
       if(numBifur==currBifur)
       {
         theCluster.children.retrieve(i).setCurrBifur(1);
         parseBifurString(graph0, theCluster.children.retrieve(i));
         updateGraph(graph0, theCluster.children.retrieve(i));
         continue;
       }
       theCluster.children.retrieve(i).setCurrBifur(currBifur+1);
       parseBifurString(graph0, theCluster.children.retrieve(i));
       updateGraph(graph0, theCluster.children.retrieve(i));
       return(true);
    }
    return(false);
}

/*  Chooses the bifurcation for theCluster.  Two automatic methods are used first:

    if there is only one bifurcation it is selected.
    if the cluster contains only original vertices and the bifurcation only adjusts reflections
      in space, the first bifurcation is chosen.

    In any other case, the method outputs the information about the clusters to the sketcher
    which queries the user for the correct bifurcation.  The format for this output is:
  
    <ID of the cluster whose bifurcations are being chosen>
    <number of bifurcations>
    
    for each bifurcation:
 
    <output of the generateOutput method above called with theCluster set to the correct bifur.>

*/
int selectBifurcation(Graph &graph0, Cluster &theCluster, bool &useFile)
{
    int i, answer=-1;
    int numBifurs=theCluster.returnNumBifurs();
    int tag=0;
    string answerString, bifurString;
    ofstream outfile;
    ifstream infile;
    bool validToSkip=true;

    useFile=false;

    if(numBifurs==1) return 1;
    if(numBifurs==0) return 0;

    int childLength=0, totalSingle=0;

    childLength=theCluster.children.returnLen();

    bifurString=theCluster.returnCurrBifurString();

    if(strchr(bifurString.c_str(),'r')!=NULL) validToSkip=false;
//    if(strchr(bifurString.c_str(),'m')!=NULL) validToSkip=false;

    int type2count=0;

    for(i=1;i<=childLength;i++)
    {
       if(theCluster.children.retrieve(i).returnOrigLen()>1) totalSingle++;
       if(theCluster.children.retrieve(i).returnType()==2) type2count++;
    }

    if(type2count==childLength) return 1;
    if(totalSingle==0 && validToSkip) return 1;

    useFile=true;
   
    int temp=currPosI;
 
    inputInts[currPosI++]=theCluster.returnName();
    inputInts[currPosI++]=numBifurs;
    for(i=1;i<=numBifurs;i++)   
    {  
       theCluster.setCurrBifur(i);
       parseBifurString(graph0, theCluster);
       updateGraph(graph0, theCluster);
       generateOutputToArray(graph0, theCluster, i==1);
    }

    cout<<"Values sent to sketcher"<<endl;

    for(i=temp;i<currPosI;i++)
     	cout<<inputInts[i]<<endl;

    theCluster.setCurrBifur(-1);

/*  REMOVE OLD SELECTION METHODS

    outfile.open("./Sketcher/testin.txt");
    outfile<<-1;
    outfile.close();

    outfile.open(outFile.c_str());

    if(first)
    {
       first=false;
       outfile<<-2<<endl;
       outfile<<roots.returnLen()<<endl;
       outputDRDAG(roots,outfile);
       outfile<<-1<<endl;
    }
    outfile<<theCluster.returnName()<<endl<<numBifurs<<endl;
    for(i=1;i<=numBifurs;i++)
    {
       theCluster.setCurrBifur(i);
       parseBifurString(graph0, theCluster);
       updateGraph(graph0, theCluster);
       generateOutput(graph0, theCluster, outfile, i==1);
    }
    outfile.close();

    while(answer<0)
    {
       infile.open("./Sketcher/testin.txt");
       infile>>answer;
       infile.close();
    }
    
    return answer+1; 

    answerString="";

    printBifurcations(theCluster);
    cout<<endl;

    while(answer<1 || answer>numBifurs)
    {
        if(tag==1) cout<<"   Incorrect selection, Please make another: ";
        if(tag==0) {cout<<"   Select a bifurcation to continue: "; tag=1; }
        cin>>answerString;
        answer=atoi(answerString.c_str());
    }
    cout<<endl;
    keys.append(answer);
    return (answer); 

END REMOVE OLD SELECTION METHODS */

}

//chooses the largest child of theCluster and sets it to be withheld, meaning
//when theCluster is reconciled, all of the other clusters will be rotated into
//the coordinate system of this withheld cluster
void setWithHeldCluster(Cluster &theCluster)
{
    int i, length=theCluster.children.returnLen();
    int temp=0;
    int currLen=0;
    int origLen;

    for(i=1;i<=length;i++)
    {
       origLen=theCluster.children.retrieve(i).returnOrigLen();
       if(origLen>1 && origLen>currLen)
       {
         temp=theCluster.children.retrieve(i).returnName();
         currLen=origLen;
       }
    }
    withHeldCluster=temp;
}

//eliminates any clusters that do not satisfy all of the imaginary constraints
void checkBifurs(Graph &graph0, Cluster &theCluster)
{
    int i,j,num,length,part1,part2;
    Edge temp;
    Vertex v1,v2;
    bool out;
    List<string> theList;

    num=theCluster.returnNumBifurs();
    length=theImags.returnLen();

    for(i=1;i<=num;i++)
    {
       out=true;
       theCluster.setCurrBifur(i);
       parseBifurString(graph0, theCluster);
       updateGraph(graph0, theCluster);
       for(j=1;j<=length;j++)
       {
          temp=theImags.retrieve(i);
          out=(out && isValidConstraint(graph0, temp, theCluster));
       }
       if(out) theList.append(theCluster.returnCurrBifurString());
    }
    theCluster.setBifurs(theList);
    theCluster.setCurrBifur(-1);
}

//sets the longest distance constraint in the cluster to .000001 less than before, this tricks
//maple to returning some numerical solutions that it otherwise would not with exact values
void setValueReduction(Graph &graph0, Cluster &theCluster)
{
    int i, length=graph0.returnNumEdg();
    int end1, end2, type;
    Edge* tempEdge;
    int longestDistIndex=-1;
    float longestDist=-1.0, value;
    bool in1, in2;

    for(i=1;i<=length;i++)
    {
       if(!graph0.hasEdge(i)) continue;
       tempEdge=graph0.EdgeAddr(i);
       end1=tempEdge->returnEnd1();
       end2=tempEdge->returnEnd2();    
       type=tempEdge->returnType();
       value=tempEdge->returnValue();
       in1=inOriginalV(end1, theCluster);
       in2=inOriginalV(end2, theCluster);
       cout<<"end1: "<<end1<<" end2: "<<end2<<" type: "<<type<<" in1: "
           <<in1<<" in2: "<<in2<<" value: "<<value<<endl;
       if(in1 && in2 && (type==0 || type==6) && value>longestDist)
       {
         longestDistIndex=i;
         longestDist=value;
       }
    }
    cout<<"longestIndex: "<<longestDistIndex<<endl;
    if(longestDistIndex>0)
    {
      tempEdge=graph0.EdgeAddr(longestDistIndex);
      tempEdge->setValue(longestDist-0.000001);
    }
}

//solves a given cluster
int solveCluster(Graph &graph0, Cluster &theCluster, bool resolve)
{
    int i, j, q, length, childLength, origLength;
    string temp, equation, answerString, childString;
    int answer=0, child, numChild;
    int tag;
    char in;
    int totalBifur,currChild=1;
    bool useFileToBifur, useFileTotal;
    Edge forOriginSet, forOriginSet2;    //stores an Edge so that it can be used for setting the origin

    childLength=(theCluster.children).returnLen();
    origLength=theCluster.returnOrigLen();

    //if the cluster has no children or no original vertices it must be an original vertex
    //end the recursion
    if(childLength==0 || theCluster.isSolved() || theCluster.returnType()==2)
    {
	 theCluster.setSolved(true);
	 return 0;
    }

    //solver the children of this cluster and store the return value in tag
    tag=solveForest(graph0, theCluster.children);   

    cout<<"Children Solved"<<endl;
 
    //if the return from the children is -2, then no solution is found, pass this up the calling stack
    if(tag==-2) return(-2);

    //if resolve is false, no additional bifurcation options remain, a -1 tag for no solution by additional
    //solutions remain is invalid here pass value up calling stack
    if(!resolve && tag==-1) return(-1);

    //answer is set to three when a valid solution is found, otherwise this loop tries to find a
    //solution with each different set of bifurcations of its children
    while(answer!=3)
    {
         useFileToBifur=true;
         useFileTotal=false;

	   //empty the lists of variables and imaginary constaints
         vars.makeEmpty();
         theImags.makeEmpty();
     
         cout<<"Solving Cluster "<<theCluster.returnName()<<"..."<<endl;
         answer=0;
         totalBifur=0;
         
	   //output the current cluster to the screen
         print(graph0, theCluster.children);

	   //count the total number of bifurcations availible to the children
         for(i=1;i<=childLength;i++)
            if(theCluster.children.retrieve(i).returnOrigLen()==0)
              totalBifur++;
 
         cout<<endl;
    
         cout<<"Bifurcations counted."<<endl;

         bool getNextB;

         //in the autosolve mode, select the next bifurcation combination with the getNextBifur
         //method, if no solution exists when immediate children are resolved, resolve their children
         if(autoSolve)
         {
           getNextB=getNextBifurcation(graph0,theCluster);
           print(graph0, theCluster.children);
           if(getNextB)
             while(currChild<=childLength && solveCluster(graph0,theCluster.children.retrieve(currChild),true)==-1)
                  currChild++;
           if(currChild>childLength) return(-1); 
         }        
         //if not set in autosolve, and the bifurcation answers returned from the sketcher have not been placed
         else if(usedBifurs)
              {
                cout<<"Solver usedBifurs"<<endl;

		    //save the state of the DRDag and the graph
                saveState(graph0);

                //output all of the bifurcation options of all of the children of this cluster
                //into the transfer array
		    cout<<"Solver state saved"<<endl;
                inputInts[0]++;
                currPosI=inputInts[0]+2;
                currPosF=(int) inputFloats[0];
                inputInts[inputInts[0]-1]=0;               

		    int count=0, currPosBackUp=currPosI++;
		    int theNumTrees=toSolverTrees->returnLen();
                for(i=1;i<=theNumTrees;i++)
                   outputFinState(toSolverTrees->retrieve(i), currPosI, inputInts, count);
		    
                inputInts[currPosBackUp]=count;

                for(i=1;i<=childLength;i++)
                {  
		       cout<<"Child processed: "<<i<<endl;
                   selectBifurcation(graph0,theCluster.children.retrieve(i),useFileToBifur);
                   useFileTotal=(useFileTotal || useFileToBifur);
                   if(useFileToBifur) 
                   {
                     cout<<"useFileToBifur is true"<<endl;
                     inputInts[inputInts[0]-1]++;
                     continue;
                   }
		       cout<<"useFileToBifur is false"<<endl;
                   parseBifurString(graph0, theCluster.children.retrieve(i));
		       cout<<"Bifur parsed"<<endl;
                   updateGraph(graph0, theCluster.children.retrieve(i));
 		       cout<<"Graph updated"<<endl;
                }
                inputInts[inputInts[0]]=0;
                inputInts[inputInts[0]+1]=inputInts[inputInts[0]-1];

                cout<<"Ending Solver usedBifurs"<<endl;
              }

              //if the bifurcations have not been placed yet set the children of this cluster
              //to have those bifurcation values
              else 
              {
                cout<<"Solver not usedBifur"<<endl;

                tag=0;
                for(i=1;i<=childLength;i++)
                {
                   //if(theCluster.children.retrieve(i).returnCurrBifur()>=0) continue;
                   if(theCluster.children.retrieve(i).children.returnLen()==0) continue;
                   cout<<"Bifur Used: "<<bifurAnswers[tag]<<endl;
                   theCluster.children.retrieve(i).setCurrBifur(bifurAnswers[tag++]);
                   parseBifurString(graph0, theCluster.children.retrieve(i));
                   updateGraph(graph0, theCluster.children.retrieve(i));
                   usedBifurs=true;
                }
              }                   
         
         //useFileTotal is set if any of the children have bifurcations, meaning that output to
         //the sketcher has been generated, and the method must be closed and control returned to
         //the sketcher so that an answer to the bifurcations can be chosen
         if(useFileTotal) cout<<"Bifurs written, returning"<<endl;
         if(useFileTotal) return -2;

	 print(graph0,theCluster.children);	 

	   //name of input file generated for Maple
         temp="input.txt";
         aOverlap=0;
         equationCount=0;

	   //sets the global cluster pointer, withHeldCluster
         setWithHeldCluster(theCluster);
         cout<<"ABOUT TO SET REDUCTION"<<endl;

	   //reduction is only used when the cluste contains distance constraints
         bool useReduction=true;
         int theVName;
         length=graph0.returnNumVer();
         for(i=1;i<=length;i++)
         {
            cout<<useReduction<<" ";
            cout<<graph0.returnVertByIndex(i).returnType()<<endl;
            theVName=graph0.returnVertByIndex(i).returnName();
            if(getChildNameWithVertex(theCluster, theVName)!=theVName) continue;
            if(graph0.returnVertByIndex(i).returnType()!=0 && 
	       graph0.returnVertByIndex(i).returnType()!=6  ) useReduction=false;
         }
      
         if(useReduction) setValueReduction(graph0, theCluster);
         
         ofstream inputFile;

	   //delete the old input file
         remove(temp.c_str());

         cout<<"Input File opened"<<endl;   
         inputFile.open(temp.c_str());

	 string outputString;

         //generate equations
	 outputString=getEquation(graph0, theCluster, inputFile);
	 
	 inputFile<<outputString<<endl;
	 inputFile.close();

	 //ofstream newInputFile;
	 cout << "Tree Flag in mfa9Solver.cpp: " << treeFlag << endl;
	 if (treeFlag==1)
	   {
	     char confirm;
	     cout << "Would you like to supply the file containing modified equations: (Y/N)? " ;
	     cin >> confirm;
	     cout << endl;
	     if (confirm == 'y' || confirm =='Y')
	       {
		 cout << "Supply name of file : ";
		 string temp_buff;
		 cin >> temp_buff;
		 cout << endl;
		 inputFile.open(temp_buff.c_str());
		 inputFile << outputString <<endl;  //as of now copy the same file
		 inputFile.close();
	       }
	   }


	 //the output information restored and altered in case the numeric solver must be used
	 int semiCount=0;
         int commaCount=0;
         ifstream outTest;
         string stringInputData="";
         outTest.open(temp.c_str());
	   outTest>>in;
	   while(!outTest.eof() && in!=',') outTest>>in;
	   while(!outTest.eof() && semiCount<1)
         {
	        outTest>>in;
              stringInputData+=in;
              if(in==';')
              {
                 stringInputData+='\n';
                 semiCount++;
	        }
         }
	   outTest.close();

	   //generated equations output to the screen         
         outTest.open(temp.c_str());
         outTest>>in;
         while(in!='{') outTest>>in;
         outTest>>in;
         while(in!='}')  
         { 
           if(in==',' || in==';') cout<<endl;
           else cout<<in;
           outTest>>in;
         }
         cout<<endl;
         cout<<getVarString()<<endl;
         outTest.close();

	 cout<<"String input data"<<endl;

	 cout<<stringInputData<<endl;

         //maple shelled
         shellMaple();

//	 postProcessMatlab();
         //bifurcation strings are read from output.txt, the maple output file

	 cout<<"After postprocesser"<<endl;

         setBifurs("output.txt",theCluster, false);

	 cout<<"After bifurcations set"<<endl;

 	   //if no solution is found with the symbolic solver, the number solver is tried
	   if(theCluster.returnNumBifurs()==0)
         {
           cout<<"Using Numeric Solver"<<endl;
           inputFile.open(temp.c_str());
           inputFile<<"interface(prettyprint=0);"<<endl;
           inputFile<<"timelimit(60,f";
           inputFile<<stringInputData;
           inputFile.close();
           shellMaple();
           setBifurs("output.txt",theCluster, true);
         }

	   //found bifurcations are printed
         printBifurcations(theCluster);

	 cout<<"After printing bifurs"<<endl;

         //bifurcations are checked to see if they meet the imaginary constraints
         checkBifurs(graph0, theCluster);

	   //bifurcation choice is set to -1 -- none chosen yet
         theCluster.setCurrBifur(-1);

	   //if when autosolving no valid bifurcations are found, terminate
         if(theCluster.returnNumBifurs()==0 && !useFileTotal && !autoSolve) 
         {
	     string fName;

             int verCount;
	     int edgeCount;

	     verCount=graph0.returnNumVer();
	     edgeCount=graph0.returnNumEdg();

	     cout<<"Num Edges="<<edgeCount<<endl;

	     bool loadData=true;

	     switch(verCount)
	     {
		case 6:	 fName="oct.dat";
			 cout<<"octPlat Loaded"<<endl;
			 break;
		case 8:  fName="sqr.dat";
			 cout<<"squarePlat Loaded"<<endl;
			 break;
		case 10: fName="pen.dat";
			 cout<<"pentPlat Loaded"<<endl;
			 break;
		default: loadData=false;
			 break;
	     }

	     //used a canned answer file	
	     if(loadData)
	     {
	       ifstream theData;
		
    	       theData.open(fName.c_str());

               saveState(graph0);
		
               currPosI=inputInts[0];
               currPosF=(int) inputFloats[0];
		
               int inputNum;

	       theData>>inputNum;
	       while(inputNum!=-1)
	       {
	          inputInts[currPosI++]=inputNum;
	  	  theData>>inputNum;
	       }
		
	       int countFloat, i;
	       float inputFlt;

	       theData>>countFloat;
		
	       for(i=0;i<countFloat;i++)
	       {
	  	  theData>>inputFlt;
		  inputFloats[currPosF++]=inputFlt;
	       }

	       theData.close();

	       return(-2);
	     }
	     //report solve failure
	     else
	     {
		saveState(graph0);
		currPosI=inputInts[0];
                currPosF=(int) inputFloats[0];
		inputInts[currPosI++]=-3;
		inputInts[currPosI++]=0;
		return(-2);
	     }
         } 

	   //if there are no bifurcations and in autosolve, resolve this cluster with different bifurcations
         if(theCluster.returnNumBifurs()==0 && autoSolve) answer=1;

         //if this cluster has bifurcations, either in autosolve or not, continue
         if(theCluster.returnNumBifurs()!=0 && autoSolve) answer=3;
         if(!autoSolve) answer=3;
    }
  
    //set the fin flag of this cluster to true
    theCluster.setSolved(true);
    return 0;
}

//calls solve cluster for every cluster in theForest
int solveForest(Graph &graph0, List<Cluster> &theForest)
{
    int length, i, tag;

    length=theForest.returnLen();
    for(i=1;i<=length;i++)
    {
       cout<<"About to call Solve Cluster"<<endl;
       tag=solveCluster(graph0, theForest.retrieve(i), false);
       if(tag<0) return tag;
    }
    return 0;
}

//test method which generates an equation for every type of constraint/vertex combination
void testEquations(Graph &graph0)
{
   int i,j,k,part1,part2;
   Edge testEdge;
   ofstream dataOut;
   Cluster dummy;
   string outData;
   string one, two, three;

   dataOut.open("../outFile.txt");

   testVertex1.setName(1);
   testVertex2.setName(2);
   testEdge.setName(999);

   for(i=0;i<=6;i++)
   {
      switch(i)
      {
          case 0: one="Distance";
                  break;
          case 1:
          case 2: one="Incidence";
                  break;
          case 3: one="Angle";
                  break;
          case 4: one="Parallel";
                  break;
          case 5: one="Perpendicular";
                  break;
          case 6: one="Tangency";
                  break;
      }
      dataOut<<one+" Equations:"<<endl;
      for(j=0;j<=12;j++)
      {
         switch(j)
         {
             case 0: two="Point";
                     type1=0;
                     part1=1;
                     break;
             case 1: two="Line";
                     type1=1;
                     part1=0;
                     break;
             case 2: two="Ray (arb)";
                     type1=2;
                     part1=0;
                     break;
             case 3: two="Ray (Pt1)";
                     type1=2;
                     part1=1;
                     break;
             case 4: two="Line Seg (arb)";
                     type1=3;
                     part1=0;
                     break;
             case 5: two="Line Seg (Pt1)";
                     type1=3;
                     part1=1;
                     break;
             case 6: two="Line Seg (Pt2)";
                     type1=3;
                     part1=2;
                     break;
             case 7: two="Circle (arb)";
                     type1=4;
                     part1=0;
                     break;
             case 8: two="Circle (center)";
                     type1=4;
                     part1=3;
                     break;
             case 9: two="Arc (arb)";
                     type1=5;
                     part1=0;
                     break;
             case 10: two="Arc (Pt1)";
                      type1=5;
                      part1=1;
                      break;
             case 11: two="Arc (Pt2)";
                      type1=5;
                      part1=2;
                      break;
             case 12: two="Arc (Center)";
                      type1=5;
                      part1=3;
                      break;
         }
         dataOut<<" End 1 is a/an "+two<<endl;
         for(k=0;k<=12;k++)
         {
            switch(k)
            {
                case 0:  three="Point:          ";
                         type2=0;
                         part2=1;
                         break;
                case 1:  three="Line:           ";
                         type2=1;
                         part2=0;
                         break;
                case 2:  three="Ray (arb):      ";
                         type2=2;
                         part2=0;
                         break;
                case 3:  three="Ray (Pt1):      ";
                         type2=2;
                         part2=1;
                         break;
                case 4:  three="Line Seg (arb): ";
                         type2=3;
                         part2=0;
                         break;
                case 5:  three="Line Seg (Pt1): ";
                         type2=3;
                         part2=1;
                         break;
                case 6:  three="Line Seg (Pt2): ";
                         type2=3;
                         part2=2;
                         break;
                case 7:  three="Circle (arb):   ";
                         type2=4;
                         part2=0;
                         break;
                case 8:  three="Circle (center):";
                         type2=4;
                         part2=3;
                         break;
                case 9:  three="Arc (arb):      ";
                         type2=5;
                         part2=0;
                         break;
                case 10: three="Arc (Pt1):      ";
                         type2=5;
                         part2=1;
                         break;
                case 11: three="Arc (Pt2):      ";
                         type2=5;
                         part2=2;
                         break;
                case 12: three="Arc (center):   ";
                         type2=5;
                         part2=3;
                         break;
            }
            outData="";
            if(i<2) testEdge.setType(i);
            else testEdge.setType(i-1);
            testEdge.setPart(0,part1);
            testEdge.setPart(1,part2);
            testVertex1.setType(type1);
            testVertex2.setType(type2);
            testEdge.setValue(5.0);
            outData=getEquationInCluster(graph0, testEdge, dummy);
            if(isImaginary(graph0,testEdge)) outData+=" *** IMAGINARY ***";
            if(outData!="")
            {
              dataOut<<"  End 2 is a/an "+three<<" ";
              dataOut<<outData<<endl;
            }
         }
      }
   }
   cout<<getSinCosConstraint();
   dataOut<<getVarString()<<endl;
   dataOut.close();
}

//used for testing only
//reads a graph struction from text input in a file
void getGraphFromFile(Graph &graph0, List<Cluster> &SolverTrees)
{
   ifstream dataIn;
   string filename;
   int numShapes;
   int numEdges;
   int i, ID, type, end1, end2;
   char s;
   float value;
   List<int> originalV, nullIntList, originalV2;
   List<Vertex> vList, nullVList;
   List<Edge> eList, nullEList;
   List<Cluster> cList, nullCList;
   Graph newGraph;

   nullIntList.makeEmpty();
   nullVList.makeEmpty();
   nullEList.makeEmpty();
   nullCList.makeEmpty();

   cout<<"Enter a filename: ";
   cin>>filename;
 
   dataIn.open(filename.c_str());

   dataIn>>s;

   if(s=='Y' || s=='y') toSolve=true;
   else toSolve =false;   

   dataIn>>numShapes;

   for(i=0;i<numShapes;i++)
   {
      originalV.makeEmpty();
      dataIn>>ID;
      originalV2.append(ID);
      originalV.append(ID);
      dataIn>>type;
      Vertex currVert(ID, type);
      vList.append(currVert);
      Cluster currClust;
      currClust.formCl(currVert,nullIntList,nullEList,nullEList,originalV);
      currClust.children=nullCList;
      cList.append(currClust);
   }

   dataIn>>numEdges;

   for(i=0;i<numEdges;i++)
   {
      dataIn>>ID;
      dataIn>>type;
      value=0;
      if(type==0 || type==2) dataIn>>value;
      dataIn>>end1;
      dataIn>>end2;
      Edge currEdge;
      currEdge.setName(ID);
      currEdge.setType(type);
      currEdge.setValue(value);
      currEdge.setEnd(0,end1);
      currEdge.setEnd(1,end2);
      eList.append(currEdge);
   }

   Vertex currVert;

   currVert.setName(99);
   currVert.setType(-1);
   currVert.setDepth(1);

   Cluster newClust;

   newClust.formCl(currVert,nullIntList,nullEList,nullEList,originalV2);
   newClust.children=cList;

   SolverTrees.makeEmpty();
   SolverTrees.append(newClust);

   newGraph.setVlist(vList);
   newGraph.setElist(eList);

   graph0=newGraph;

   dataIn.close();
}

//stores a string in the output arrays by storing the ascii value of each of its chars in 
//consequtive integers
void stringToArray(string theString, int*& theInts)
{
    int indexI=theInts[0];
    int i, length=theString.size();

    for(i=0;i<length;i++) theInts[indexI++]=theString[i];
    theInts[indexI++]=-1;
    theInts[0]=indexI;
}

//reads a string in the above format
string readStringFromArray(int& start, int* theInts)
{
    string output="";    

    while(theInts[start]!=-1)
         output+=(char) theInts[start++];

    start++;
    return output;
}

//stores a vertex in the output arrays
void vertexToArray(Vertex &theVertex, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    int i, nIncid;

    theInts[indexI++]=theVertex.returnName();
    theInts[indexI++]=theVertex.returnType();
    theInts[indexI++]=theVertex.returnWeight();
    theInts[indexI++]=theVertex.returnDepth();
    theInts[indexI++]=theVertex.returnLabel();
    theInts[indexI++]=theVertex.returnScan();
    theInts[indexI++]=theVertex.isFrozen();
    theInts[indexI++]=theVertex.returnPosFlow();
    theInts[indexI++]=theVertex.returnEstFlow();
    theInts[indexI++]=(theVertex.returnIgnore() ? 1 : 0);
    nIncid=theInts[indexI++]=theVertex.numIncid();
    for(i=1;i<=nIncid;i++) theInts[indexI++]=theVertex.returnIncid(i);
    theInts[indexI++]=theVertex.returnPredEdg();
    for(i=0;i<9;i++) theInts[indexI++]=theVertex.returnFreeDegs(i);
    theInts[indexI++]=theVertex.leftEndIncid.returnLen();
    for(i=1;i<=theVertex.leftEndIncid.returnLen();i++)
	theInts[indexI++]=theVertex.leftEndIncid.retrieve(i);
    theInts[indexI++]=theVertex.rightEndIncid.returnLen();
    for(i=1;i<=theVertex.rightEndIncid.returnLen();i++)
        theInts[indexI++]=theVertex.rightEndIncid.retrieve(i);
    theInts[0]=indexI;

    for(i=0;i<9;i++) theFloats[indexF++]=theVertex.returnDegValueByName(i);
    theFloats[0]=indexF;
}

//reads a vertex from the output arrays
void readVertexFromArray(Vertex &theVertex, int &startI, int* theInts, int& startF, float* theFloats)
{   
    int i, count;    

    theVertex.setName(theInts[startI++]);
    theVertex.setType(theInts[startI++]);
    theVertex.setWeight(theInts[startI++]);
    theVertex.setDepth(theInts[startI++]);
    theVertex.setLabel(theInts[startI++]);
    theVertex.setScan(theInts[startI++]);
    count=theInts[startI++];
    if(count==2) theVertex.freezeForever();
    if(count==1) theVertex.freeze();
    theVertex.setPosFlow(theInts[startI++]);
    theVertex.setEstFlow(theInts[startI++]);
    theVertex.setIgnore(theInts[startI++]==1);
    count=theInts[startI++];
    for(i=0;i<count;i++)
       theVertex.appendIncid(theInts[startI++]);
    theVertex.setPredEdg(theInts[startI++]);
    for(i=0;i<9;i++) 
    {
       theVertex.initialValue(i,theFloats[startF++],theInts[startI++]);
    }
    count=theInts[startI++];
    for(i=0;i<count;i++)
	theVertex.leftEndIncid.append(theInts[startI++]);
    count=theInts[startI++];
    for(i=0;i<count;i++)
        theVertex.rightEndIncid.append(theInts[startI++]);
}    
    
//outputs an edge to the output arrays
void edgeToArray(Edge &theEdge, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    
    theInts[indexI++]=theEdge.returnName();
    theInts[indexI++]=theEdge.returnType();
    theInts[indexI++]=theEdge.returnWeight();
    theInts[indexI++]=theEdge.returnLabel();
    theInts[indexI++]=theEdge.returnScan();
    theInts[indexI++]=theEdge.returnPosFlow();
    theInts[indexI++]=theEdge.returnFlow1();
    theInts[indexI++]=theEdge.returnFlow2();
    theInts[indexI++]=theEdge.returnEnd1();
    theInts[indexI++]=theEdge.returnPart1();
    theInts[indexI++]=theEdge.returnEnd2();
    theInts[indexI++]=theEdge.returnPart2();
    theInts[indexI++]=theEdge.returnPredVer();
    theInts[0]=indexI;

    theFloats[indexF++]=theEdge.returnValue();
    theFloats[0]=indexF;
}

//reads an edge from the output arrays
void readEdgeFromArray(Edge &theEdge, int &startI, int* theInts, int &startF, float* theFloats)
{    
    theEdge.setName(theInts[startI++]);
    theEdge.setType(theInts[startI++]);
    theEdge.setWeight(theInts[startI++]);
    theEdge.setLabel(theInts[startI++]);
    theEdge.setScan(theInts[startI++]);
    theEdge.setPosFlow(theInts[startI++]);
    theEdge.setFlow1(theInts[startI++]);
    theEdge.setFlow2(theInts[startI++]);
    theEdge.setEnd(0,theInts[startI++]);
    theEdge.setPart(0,theInts[startI++]);
    theEdge.setEnd(1,theInts[startI++]);
    theEdge.setPart(1,theInts[startI++]);    
    theEdge.setPredVer(theInts[startI++]);
    theEdge.setValue(theFloats[startF++]);
}
    
//outputs a graph to an array
void graphToArray(Graph &graph0, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    int i, count;
   
    theInts[indexI++]=graph0.returnDimen();
    theInts[indexI++]=graph0.returnDepth();
    count=theInts[indexI++]=graph0.returnNumVer();
    theInts[0]=indexI;
    for(i=1;i<=count;i++) vertexToArray(graph0.returnVertByIndex(i), theInts, theFloats);
    indexI=theInts[0];

    count=theInts[indexI++]=graph0.returnNumEdg();
    theInts[0]=indexI;
    for(i=1;i<=count;i++) edgeToArray(graph0.returnEdgeByIndex(i), theInts, theFloats);
}

//reads a graph from the arrays
void readGraphFromArray(Graph &graph0, int &startI, int* theInts, int &startF, float* theFloats)
{
    int i, count;

    graph0.setDimen(theInts[startI++]);
    graph0.setDepth(theInts[startI++]);
    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
       Vertex newVert;
       readVertexFromArray(newVert, startI, theInts, startF, theFloats);
       graph0.appendVertex(newVert);
    }
    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
       Edge newEdge;
       readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
       graph0.appendEdge(newEdge);
    }    
}        

//outputs a cluster to the arrays
void clusterToArray(Cluster &theCluster, int*& theInts, float*& theFloats)
{
   vertexToArray(theCluster.returnCore(), theInts, theFloats);

   int indexI=theInts[0];
   int indexF=(int) theFloats[0];
   int i, j, count, mem;
   
   theInts[indexI++]=theCluster.returnGroup();
   theInts[indexI++]=theCluster.returnType();
   count=theInts[indexI++]=theCluster.returnFronLen();
   for(i=1;i<=count;i++) theInts[indexI++]=theCluster.returnFrontier(i);
   count=theInts[indexI++]=theCluster.returnInnerELen();
   theInts[0]=indexI;
   for(i=1;i<=count;i++) edgeToArray(theCluster.returnInnerE(i), theInts, theFloats);
   indexI=theInts[0];
   count=theInts[indexI++]=theCluster.returnOuterELen();
   theInts[0]=indexI++;
   for(i=1;i<=count;i++) edgeToArray(theCluster.returnOuterE(i), theInts, theFloats);
   indexI=theInts[0];
   indexF=(int) theFloats[0];
   count=theInts[indexI++]=theCluster.returnOrigLen();
   for(i=1;i<=count;i++) theInts[indexI++]=theCluster.returnOrigV(i);
   count=theInts[indexI++]=theCluster.returnNumBifurs();
   mem=theInts[indexI++]=theCluster.returnCurrBifur();
   theInts[0]=indexI;
   for(i=1;i<=count;i++)
   {
      theCluster.setCurrBifur(i);
      stringToArray(theCluster.returnCurrBifurString(), theInts);
   }   
   theCluster.setCurrBifur(mem);
   indexI=theInts[0];
   theInts[indexI++]=theCluster.currAlias;
   count=theInts[indexI++]=theCluster.left.returnLen();
   for(i=1;i<=count;i++)
	theInts[indexI++]=theCluster.left.retrieve(i);
   count=theInts[indexI++]=theCluster.right.returnLen();
   for(i=1;i<=count;i++)
        theInts[indexI++]=theCluster.right.retrieve(i);
   count=theInts[indexI++]=theCluster.lines.returnLen();
   for(i=1;i<=count;i++)
        theInts[indexI++]=theCluster.lines.retrieve(i);
   if(theCluster.isSolved()) theInts[indexI++]=0;
   else theInts[indexI++]=-1;
   theInts[indexI++]=theCluster.returnConst();
   mem=theInts[indexI++]=theCluster.returnCurrDeg();
   count=theInts[indexI++]=theCluster.returnNumDeg();

   for(i=1;i<=count;i++)
   {
      theCluster.setCurrDeg(i);
      for(j=0;j<9;j++) theFloats[indexF++]=theCluster.returnValue(j);
   }
   theCluster.setCurrDeg(mem);
   theFloats[0]=indexF;

   count=theInts[indexI++]=theCluster.children.returnLen();
   theInts[0]=indexI;
   for(i=1;i<=count;i++)
      clusterToArray(theCluster.children.retrieve(i), theInts, theFloats);
}

//reads a cluster from the arrays
void readClusterFromArray(Cluster &theCluster, int &startI, int* theInts, int &startF, float* theFloats)
{
   Vertex core;

   readVertexFromArray(core, startI, theInts, startF, theFloats);

   int i, j, count;

   theCluster.setGroup(theInts[startI++]);
   theCluster.setType(theInts[startI++]);

   List<int> fronts, origs;
   List<Edge> inner, outer;

   count=theInts[startI++];
   for(i=0;i<count;i++) fronts.append(theInts[startI++]);
   
   count=theInts[startI++];
   for(i=0;i<count;i++)
   {
      Edge newEdge;
      readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
      inner.append(newEdge);
   }

   count=theInts[startI++];
   for(i=0;i<count;i++)
   {
      Edge newEdge;
      readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
      outer.append(newEdge);
   }

   count=theInts[startI++];
   for(i=0;i<count;i++) origs.append(theInts[startI++]);

   theCluster.formCl(core, fronts, inner, outer, origs);

   List<string> strings;
   int currBifur;

   count=theInts[startI++];
   currBifur=theInts[startI++];
   for(i=0;i<count;i++)
   { 
      string test=readStringFromArray(startI, theInts);
      strings.append(test);
   }
   theCluster.setBifurs(strings);
   theCluster.setCurrBifur(currBifur);

   theCluster.currAlias=theInts[startI++];
   count=theInts[startI++];
   for(i=0;i<count;i++)
	theCluster.left.append(theInts[startI++]);
   count=theInts[startI++];
   for(i=0;i<count;i++)
        theCluster.right.append(theInts[startI++]);
   count=theInts[startI++];
   for(i=0;i<count;i++)
        theCluster.lines.append(theInts[startI++]);

   theCluster.setSolved(theInts[startI++]==0);
   theCluster.setConst(theInts[startI++]);

   List<DegValues> values;
   int currDeg;

   currDeg=theInts[startI++];
   count=theInts[startI++];

   for(i=0;i<count;i++)
   {
      DegValues newDeg;
      for(j=0;j<9;j++) newDeg.setValue(j,theFloats[startF++]);
      values.append(newDeg);
   }
   theCluster.setDegValues(values);
   theCluster.setCurrDeg(currDeg);

   count=theInts[startI++];
   for(i=0;i<count;i++)
   {
      Cluster newClust;
      readClusterFromArray(newClust, startI, theInts, startF, theFloats);
      theCluster.children.append(newClust);
   }
}


/*

THE JNI ARRAYS BEING LOADED AND UNLOADED WITH THE TREE INFORMATION


//stores the root of the Conversion Tree in the output arrays by storing the pointer to the Root node of the Tree 
void treeToArray(Node *Root, int*& theInts)
{
    int indexI=theInts[0];
    theInts[indexI++]=(int)Root;
    theInts[0]=indexI;
}

//reads the root of the Tree from the array
Node* readTreeFromArray(int& start, int* theInts)
{
    Node *Root=(Node *)theInts[start++];
    start++;
    return Root;
}

*/

//prints the arrays to a file
void outputArrays(int* theInts, float* theFloats)
{
   int i;
   ofstream arrayOut;

   arrayOut.open("arrayOut.txt");

   for(i=0;i<INTSIZE;i++)
      arrayOut<<theInts[i]<<endl;
   
   for(i=0;i<FLOATSIZE;i++)
      arrayOut<<theFloats[i]<<endl;

   arrayOut.close();
}


//reads the arrays from a file
void inputArrays(int*& theInts, float*& theFloats)
{
   int i;

   ifstream arrayIn;

   arrayIn.open("arrayOut.txt");

   for(i=0;i<INTSIZE;i++) 
   {  
      arrayIn>>theInts[i];
      if(theInts[0]==-1) return;
   }

   for(i=0;i<FLOATSIZE;i++) arrayIn>>theFloats[i];   

   arrayIn.close();
}

//saves the DRTree and the graph into the output arrays, used when called within this file
void saveState(Graph &graph0)
{
     int i, length;

     length=toSolverTrees->returnLen();

     inputInts[0]=2;
     inputFloats[0]=1.0f;
     inputInts[1]=treeFlag;
/*

     cout << "Comes into calling one" << endl;
     Tree.copyTreeIntoArray(inputInts, inputChars);
     cout << "Comes into after calling one" << endl;
*/

     graphToArray(gGraph1, inputInts, inputFloats);
     graphToArray(graph0, inputInts, inputFloats);
     inputInts[inputInts[0]++]=length;
     for(i=1;i<=length;i++)
        clusterToArray(toSolverTrees->retrieve(i), inputInts, inputFloats);
}

/* LEGACY CODE
saves the DRTree and the graph into the output arrays, used when caled outside this file
void saveState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, jint*& thejInts, jdouble*& thejDoubles)
{
     int i, length;
     int *inputInts=new int[INTSIZE];
     float *inputFloats=new float[FLOATSIZE];

     length=SolverTrees.returnLen();

     inputInts[0]=1;
     inputFloats[0]=1.0f;
     graphToArray(graph1, inputInts, inputFloats);
     graphToArray(graph0, inputInts, inputFloats);
     inputInts[inputInts[0]++]=length;
     for(i=1;i<=length;i++)
        clusterToArray(SolverTrees.retrieve(i), inputInts, inputFloats);
     for(i=0;i<INTSIZE;i++)
        thejInts[i]=inputInts[i];
     for(i=0;i<FLOATSIZE;i++)
        thejDoubles[i]=inputFloats[i];     
}

*/

//loads the DRTree and graph from the arrays
void loadState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees,
int &startI, jint *thejInts, int &startF, jdouble *thejDoubles, jchar *thejChars)
{
    int i, length;
    int *inputInts=new int[INTSIZE];
    float *inputFloats=new float[FLOATSIZE];
    char *inputChars=new char[CHARSIZE];

    for(i=0;i<INTSIZE;i++)
       inputInts[i]=thejInts[i];
 
    for(i=0;i<FLOATSIZE;i++)
       inputFloats[i]=(float) thejDoubles[i];

    for(i=0;i<CHARSIZE;i++)
       inputChars[i]=thejChars[i];
/*
    cout << "Code did come into this.... see this here??" <<  endl;
    Tree.copyArrayIntoTree(startI, inputInts, inputChars);
*/
    graph0.makeEmpty();
    graph1.makeEmpty();
    SolverTrees.makeEmpty();
    readGraphFromArray(graph1, startI, inputInts, startF, inputFloats);
    readGraphFromArray(graph0, startI, inputInts, startF, inputFloats);

    length=inputInts[startI++];

    for(i=0;i<length;i++)
    {
       Cluster newClust;
       readClusterFromArray(newClust, startI, inputInts, startF, inputFloats);
       SolverTrees.append(newClust);
    }

    startI++;
}

//Sets the contents of the globals inputInts and inputFloats 
//to output arrays inputTheInts and inputDouble
void setArraysForOutput(jint*& theJInts, jdouble*& theJDoubles)
{
   int i;

   for(i=0;i<FLOATSIZE;i++)
      theJDoubles[i]=inputFloats[i];

   outputArrays(inputInts, inputFloats);
}

//Sets the contents of the globals inputInts and inputFloats
//to input arrays inputTheInts and inputDouble
void setArraysForInput(jint*& theJInts, jdouble*& theJDoubles, jchar*& theJChars)
{
   int i;

   inputChars=new char[CHARSIZE];

   inputChars=(char*) theJChars;

   inputInts=new int[INTSIZE];

   inputInts=(int*) theJInts;

   inputFloats=new float[FLOATSIZE];

   for(i=0;i<FLOATSIZE;i++)
      inputFloats[i]=(float) theJDoubles[i];   
}

//resets the fin state of all of the cluster in the DRTree above theCluster
bool resetFinByClusterName(Cluster &theCluster, int name)
{
   int i, length;
   bool anyChild=theCluster.isSolved();

   if(theCluster.returnName()==name)
   {
	theCluster.setSolved(false);
	return false;
   }

   length=theCluster.children.returnLen();

   for(i=1;i<=length;i++)
	anyChild=resetFinByClusterName(theCluster.children.retrieve(i),name) && anyChild;

   theCluster.setSolved(anyChild);
   return anyChild;
}

//resets the DRTrees datastructure to contain only the original vertices in the graph
//this method is used right before calling the DRPlanner to force it to recalculate the
//entire tree
void resetTrees(Graph &graph0, List<Cluster> &SolverTrees)
{
   int i, length;
   Cluster newCluster;
   List<Edge> innerE, outerE;
   List<int> frontiers, originalV;
   Vertex core;
   int coreName;

   SolverTrees.makeEmpty();

   length=graph0.returnNumVer();
   
   for(i=1;i<=length;i++)
   {
      core=graph0.returnVertByIndex(i);
      coreName=core.returnName();
      core.defrost();
      graph0.defrostOneVert(coreName);
      frontiers.append(coreName);
      originalV.append(coreName);
      newCluster.formCl(core, frontiers, innerE, outerE, originalV);
      SolverTrees.append(newCluster);
      frontiers.makeEmpty();
      originalV.makeEmpty();
   }

   singleVertex=length+4;
}

//eliminates extra vertex in frontiers latent after DRPlanner
void fixTree(List<Cluster> &SolverTrees)
{
   int i, length;

   length=SolverTrees.returnLen();

   for(i=1;i<=length;i++)
      fixCluster(SolverTrees.retrieve(i));
}

//as fixTrees but for clusters
void fixCluster(Cluster &theCluster)
{
   Vertex theCore;
   int i, length, coreName;

   theCore=theCluster.returnCore();
   coreName=theCore.returnName();

   if(coreName==0) return;

   if(theCore.returnDepth()==0)
   {
     length=theCluster.returnOrigLen();
     for(i=1;i<=length;i++)
        if(theCluster.returnOrigV(i)!=coreName)
          theCluster.deleteOrig(theCluster.returnOrigV(i));
   }
   else fixTree(theCluster.children);
}

//given theEdge as input, resets all of the clusters in SolverTrees that contain both endpoints of theEdge as
//original vertices.  the method returns a count of the number of the vertices so reset
int resetTreesByEdge(List<Cluster> SolverTrees, Edge &theEdge)
{
   int i, length;
   int end1, end2;
   int count=0;

   length=SolverTrees.returnLen();
   end1=theEdge.returnEnd1();
   end2=theEdge.returnEnd2();

   for(i=1;i<=length;i++)
      if(inOriginalV(end1, SolverTrees.retrieve(i)) && inOriginalV(end2, SolverTrees.retrieve(i)))
      {
        SolverTrees.retrieve(i).setSolved(false);
        count+=1+resetTreesByEdge(SolverTrees.retrieve(i).children, theEdge);
      }

   return count;
}

//checks to see if theEdge is valid using the already solved values in SolverTrees
bool checkEdge(Edge &theEdge, Graph &graph0, List<Cluster> &SolverTrees)
{
   int end1, end2;
   Vertex v1, v2;
   string input;

   vars.makeEmpty();
   input=getEquationInCluster(graph0, theEdge, SolverTrees.retrieve(1));
   end1=theEdge.returnEnd1();
   v1=graph0.returnVertByName(end1);
   end2=theEdge.returnEnd2();
   v2=graph0.returnVertByName(end2);
   input=switchString(v1, v1.returnType(), v1.returnName(), 0, input, false);
   input=switchString(v2, v2.returnType(), v2.returnName(), 0, input, false);	

   input.replace(input.find("=",0),1,"-");

   ofstream inputFile;
   remove("input.txt");
   inputFile.open("input.txt");

   inputFile<<"evalb(abs("<<input<<")<0.00001);"<<endl;
   inputFile.close();

   shellMaple();        

   cout<<input<<endl;
   cout<<getVarString()<<endl;

   ifstream outputFile;
   
   outputFile.open("output.txt");

   while(outputFile.get()!=';') if(outputFile.eof()) return false;
   
   char in;

   outputFile>>in;
   while(in!='t' || in!='f')
   {
        if(outputFile.eof()) return false;
        outputFile>>in;
   }
   if(in=='f') return false;
   else return true;
}

//deletes clusters from SolverTrees that contain theEdge
void deleteClusterWithEdge(Graph &graph0, List<Cluster> &SolverTrees, Edge &theEdge)
{
   int end1, end2;

   end1=theEdge.returnEnd1();
   end2=theEdge.returnEnd2();
   
   int i=1, length=SolverTrees.returnLen();
   
   while(i<=length)
   {
      print(graph0, SolverTrees);
      if(inOriginalV(end1,SolverTrees.retrieve(i)) && inOriginalV(end2,SolverTrees.retrieve(i)))
      {
        int j,childLength;

        childLength=SolverTrees.retrieve(i).children.returnLen();
        
        for(j=1;j<=childLength;j++)
           SolverTrees.append(SolverTrees.retrieve(i).children.retrieve(j));

	SolverTrees.deleteIndex(i);
      }
      else i++;
      length=SolverTrees.returnLen();
   }
}

// formats the input string so that it can be taken as an input M-file in Matlab
string processMatlab(string input)
{
        input=input.erase(0,46);
	input=input.erase(input.find("}"));
	input=replaceAll(input, ",", ";\n");
	input=replaceAll(input, "=", "-");
	input=input+";";

	string temp="";
	string backup=varsToZero;

	bool everFull=false;

	while(varsToZero!="")
	{
		everFull=true;
		temp=varsToZero.substr(0,2);
		varsToZero=varsToZero.erase(0,2);
		input=replaceAll(input, temp, "0");
		vars.deleteElem(temp);
	}

	varsToZero=backup;
	
	int i, len;
	len=vars.returnLen();
	for(i=1;i<=len;i++)
	{
		temp="X("+toString(i)+")";
		input=replaceAll(input, vars.retrieve(i),temp);
	}

	input=replaceAll(input,"\n0-0;","");
	
	int length=input.size();
	input=input.insert(0,"F(#)=");
	int count=0;
	input=replaceAll(input,";\n",";\nF(#)=");
	
	int pos=0;
	count=1;
	pos=input.find("#");
	while(pos>=0)
	{
		input.insert(pos,toString(count));
		count++;
		input.erase(pos+1,1);
		pos=input.find("#");
	}

	input="F=zeros("+toString(vars.returnLen())+",1);\n"+input;
	input="function F = dplanFun(X)\n\n"+input;

   	return input;
}

void postProcessMatlab()
{
	ifstream input;
	ofstream output;
	
	input.open("data.out");
	output.open("output.txt");

	output<<">>>>>{";
	
	int i, length=vars.returnLen();
	float indata;

	for(i=0;i<length;i++)
	{
		input>>indata;
		if(i!=0) output<<", ";
		output<<vars.retrieve(i)<<" = "<<indata;
		
	} 

	string temp;

	while(varsToZero!="")
        {
                temp=varsToZero.substr(0,2);
                varsToZero=varsToZero.erase(0,2);
		output<<", "<<temp<<" = 0.0";
        }	

	output<<"}q";
	
	input.close();
	output.close();
}
	
int countEdges(Graph &graph0, Cluster &theCluster)
{
	int i, length;
	int temp, count=0;
	Edge edge;

	length=graph0.returnNumEdg();
	for(i=1; i<=length; i++)
	{
		edge=graph0.returnEdgeByIndex(i);
		temp=getEdgeCode(edge, theCluster);
		if(temp!=-1)
			count++;
	}

	return count;
}


ClustData& summCluster(Graph &graph0, Cluster &theCluster)
{
	int len;

	len=countEdges(graph0, theCluster);

	ClustData *temp = new ClustData(len);

	int i, count=0, eCode;
	Edge edge;

	len=graph0.returnNumEdg();
	for(i=1; i<=len; i++)
	{
		int end1Name;
		int end2Name;

		edge=graph0.returnEdgeByIndex(i);
		eCode=getEdgeCode(edge, theCluster);
		
		end1Name=edge.returnEnd1();
                end2Name=edge.returnEnd2();

		if(eCode==1)
		{
			int j;
			int countSet=0;
			
			for(j=1;j<=theCluster.children.returnLen();j++)
			{
				cout<<"Current Item: "<<j<<endl;
				if(theCluster.children.retrieve(j).returnType()==2)
				{
					if(inOriginalV(end1Name,theCluster.children.retrieve(j)))
					{	
						countSet++;
						end1Name=end1Name*1000+theCluster.children.retrieve(j).returnName();
					}
 					if(inOriginalV(end2Name,theCluster.children.retrieve(j)))
                                        {       
                                                countSet++;
						end2Name=end2Name*1000+theCluster.children.retrieve(j).returnName();
					}
				}
			}
			if(countSet>1) eCode=0;
		}

		cout<<"Break"<<endl;

		if(eCode!=-1)
		{
			temp->inOrBetween[count]=(eCode==1);
			temp->edgeID[count]=edge.returnName();
			temp->edgeType[count]=edge.returnType();
			temp->e1ID[count]=end1Name;
			temp->e2ID[count]=end2Name;
                        temp->e1Type[count]=graph0.returnVertByName(edge.returnEnd1()).returnType();
			temp->e2Type[count]=graph0.returnVertByName(edge.returnEnd2()).returnType();
			temp->e1Part[count]=edge.returnPart1();
			temp->e2Part[count]=edge.returnPart2();
			temp->values[count]=edge.returnValue();

			cout<<edge<<endl;
			cout<<temp->inOrBetween[count]<<endl;
			cout<<temp->edgeID[count]<<endl;
                        cout<<temp->edgeType[count]<<endl;
                        cout<<temp->e1ID[count]<<endl;
                        cout<<temp->e2ID[count]<<endl;
                        cout<<temp->e1Type[count]<<endl;
                        cout<<temp->e2Type[count]<<endl;
                        cout<<temp->e1Part[count]<<endl;
                        cout<<temp->e2Part[count]<<endl;
                        cout<<temp->values[count]<<endl;

			count++;
		}
	}

	cout<<"Displaying Temp"<<endl;
//	cout<<*temp<<endl;

	return *temp;
}

void getClusterByName(Cluster &outputCluster, List<Cluster> &SolverTrees, int name)
{
	int i, length=SolverTrees.returnLen();

	for(i=1;i<=length;i++)
		if(SolverTrees.retrieve(i).returnName()==name)
		{
			outputCluster=SolverTrees.retrieve(i);
			return;
		}

	for(i=1;i<=length;i++)
		getClusterByName(outputCluster,SolverTrees.retrieve(i).children,name);
}

float findDist(Graph &graph0, Cluster &theCluster)
{
	int i, length=graph0.returnNumEdg();
	Edge tempEdge;

	for(i=1;i<=length;i++)
	{
		tempEdge=graph0.returnEdgeByIndex(i);
		if(!inOriginalV(tempEdge.returnEnd1(),theCluster) ||
		   !inOriginalV(tempEdge.returnEnd2(),theCluster))
			continue;

		if(tempEdge.returnType()==6 || tempEdge.returnType()==0)
			return tempEdge.returnValue();
	}

	return 0;
}

string getSpecialCaseEquations(Graph &graph0, Cluster &theCluster, int caseInt)
{
	string output="";

	switch(caseInt)
	{
		case 1:	int ID1=theCluster.children.retrieve(1).returnName();
			int ID2=theCluster.children.retrieve(2).returnName();

			string id1Name=toString(ID1);
			string id2Name=toString(ID2);

			output+="x"+id1Name+"=0,";
                        output+="y"+id1Name+"=0,";
                        output+="z"+id1Name+"=0,";
                        output+="c"+id1Name+"=0,";
                        output+="d"+id1Name+"=0,";
                        output+="c"+id2Name+"=0";
			break;
	}

	return output;
}
			


void parseForExternalOverCons(Graph &graph0, List<Cluster> *SolverTrees)
{
	int i, length;
	Cluster currClust;

	length=SolverTrees->returnLen();

	for(i=length;i>=1;i--)
	{
		currClust=SolverTrees->retrieve(i);
		if(currClust.returnType()==2)
		{
			Vertex* theLS;
			int origName;

			for(int j=1;j<=currClust.children.returnLen();j++)
			{
				theLS=graph0.VertAddr(currClust.children.retrieve(j).returnName());
				if(theLS->returnType()==7)
					break;
			}


			for(int j=currClust.children.returnLen();j>=1;j--)
			{
				origName=currClust.children.retrieve(j).returnOrigV(1);
				Vertex tempVert=graph0.returnVertByName(origName);

				if(tempVert.returnType()==6);
				{
					Edge tempEdge=graph0.returnEdgeByEnds(theLS->returnName(),origName);
					if(tempEdge.returnName()==0) continue;
					if(tempEdge.returnEnd1()==theLS->returnName())
					{
						cout<<"LS is end1"<<endl;
						if(tempEdge.returnPart1()==1)
						{
							cout<<"Part 1 is 1"<<endl;
							SolverTrees->addrByIndex(i)->setValue(6,origName);
						}
						else	
						{
							cout<<"Part 1 is 2"<<endl;
							SolverTrees->addrByIndex(i)->setValue(7,origName);
						}
					}
					else
					{
						cout<<"LS is end2"<<endl;
						if(tempEdge.returnPart2()==1)
						{
							cout<<"Part 2 is 1"<<endl;
							SolverTrees->addrByIndex(i)->setValue(6,origName);
						}
                                                else
						{	
							cout<<"Part 2 is 2"<<endl;
							SolverTrees->addrByIndex(i)->setValue(7,origName);
						}
					}
				}

			}
		}
		else parseForExternalOverCons(graph0, &(SolverTrees->retrieve(i).children));
	}
}

void Solver(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, jint* inputTheInts, jdouble* inputDouble, jchar* inputChar)
{
   //Variables
   int i, numTrees=SolverTrees.returnLen();
   ofstream outfile;
   string answer;
   bool useless;
   int count, tag;
   bool useFileTotal, useFileToBifur;

   copyG(graph1, gGraph1);

   cout<<"Solver Start"<<endl;

//   parseForExternalOverCons(graph0,&SolverTrees);
   
   setArraysForInput(inputTheInts, inputDouble, inputChar);

   graphDimen=graph0.returnDimen();

   //Global Initializations
   startI=2;
   startF=1;
   treeFlag=inputInts[1];
   usingArrays=(inputInts[0]==2 || inputInts[0]==1);
   autoSolve=(inputInts[0]==1);
   first=true;
   roots=SolverTrees;
   vars.makeEmpty();
   usedBifurs=true;
   toSolverTrees=&SolverTrees;

   cout<<"AutoSolve: "<<(autoSolve ? "true" : "false")<<endl;
   cout<<"Variable Initialized usingArrays: "<<(usingArrays ? "true" : "false")<<endl;

   //if the DRDag has just been generated, outputs it to the sketcher
   if(!usingArrays)
   {
     saveState(graph0);
     cout<<"State Saved"<<endl;
     inputInts[inputInts[0]]=-1;
     inputInts[0]++;
     
     int backupIndex=inputInts[0];

     cout<<"DRDAG Output"<<endl;

     outputDRDAGToArray(SolverTrees, backupIndex, inputInts, true);

     for(i=backupIndex; i<backupIndex+100; i++)
	cout<<inputInts[i]<<endl;

     cout<<"DAG Output"<<endl;
     outputArrays(inputInts, inputFloats);
     setArraysForOutput(inputTheInts, inputDouble);
     cout<<"Arrays output, Solver returns"<<endl;
     return;
   }          

   //runs the solver on the DRDag and if necessary sets the bifurcation of the root nodes in the DRDag
   if(usingArrays)
   {
     graph0.makeEmpty();
     SolverTrees.makeEmpty();
     gGraph1.makeEmpty();
//     Tree.copyArrayIntoTree(startI, inputInts, inputChars);
     
     for(i=startI;i<startI+100;i++)
	cout<<"Integer after Tree: "<<inputInts[i]<<endl;

     readGraphFromArray(graph1, startI, inputInts, startF, inputFloats);
     readGraphFromArray(graph0, startI, inputInts, startF, inputFloats);

     graphDimen=graph0.returnDimen();
     
     int length=inputInts[startI++]; 

     for(i=0;i<length;i++)
     {
          Cluster newClust;
          readClusterFromArray(newClust, startI, inputInts, startF, inputFloats);
          SolverTrees.append(newClust);
     }
    
     cout<<"Graph and Cluster read:"<<endl;

     numTrees=SolverTrees.returnLen();
     graph0.output(cout);
     print(graph0, SolverTrees);
     cout<<"value at startI="<<inputInts[startI]<<endl;
     count=inputInts[startI];
     if(count>0) bifurAnswers=new int[count];
     
     int temp;

     for(i=1;i<=count;i++)
     {
	if(i==1 && inputInts[startI+i]==-1)
	{
	   for(int j=1;j<=SolverTrees.returnLen();j++)
		resetFinByClusterName(SolverTrees.retrieve(j),inputInts[startI+2]);
	   break;
	}
	usedBifurs=false;	
        bifurAnswers[i-1]=inputInts[startI+i]+1;
        cout<<bifurAnswers[i-1]<<endl;
     }

     cout<<"Bifucations read, number="<<count<<endl;

     tag=0;
     for(i=1;i<=numTrees;i++)
     {
	cout<<"In Solve Forest Loop"<<endl;
        tag=solveForest(graph0, SolverTrees);
        if(tag==-2)
        {
          cout<<"Return by tag"<<endl; 
          setArraysForOutput(inputTheInts, inputDouble);
          return;
        }
     }

     cout<<"Solver ran, return value="<<tag<<endl;

     useFileToBifur=true;
     useFileTotal=false;     

     cout<<"usedBifurs="<<(usedBifurs ? "true" : "false")<<endl;

     if(usedBifurs && !autoSolve)
     { 
       saveState(graph0);
       inputInts[0]++;
       currPosI=inputInts[0]+2;
       currPosF=(int) inputFloats[0];
       inputInts[inputInts[0]-1]=0;

       int count=0, currPosBackUp=currPosI++;
       for(i=1;i<=numTrees;i++)
          outputFinState(SolverTrees.retrieve(i), currPosI, inputInts, count);
       inputInts[currPosBackUp]=count;

       for(i=1;i<=numTrees;i++)
       {
          selectBifurcation(graph0,SolverTrees.retrieve(i),useFileToBifur);
          useFileTotal=(useFileTotal || useFileToBifur);
          if(useFileToBifur)
          {
            inputInts[inputInts[0]-1]++;
            continue;
          } 
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));  
       }
       inputInts[inputInts[0]]=0;
       inputInts[inputInts[0]+1]=inputInts[inputInts[0]-1];
     }
     else if(!autoSolve)
     {
       tag=0;
       for(i=1;i<=numTrees;i++)
       {
          if(SolverTrees.retrieve(i).returnCurrBifur()>=0) continue;
          SolverTrees.retrieve(i).setCurrBifur(bifurAnswers[tag++]);
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));
       }
     }

     if(useFileTotal && !autoSolve) 
     {
       cout<<"Return by useFileTotal"<<endl;
       setArraysForOutput(inputTheInts, inputDouble);
       return;
     }

     if(autoSolve)
     {
       saveState(graph0);
       for(i=1;i<=numTrees;i++)
       {
          SolverTrees.retrieve(i).setCurrBifur(1);
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));
       }
     }

     saveState(graph0);
     currPosI=inputInts[0]++;
     inputInts[currPosI++]=1;
     currPosF=(int) inputFloats[0];
     inputInts[currPosI++]=1;
     inputInts[currPosI++]=numTrees;
     for(i=1;i<=numTrees;i++)
     {
	updateGraph(graph0, SolverTrees.retrieve(i));	
        inputInts[currPosI++]=SolverTrees.retrieve(i).returnName();
        generateOutputToArray(graph0, SolverTrees.retrieve(i), true);
     }
   }

   setArraysForOutput(inputTheInts, inputDouble);

   outputArrays(inputInts, inputFloats);

   cout<<"Final Bifurcation Return"<<endl;
   
   return;
}

