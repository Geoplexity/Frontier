# HyperGraph manipulator/event package
This package provides an abstract interface for manipulating hypergraphs
(e.g. graphs where edges can connect 0-inf nodes, instead of the traditional 2).

This is useful in practice for storing information about a "sketch" with geometric entities as nodes and constraints 
between one or more of them as the hyperedges, but the interface allows other applications.

The core types passed around using the interface are `HyperNode` and `HyperEdge`. Smart pointers to these types are 
associated with a given `HyperGraph`. The storage used under the hood may vary, but the only current implementation 
is `SparseHyperGraph`, which maintains the data structure using sets and pointers (derivations of `HyperEdge` with 
pointers to nodes).

Event handling may be added using  the `EventDispatchingHyperGraph` decorator, which delegates to a given 
implementation, logging changes in the graph data structure.

`AnnotatedHyperGraph` may be used when it is necessary to associate the edge and node classes with external data 
(such as shapes etc.). A specific graph implementation may be used or the `::create` method can be used to generate
a ready-to-use `AnnotatedHyperGraph`.