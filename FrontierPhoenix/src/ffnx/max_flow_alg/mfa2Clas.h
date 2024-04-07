//file: mfa2Clas.h
/* header file that contains the interface for the datastructures
used by the UTU, ESM and DR-planner*/

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

#ifndef MFA_CLASS_H
#define MFA_CLASS_H

// numerical recipies p. 279
#include <math.h>

#include <iostream>

#include "mfa1List.h"
#include "entities/vertex.h"
#include "entities/edge.h"
#include "entities/graph.h"

namespace ffnx::mfa {


//class that stores one set of degvalues exactly as described in Class Vertex above
    class DegValues {

    private:

        float degValues[8];

    public:

        DegValues() {
            int i;

            for (i = 0; i < 8; i++) degValues[i] = 0;
        }

        void setValue(int i, float value) { degValues[i] = value; }

        float returnValue(int i) { return degValues[i]; }

        DegValues &operator=(const DegValues &deg);
    };

    DegValues &DegValues::operator=(const DegValues &deg) {
        int i;

        if (this != &deg) {
            for (i = 0; i < 8; i++)
                degValues[i] = deg.degValues[i];

        }
        return *this;
    }

    class Cluster {
    private:
        Vertex core;           // core vertex formed by the reduction of all the interior vertices in the cluster
        int group;             // group ID is specified by user
        List<int> frontiers;   // list of the frontier edges in the cluster by ID
        List<Edge> innerE;     // list of edges between core and frontiers
        List<Edge> outerE;     // list of edges between frontiers
        List<int> originalV;   // list of vertices corresponding to original graph
        List<std::string> bifurs;   // list of the strings representing the bifurcations of this cluster
        int constrain;         // 1--well constrained, 2,3,...--over constrained
        int numBifurs;         // lenght of the list bifurs
        int currBifur;         // the currently selected bifurcation, -1 means no selection
        bool solved;           // true if the cluster has been solved before
        int currDeg;           // the index of the currect set of DegValues;
        List<DegValues> values;// same format and use as in Vertex

    public:
        List<Cluster> children;    // list of children

        //Default constructor sets all values to default, note that the default value of the degValues[2] is 1
        //since a zero rotation has a cosine of 1
        Cluster() {
            int i, j;
            DegValues first;

            first.setValue(2, 1.0);
            group = 0;
            constrain = 0;
            currBifur = -1;
            numBifurs = 0;
            solved = false;
            currDeg = 1;
            values.append(first);

        }

        //intializes a cluster in group g
        Cluster(int g) { group = g; }

        //copy constructor
        Cluster(const Cluster &);

        void setName(int n) { core.setName(n); }

        void setGroup(int g) { group = g; }

        void setBifurs(List<std::string> &theBifurs) {
            bifurs = theBifurs;
            numBifurs = theBifurs.returnLen();
        }

        void setCurrBifur(int theNum) { currBifur = theNum; }

        void setCore(Vertex &c) { core = c; }

        void setConst(int c) { constrain = c; }

        void setValue(int index, float value) { values.retrieve(currDeg).setValue(index, value); }

        void setSolved(bool sol) { solved = sol; }

        void setCurrDeg(int i) { currDeg = i; }

        void setDegValues(List<DegValues> theValues) { values = theValues; }

        //adds a new set of degvalues to this cluster in the case that this cluster has multiple parents
        void setToCluster(Cluster &theCluster) {
            int i, length, name;
            bool found;
            DegValues theNew;

            length = values.returnLen();
            name = theCluster.returnName();

            if (length == 1 && values.retrieve(1).returnValue(7) == 0) {
                currDeg = 1;
                values.retrieve(currDeg).setValue(7, name);
            } else {
                found = false;
                for (i = 1; i <= length; i++)
                    if (values.retrieve(i).returnValue(7) == name) {
                        found = true;
                        currDeg = 1;
                    }
                if (!found) {
                    theNew.setValue(2, 1.0);
                    values.append(theNew);
                    currDeg = length + 1;
                    values.retrieve(currDeg).setValue(7, name);
                }
            }
        }

        //creates a new cluster
        void formCl(Vertex &, List<int> &, List<Edge> &, List<Edge> &, List<int> &);

        void addOrig(int v) { if (!originalV.hasElem(v)) originalV.append(v); }

        void addFrot(int v) { if (!frontiers.hasElem(v)) frontiers.append(v); }

        void deleteOrig(int ver) { originalV.deleteElem(ver); }

        void deleteFrot(int ver) { frontiers.deleteElem(ver); }

        void makeEmpty() {
            int i, j;
            DegValues first;

            core.setName(0);
            group = 0;
            frontiers.makeEmpty();
            innerE.makeEmpty();
            outerE.makeEmpty();
            originalV.makeEmpty();
            children.makeEmpty();
            bifurs.makeEmpty();
            currBifur = -1;
            numBifurs = 0;
            constrain = 0;
            solved = false;
            values.makeEmpty();
            currDeg = 1;
            first.setValue(2, 1.0);
            values.append(first);
        }

        Vertex &returnCore() { return core; }

        int returnName() { return core.returnName(); }

        int returnGroup() { return group; }

        int returnConst() { return constrain; }

        int returnCurrDeg() { return currDeg; }

        int returnNumDeg() { return values.returnLen(); }

        float returnValue(int index) { return values.retrieve(currDeg).returnValue(index); }

        int returnNumBifurs() { return numBifurs; }

