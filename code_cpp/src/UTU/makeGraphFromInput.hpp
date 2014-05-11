#include "../GDS/Graph.hpp"

#include <iostream>
#include <fstream>

int sketchInput(Graph &g, int &idx, jint *inputData, int &idxDbl, jdouble *dbleData) //from sketch to graph
{

    int i, j, shapeType, constraintType, numInvolved, eName, atPart[2];
    Vertex *vert;
    Edge new_edg;

    //part 1 shape info.
    //NumVert=0;  //# of shapes in sketch == # of vertices in graph
    //idx=1;
    //idxDbl=0;

    std::ofstream outf;
    outf.open("sketchInput.out");

    g.output(outf);

    while(inputData[idx] >= 0)
    {
        g.NumVert++;
        Vertex new_ver;
        shapeType=inputData[idx++];            // shapeType
        outf<<"shapeType="<<shapeType<<std::endl;

        new_ver.setType(shapeType);
        //      outf<<"singleVertex="<<singleVertex<<std::endl;
        if(singleVertex<inputData[idx])
            singleVertex=inputData[idx];
        outf<<"singleVertex="<<singleVertex<<std::endl;
        outf<<"shapeID="<<inputData[idx]<<std::endl;

        switch(shapeType)                      // get weigth based on shapeType
        {
            case 0:                             // point
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(2);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
                new_ver.notDefinedValue(2);   // Not Applicable
                new_ver.notDefinedValue(3);   // Not Applicable
                new_ver.notDefinedValue(4);   // Not Applicable
                new_ver.notDefinedValue(5);   // Not Applicable
                break;
            case 1:                             // line
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(2);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
                new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
                new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
                idxDbl++;
                new_ver.notDefinedValue(5);   // Not Applicable
                break;
            case 2:                             // ray
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(3);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
                new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
                new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
                idxDbl++;
                new_ver.notDefinedValue(5);   // Not Applicable
                break;
            case 3:                             // line segment
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(4);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x1
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y1
                new_ver.initialValue(2, dbleData[idxDbl++], 0); //x2
                new_ver.initialValue(3, dbleData[idxDbl++], 0); //y2
                idxDbl++;

                /*               if(inputData[idx]>=0) {
                new_ver.initialValue(5, dbleData[idxDbl++], -1); //length
                new_ver.incrsWt(-1);
                }*/
                break;
            case 4:                             // circle
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(3);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
                /*             if(inputData[idx]>=0) {
                new_ver.initialValue(2, dbleData[idxDbl++], -1); // radius
                new_ver.incrsWt(-1);
                } */
                idxDbl++;
                new_ver.notDefinedValue(3);   // Not Applicable
                new_ver.notDefinedValue(4);   // Not Applicable
                new_ver.notDefinedValue(5);   // Not Applicable
                break;
            case 5:                             // arc
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(5);
                new_ver.initialValue(0, (float)inputData[idx++], 0); //x
                new_ver.initialValue(1, (float)inputData[idx++], 0); //y
                if(inputData[idx]>=0) {
                    new_ver.initialValue(2, dbleData[idxDbl++], -1); // radius
                    new_ver.incrsWt(-1);
                }
                idx=idx+2;  // float is 64-bit
                if(inputData[idx]>=0) {
                    new_ver.initialValue(3, dbleData[idxDbl++], -1); // angle
                    new_ver.incrsWt(-1);
                }
                break;
            case 6:                //point3D
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(3);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
                new_ver.initialValue(2, dbleData[idxDbl++], 0); //z
                new_ver.notDefinedValue(3);   // Not Applicable
                new_ver.notDefinedValue(4);   // Not Applicable
                new_ver.notDefinedValue(5);   // Not Applicable
                break;
            case 7:                            //point3D
                outf<<"shapeID="<<inputData[idx]<<std::endl;
                new_ver.setName(inputData[idx++]);     // shape ID  == vertex name
                new_ver.setWeight(6);
                new_ver.initialValue(0, dbleData[idxDbl++], 0); //x
                new_ver.initialValue(1, dbleData[idxDbl++], 0); //y
                new_ver.initialValue(2, dbleData[idxDbl++], 0); //z
                new_ver.initialValue(3, dbleData[idxDbl++], 0); //c
                new_ver.initialValue(4, dbleData[idxDbl++], 0); //d
                new_ver.initialValue(5, dbleData[idxDbl++], 0); //e
                break;

        };
        //if(inputData[idx]!=-1) idx++;
        g.vertices.append(new_ver);
    }
    outf<< "Vertices Number is: " << g.NumVert<<std::endl;
    outf<<inputData[idx]<<std::endl;
    outf << "Start to do part 2" << std::endl;
    idx++;

    int highEdgeCount=0, temp;

    highEdgeCount=0;

    //part 2 constraint info.
    while(inputData[idx]>=0)
    {
        g.NumEdge++;
        constraintType=inputData[idx++];      // constraint type
        outf<<"constraintType="<<constraintType<<std::endl;


        if(constraintType==7)
        {//imaginary object???
            idx++;
            idx++;

            int name1, name2, name3;

            int temp;

            name1=inputData[idx++];
            idx++;

            name2=inputData[idx++];
            idx++;

            name3=inputData[idx++];
            idx++;

            //Check whether the imaginary object exists.
            Vertex temp1, temp2, currVert;
            Edge currEdge1, currEdge2;
            for(i=1;i<=g.NumVert;i++)
            {
                currVert=g.vertices.retrieve(i);
                if (currVert.returnType()==7)   //imaginary object
                {
                    currEdge1=g.returnEdgeByEnds(name1, currVert.returnName());
                    currEdge2=g.returnEdgeByEnds(name2, currVert.returnName());
                    //this imaginary object exists
                    if (currEdge1.returnName() && currEdge1.returnType()==1 && currEdge1.returnWeight()==3
                        && currEdge2.returnName() && currEdge2.returnType()==1 && currEdge2.returnWeight()==3)
                            temp1 = currVert;

                    currEdge1=g.returnEdgeByEnds(name3, currVert.returnName());
                    currEdge2=g.returnEdgeByEnds(name2, currVert.returnName());
                    //this imaginary object exists
                    if (currEdge1.returnName() && currEdge1.returnType()==1 && currEdge1.returnWeight()==3
                        && currEdge2.returnName() && currEdge2.returnType()==1 && currEdge2.returnWeight()==3)
                            temp2 = currVert;
                }
            }

            if (g.hasVert(temp1.returnName()))
                g.delVerByName(temp1.returnName());
            else
            {
                temp1.setType(7);
                temp1.setName(++singleVertex);
                temp1.setWeight(6);
                temp1.setIgnore(true);
            }

            if (g.hasVert(temp2.returnName()))
                g.delVerByName(temp2.returnName());
            else
            {
                temp2.setType(7);
                temp2.setName(++singleVertex);
                temp2.setWeight(6);
                temp2.setIgnore(true);
            }

            outf<<"name1 = "<<name1<<";   name2 = "<<name2<<";   name3 = "<<name3<<std::endl;
            Edge tempE1, tempE2, tempE3, tempE4, tempE5;
            int tempName;

            tempE1.setName(1000+highEdgeCount++);
            tempE1.setEnd(0, temp1.returnName());
            tempE1.setEnd(1, temp2.returnName());
            tempE1.setType(8);
            tempE1.setWeight(1);
            tempE1.setValue(dbleData[idxDbl++]);

            temp1.appendIncid(tempE1.returnName());
            temp2.appendIncid(tempE1.returnName());

            currEdge1 = g.returnEdgeByEnds(name1, temp1.returnName());
            if (currEdge1.returnName())
            {
                tempE2 = currEdge1;
                g.delEdgeByName(currEdge1.returnName());
            }
            else
            {
                tempE2.setName(1000+highEdgeCount++);
                tempName=tempE2.returnName();
                tempE2.setEnd(0, name1);
                tempE2.setPart(0, 1);
                tempE2.setEnd(1, temp1.returnName());
                tempE2.setPart(1, 1);
                tempE2.setType(1);
                tempE2.setWeight(3);
                temp1.appendIncid(tempName);
                for(j=1;j<=g.NumVert;j++)
                {
                    vert=g.vertices.addrByIndex(j);
                    if(vert->returnName()==name1)
                        vert->appendIncid(tempName);
                }
            }

            currEdge1 = g.returnEdgeByEnds(name2, temp1.returnName());
            if (currEdge1.returnName())
            {
                tempE3 = currEdge1;
                g.delEdgeByName(currEdge1.returnName());
            }
            else
            {
                tempE3.setName(1000+highEdgeCount++);
                tempName=tempE3.returnName();
                tempE3.setEnd(0, name2);
                tempE3.setPart(0, 1);
                tempE3.setEnd(1, temp1.returnName());
                tempE3.setPart(1, 2);
                tempE3.setType(1);
                tempE3.setWeight(3);
                temp1.appendIncid(tempName);
                for(j=1;j<=g.NumVert;j++)
                {
                    vert=g.vertices.addrByIndex(j);
                    if(vert->returnName()==name2)
                        vert->appendIncid(tempName);
                }
            }

            currEdge1 = g.returnEdgeByEnds(name2, temp2.returnName());
            if (currEdge1.returnName())
            {
                tempE4 = currEdge1;
                g.delEdgeByName(currEdge1.returnName());
            }
            else
            {
                tempE4.setName(1000+highEdgeCount++);
                tempName=tempE4.returnName();
                tempE4.setEnd(0, name2);
                tempE4.setPart(0, 1);
                tempE4.setEnd(1, temp2.returnName());
                tempE4.setPart(1, 2);
                tempE4.setType(1);
                tempE4.setWeight(3);
                temp2.appendIncid(tempName);
                for(j=1;j<=g.NumVert;j++)
                {
                    vert=g.vertices.addrByIndex(j);
                    if(vert->returnName()==name2)
                        vert->appendIncid(tempName);
                }
            }

            currEdge1 = g.returnEdgeByEnds(name3, temp2.returnName());
            if (currEdge1.returnName())
            {
                tempE5 = currEdge1;
                g.delEdgeByName(currEdge1.returnName());
            }
            else
            {
                tempE5.setName(1000+highEdgeCount++);
                tempName=tempE5.returnName();
                tempE5.setEnd(0, name3);
                tempE5.setPart(0, 1);
                tempE5.setEnd(1, temp2.returnName());
                tempE5.setPart(1, 1);
                tempE5.setType(1);
                tempE5.setWeight(3);
                temp2.appendIncid(tempName);
                for(j=1;j<=g.NumVert;j++)
                {
                    vert=g.vertices.addrByIndex(j);
                    if(vert->returnName()==name3)
                        vert->appendIncid(tempName);
                }
            }

            g.vertices.append(temp1);
            g.vertices.append(temp2);
            g.edges.append(tempE1);
            g.edges.append(tempE2);
            g.edges.append(tempE3);
            g.edges.append(tempE4);
            g.edges.append(tempE5);
            g.NumVert+=2;
            g.NumEdge+=4;
            g.output(outf);
            continue;
        }//if (constraintType = 7)


        new_edg.setType(constraintType);
        new_edg.setWeight(1);                 // weight = 1 for most constraints
        eName=inputData[idx++];               // constraint ID  == edge name
        outf<<"constraintID="<<eName<<std::endl;

        if(nextEdgeName<=eName)
            nextEdgeName=eName+1;
        new_edg.setName(eName);
        numInvolved=inputData[idx++];         // # of Shapes involved==2 for now
        for(i=0;i<numInvolved;i++)
        {
            int endID;

            //out<<"   involved="<<inputData[idx]<<std::endl;
            endID=inputData[idx];
            new_edg.setEnd(i, endID);
            for(j=1;j<=g.NumVert;j++)
            {
                vert=g.vertices.addrByIndex(j);
                if(vert->returnName()==inputData[idx])
                    vert->appendIncid(eName);
            }
            idx++;

            int temp, vertType;

            if(constraintType==0||constraintType==1||constraintType==4||constraintType==6) {//dista
                atPart[i]=inputData[idx];
                //out<<"   part="<<inputData[idx]<<std::endl;
                temp=inputData[idx++];

                outf<<"Constraint Type="<<constraintType<<" part="<<temp<<std::endl;

                vertType=g.returnVertByName(endID).returnType();

                if(vertType==4 && temp==1)
                    temp=3;

                new_edg.setPart(i, temp);
            }
            else
            new_edg.setPart(i, 0);
        }//     for(i=0;i<numInvolved;i++)

        if(constraintType==1) { //incidence
            if (atPart[0]==0 && atPart[1]==0)                 new_edg.setWeight(0);
            if (atPart[0]>0  && atPart[0]<4  && atPart[1]==0) new_edg.setWeight(1);
            if (atPart[1]>0  && atPart[1]<4  && atPart[0]==0) new_edg.setWeight(1);
            if (atPart[0]!=0 && atPart[1]!=0)                 new_edg.setWeight(2);
        }

        if(constraintType==8) new_edg.setWeight(2);

        if(constraintType==0||constraintType==4||constraintType==6||constraintType==8)
        { // distance (2 or 3D) or angle
            new_edg.setValue(dbleData[idxDbl++]);//-1.0 for arbitrary
            //new_edg.setValue(toFloat(inputData[idx]));//-1.0 for arbitrary
            //idx=idx+2;
        }
        else
            new_edg.setValue(-2.0);                 // N/A for other constraints
        g.edges.append(new_edg);
    }//while(inputData[idx]>=0)
    idx++;
    //out<<"in input, idx="<<idx<<" inputData[idx]="<<inputData[idx]<<std::endl;
    nextVerName=singleVertex+4;
    if(g.vertices.retrieve(1).returnType()>5)
        g.dimension=3;
    else
        g.dimension=2;

    g.output(outf);
    outf.close();
    return idx;
}
