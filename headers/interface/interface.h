/**
 * @file interface.h
 * 
 * @brief Defines the structure of the nanowire-network device interface to the
 * external world.
 * 
 * This file defines the data structures and functions for interfacing with a
 * nanowire-network device. It includes functionality for comparing, copying,
 * and destroying interfaces.
 * 
 * The interface is represented by the `interface` structure, which holds
 * information about sources, grounds, loads and weights, and index of the
 * nanowire they are connected to. The provided functions allow for the
 * comparison of interfaces, creation of deep copies, and proper cleanup of
 * allocated resources.
 * 
 * @note The `interface` uses dynamically allocated memory that must be managed
 * using the provided functions.
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

#include "interface/connection.h"

/// @brief Interface between the nanowire-network device and external systems.
/// 
/// @details Example of the possible use of the interface.
/// ```
///             V
///             |
///          ---------------
///         |   |           |
///         |   *---*---*   |
///         |   |  /    |   |
///         |   | /     |   |
///         |   *===L===*   |
///         | NN        |   |
///          ---------------
///                     |
///                     G
/// ```
typedef struct
{
    int     sources_count;  ///< Number of sources connected to the device.
    int*    sources_index;  ///< Index of the input nanowires.

    int     grounds_count;  ///< Number of grounds connected to the device.
    int*    grounds_index;  ///< Index of the output nanowires.

    int     loads_count;    ///< Number of loads connected to the device.
    int*    loads_index;    ///< Index of the output nanowires.
    double* loads_weight;   ///< Weight of the optional load connected to a
                            ///< nanowire (in siemens).
} interface;

/// @brief Compare two network interfaces according to the number of sources,
/// grounds, and loads.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the datasheets contain the same data; > 0 if e1 has, in order,
/// more sources, grounds, or loads than e2; < 0 otherwise.
int itcmp(const void* e1, const void* e2);

/// @brief Create a deep copy of an interface.
/// 
/// @param[in] it The interface to copy.
/// @return The deep copy of the interface.
interface copy_interface(const interface it);

/// @brief Destroy an interface memorized in the stack. This function
/// practically frees the masks and weights of the interface. The arrays to be
/// freed must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] it The interface to destroy.
void destroy_interface(interface it);

#endif /* INTERFACE_H */
