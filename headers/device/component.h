/**
 * @file component.h
 * 
 * @brief Defines the structure and operations for connected components in a
 * Nanowire Network.
 * 
 * This file contains the definition of the `connected_component` struct, which
 * represents a connected component of the Nanowire Network and identify its
 * belonging nanowires and junctions.
 * Additionally, it provides functions to compare, copy, and destroy connected
 * components.
 * 
 * @note The `connected_component` uses dynamically allocated memory that must be 
 * managed using the provided functions.
 */
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

/// @brief Create a deep copy of a connected component.
/// 
/// @param[in] cc The connected component to copy.
/// @return The deep copy of the connected component.
connected_component copy_component(const connected_component cc);

/// @brief Destroy a connected component by freeing its pointers (i.e., Is).
/// The array to be freed must have been allocated in the heap.
/// 
/// @param[in, out] cc The connected component to destroy.
void destroy_component(connected_component cc);

#endif /* COMPONENT_H */
