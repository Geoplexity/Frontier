FRONTIER
========
A geometric constraint solver that supports 3D feature based CAD and assembly.

NOTE: email sitharam@cise.ufl.edu to request any additional information.

Modifications and dates
-----------------------
The FRONTIER software is undergoing continuous transformation. Watch this space for version changes as well as pointers to changes and additions to documentation and publications.

Versions
--------
This repository has 3 versions of the Frontier software, each organized in a separate folder. 
1. FRONTIER 2D - This version has a working GUI and accepts 2D input. However, it has JNI issues and as such cannot
communicate with the backend solver. 
2. FRONTIER 3D and Frontier 3D with module rigidity - These versions allow 3D input but, they were written with an old version of Java 3D and hence certain libraries (javax/media/j3d/TransformGroup) don't seem to work. The only difference between these two version is that Frontier3D does not have module rigidity.

Currently, the frontier software consists of a backend solver written in C++ and GUI written in Java. JNI is used to connect the two. Since JNI is extremly platform sensitive, we recommend that a new GUI be written in C++ for the same (possibly using a cross platform framework like QT).


Installation (for unix):
------------------------
NOTE: Currently, to run FRONTIER, you need Maple V or a later version.

- From the root directory of any of the versions mentined above, type `./run` to start the FRONTIER geometric constraint solver.

Description of FRONTIER's overall strengths
-------------------------------------------
Its organization and components can be found in: "FRONTIER: fully enabling geometric constraints for feature based 3d design and assembly," J. Oung, M. Sitharam, B. Moro, A. Arbree, Proceedings of the ACM Solid Modeling symposium, 2001. [Link to paper](http://www.cise.ufl.edu/~sitharam/shortfrontier.ps) (Full paper in preparation -- watch this space).

See also [J-J. Oung's masters thesis](http://www.cise.ufl.edu/~joung).

Many of FRONTIER's strengths rely on  the degree-of-freedom-graph-based decomposition and recombination method called the Frontier Vertex Algorithm (FA) for geometric constraint systems.  This algorithms applies equally well to 3D, although the sketcher user interface  currently available with FRONTIER is restricted to 2d. FA's advantages, performance, construction and comparison with existing decomposition-recombination methods can be found in the following papers.

1. [Decomposition Plans for Geometric Constraint Systems, Part I: Performance Measures for CAD](http://www.idealibrary.com/links/toc/jsco/31/4/0); Christoph M. Hoffman, Andrew Lomonosov, Meera Sitharam. Journal of Symbolic computation, vol 31, issue 4, pp. 367-408.
2. [Decomposition Plans for Geometric Constraint Problems, Part II: New Algorithms](http://www.idealibrary.com/links/toc/jsco/31/4/0); Christoph M. Hoffman, Andrew Lomonosov, Meera Sitharam. Vol 31, issue 4, pp. 409-427.
3. Geometric constraint decomposition; C. Hoffman, M. Sitharam, A. Lomonosov. In "Geometric Constraint Solving and Applications", Springer Verlag, Edited by Beat Br\" uderlin and Dieter Roller, 1998.
4. Finding dense subgraphs of constraint graphs; M. Sitharam, C. Hoffman, A. Lomonosov. Constraint Programming '97, Lecture Notes in Computer Science 1330, G. Smolka Ed., Springer Verlag, pp. 463-478, 1997.
5. Planning Geometric constraint decompositions via graph transformations; C. Hoffman, A. Lomonosov, M. Sitharam, Proceedings of AGTIVE '99 (Graph Transformations with Industrial Relevance), Springer lecture notes, LNCS 1779, eds Nagl, Schurr, Munch, pp. 309-324, 1999.
6. FRONTIER, a 3d geometric constraint solver Part I: architecture, Sitharam, Oung, Arbree, Kohareswaran. Submitted.
7. FRONTIER, a 3d geometric constraint solver Part II: algorithms; Sitharam, Arbree, Zhou. Submitted.
8. [Solution space navigation for geometric constraint systems](https://www.cise.ufl.edu/~sitharam/pdfs/esm.pdf) (ACM Transactions on Graphics 2006)
9. [Combinatorial approahes to Geometri Constraint Solving:
Problems, Progress and Diretions](https://www.cise.ufl.edu/~sitharam/pdfs/dimacs.pdf)  (DIMACS survey 2006)


I/O specifications and further documentation
--------------------------------------------
For each of FRONTIER's four components:

- FUI sketcher (user interface, Java)
- UTU (main program in C++),
- D(ecomp)R(ecomb)-planner( Frontier vertex algorithm in C++),
- ESM (equation-solution-manager in C++)

can be found in the documentation folder within the FRONTIER-gnu directory as well as interspersed in the code.

The communication between the Java and C++ programs is achieved using the JavaNativeInterface so that they use the same frep datastructures without having to open and close files. The datastructures are explained in documentation/CCcodedocs/datastructures_overview.txt.

FRONTIER needs access to an algebraic/numeric solver, and currently it uses Maple V (or a later version).

### License ###
Frontier is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
The GNU GPL license can be found at [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).

### Contact ###
Meera Sitharam, CISE @ UF