        int returnCurrBifur() { return currBifur; }

        std::string returnCurrBifurString() { return bifurs.retrieve(currBifur); }

        List<std::string> &returnBifurs() { return bifurs; }

        List<int> &returnFrontiers() { return frontiers; }

        int returnFronLen() { return frontiers.returnLen(); }

        int returnFrontier(int index) { return frontiers.retrieve(index); }

        List<Edge> &returnInnerE() { return innerE; }

        int returnInnerELen() { return innerE.returnLen(); }

        Edge &returnInnerE(int index) { return innerE.retrieve(index); }

        List<Edge> &returnOuterE() { return outerE; }

        int returnOuterELen() { return outerE.returnLen(); }

        Edge &returnOuterE(int index) { return outerE.retrieve(index); }

        List<int> &returnOrig() { return originalV; }

        int returnOrigV(int index) { return originalV.retrieve(index); }

        int returnOrigLen() { return originalV.returnLen(); }

        bool isSolved() { return solved; }


        bool empty() {
            if (core.returnName() == 0 && group == 0 && children.empty())
                return true;
            else return false;
        }

        //clusters are equal if they have the same name and are in the same group
        inline bool operator==(const Cluster &clu) {
            if (core == clu.core && group == clu.group) return true;
            else return false;
        }

        // = operator
        Cluster &operator=(const Cluster &clu);

        // a cluster is a sub cluster of another if all of its original vertices are original vertices in the
        // larger cluster
        bool operator<=(Cluster &cl0)  // subset operator
        {
            int i, nFront, nOrig0;
            List<int> origV0;

            nFront = frontiers.returnLen();
            origV0 = cl0.returnOrig();
            nOrig0 = origV0.returnLen();
            if (nFront > nOrig0 || nFront < 2) return false;

            for (i = 1; i <= nFront; i++) {
                if (!origV0.hasElem(frontiers.retrieve(i))) return false;
            }
            return true;
        }

        // stream output
        void output(std::ostream &);
    };


    Cluster::Cluster(const Cluster &clu) {
        int i;

        core = clu.core;
        group = clu.group;
        frontiers = clu.frontiers;
        innerE = clu.innerE;
        outerE = clu.outerE;
        originalV = clu.originalV;
        children = clu.children;
        bifurs = clu.bifurs;
        currBifur = clu.currBifur;
        numBifurs = clu.numBifurs;
        constrain = clu.constrain;
        solved = clu.solved;
        values = clu.values;
    }

    void Cluster::formCl(Vertex &c, List<int> &fV, List<Edge> &iE,
                         List<Edge> &oE, List<int> &o) {
        core = c;
        frontiers = fV;
        innerE = iE;
        outerE = oE;
        originalV = o;
    }

    Cluster &Cluster::operator=(const Cluster &clu) {
        int i;

        if (this != &clu) {
            core = clu.core;
            group = clu.group;
            frontiers = clu.frontiers;
            innerE = clu.innerE;
            outerE = clu.outerE;
            originalV = clu.originalV;
            children = clu.children;
            bifurs = clu.bifurs;
            currBifur = clu.currBifur;
            numBifurs = clu.numBifurs;
            constrain = clu.constrain;
            solved = clu.solved;
            values = clu.values;
        }
        return *this;
    }


    void Cluster::output(std::ostream &os) {
        int i, len, child;
        os << "Cluster(core):" << core.returnName()
           << ", depth=" << core.returnDepth();
        if (constrain == 1) os << ", well constrained";
        if (constrain > 1) os << ", over constrained";
        if (constrain < 1) os << ", under constrained";
        os << std::endl << "Frontiers: ";
        len = frontiers.returnLen();
        for (i = 1; i <= len; i++)
            os << frontiers.retrieve(i) << ' ';

        os << std::endl << "Previous clusters: ";
        len = children.returnLen();
        for (i = 1; i <= len; i++) {
            child = children.retrieve(i).returnCore().returnName();
            if (child != 0)
                os << child << ' ';
            else
                os << "Grp" << children.retrieve(i).returnGroup() << ' ';
        }

        os << std::endl << "Original Vertices: ";
        len = originalV.returnLen();
        for (i = 1; i <= len; i++)
            os << originalV.retrieve(i) << ' ';

        os << std::endl << "Inner Edge: ";
        len = innerE.returnLen();
        for (i = 1; i <= len; i++)
            os << innerE.retrieve(i).returnName() << '('
               << innerE.retrieve(i).returnWeight() << ','
               << innerE.retrieve(i).returnEnd1() << '-'
               << innerE.retrieve(i).returnEnd2() << ") ";

        os << std::endl << "Onter Edge: ";
        len = outerE.returnLen();
        for (i = 1; i <= len; i++)
            os << outerE.retrieve(i).returnName() << '('
               << outerE.retrieve(i).returnWeight() << ','
               << outerE.retrieve(i).returnEnd1() << '-'
               << outerE.retrieve(i).returnEnd2() << ") ";

        os << std::endl << "Bifurcations (Current Selection: ";
        if (currBifur < 1) os << "None):" << std::endl;
        else os << currBifur << "):" << std::endl;
        len = bifurs.returnLen();
        for (i = 1; i <= len; i++)
            os << "    " << bifurs.retrieve(i) << std::endl;

        os << std::endl;
    }


    class StaticVariables {
    public:
        int singleVertex = 1;
        int nextVerName = 1;
        int nextEdgeName = 1;
    };
}

#endif
