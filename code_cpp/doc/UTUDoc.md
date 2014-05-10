General Overview/The operation of the UTU
=========================================

(Note:  In this document, I make note of various datastructures used in these components.  Since all three component use the same datastructures, I outlined the datastructures in a separate document.  Information about these structures can be found in the interface header file, mfa2Clas.h, and in the attached file
dataStructuresOverview.txt.)

(Definition:  I use the term DR-Dag in this document and others that follow.  DR-Dag refers to a linked list clusters.  The definition of the linked list can be found in mfa1List.h and the definition of cluster can be found in mfa2Clas.h)

The three components, UTU, DR-Planner, and ESM, work together to decomposed, solve, then recombine the constraint system drawn in the sketcher.

The data from the sketcher is recieved by the UTU.  When the UTU is called it will either recieve raw sketcher data, or a copy of a partially solved graph and DR-Dag that it began processing earlier in the form of two arrays, one integer and one double. In the case that the UTU recieves raw data, the UTU converts the data into a graph structure and a input DR-Dag.  The description of this input format can be found in the sketcherIO.pdf file.  If the UTU recieve arrays, its converts them back to a graph and a DR-Dag using the method described in dataStructuresOverview.txt.  When the UTU recieves raw data and has translated it into a input DR-Dag and graph, the UTU called that DR-Planner, which creates an output decomposition plan. It then takes this output plan and calls the ESM, which generates equation systems following the decomposition plan, and uses an off-the-shelf algebraic/numeric solver to obtain solutions for these systems. Finally, solver stores all the final solutions to the solved system back into output arrays (see the method in dataStructuresOverview.txt) returns these to the UTU which returns them to the sketcher.  These actions, other that the decoding of the input from the sketcher, are called normal operation.

When the UTU is called with paritally solved data, the UTU considers a flag to select a mode of operation. These modes are normal operation, change constraint value, add constraint, delete constraint, add decomposition tree, and add vertex and constraint.  For each of these, the UTU updates both the graph and DR-Dag datastructures as follows:

- normal solution: no change necessary
- change constraint value: value of the appropriate edge updated in the graph structure
- add constriant: add a constraint to the graph structure and reset the solved flags in the various clusters in the DR-Dag to resolve those clusters which could be affected by the added constraint
- delete constraint: same as add constraint except the constraint deleted from the graph
- add decomposition tree: the DR-Dag creates a new input DR-Tree incorporating the new partial decomposition
- add vertex/object: same as add constraint except that a new vertex can be added as well

After these changes have been made, the UTU proceeds through the normal operation with the updated input data.  For a description of how the UTU communicates this information with the sketcher see the sketcherIO.pdf.


This operation of the UTU is very simple.  The UTU is coded in one file, utuC.cpp, and consists almost completely of one switch statement, who key is the flag sent by the sketcher.  Different cases cover the different modes discussed above and direct the correct solution procedure.
