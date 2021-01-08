# Sparse Matrix / Graph Library

this part of OJLIBS is originally designed for sparse matrix and graph in sparse format.

A graph contains several components:
- Vertices V: indexed by integer {0, 1, ..., nvert - 1}
- Edges E: E is subset of (V x V), each edge has a edge_id (eid) indexed by integer {0, 1, ..., esize - 1}
- Vertex Attributes: V -> VAttr_i
- Edge Attributes: E -> EAttr_j

G = (V, E) is represented by `concept sparse` Sp (`ojlibs::csr` or `ojlibs::lil`).
VAttr and EAttr are stored in `std::vector<Attr>`.

E and EAttr is separated due to flexibility.
we can create EAttr on the fly, and specify which EAttr for our algorithm to use without boilerplate when there are multiple attributes.

for example, when apply Dijkstra algorithm. Input contains G(Sp) and Weight(EAttr). Output is Dist(VAttr), Visit(VAttr).

there is also a `ojlibs::graph` combining Sp and EAttr. it can access edge with its attributes easily.
it contains only references, so it owns no state.

every type in this sparse library is white-box, feel free to break abstraction.
