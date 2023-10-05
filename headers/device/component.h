#ifndef COMPONENT_H
#define COMPONENT_H

/// @brief Connected component of the Nanowire Network. It contains all the
/// information needed to identify the nanowires and junctions in it.
/// Additionally, it contains the index of the junctions in the CC-specific
/// adjacency matrix.
typedef struct
{
    int ws_count;           ///< Number of nanowires in the connected
                            ///< component.
    int js_count;           ///< Number of junctions in the connected
                            ///< component.
    int ws_skip;            ///< Start index of the CC sub-array in the ns
                            ///< arrays (for nanowires).
    int js_skip;            ///< Start index of the CC sub-array in the ns
                            ///< arrays (for junctions).
    int* Is;                ///< Linearized index of the junctions (i.e.,
                            ///< junction(i, j) = junction(i * size + j)) in
                            ///< the CC-specific adjacency matrix. Ordered to
                            ///< specify the position of the Ys(i, j) weights.
} connected_component;

/// @brief Compare two connected components according to their size and
/// potentially the number of junctions.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the connected components have the same number of wires and
/// junctions; > 0 if e1 has more wires (or in case of equality junctions) than
/// e2; < 0 otherwise.
int cccmp(const void* e1, const void* e2);

#endif /* COMPONENT_H */
