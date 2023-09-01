#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

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
    int     mask_size;      ///< The size of the nodes masks.

    int     sources_count;  ///< Number of sources connected to the device.
    bool*   sources_mask;   ///< Mask of the source nodes (TRUE = source).

    int     grounds_count;  ///< Number of grounds connected to the device.
    bool*   grounds_mask;   ///< Mask of the grounded nodes (TRUE = grounded).

    int     loads_count;    ///< Number of loads connected to the device.
    bool*   loads_mask;     ///< Mask of the loaded nodes (TRUE = loaded).
    double* loads_weight;   ///< Weight of the optional load connected to the node (in siemens).
} interface;

/// @brief Destroy an interface memorized in the stack. This function
/// practically frees the masks and weights of the interface. The arrays to be
/// freed must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] it The interface to destroy.
void destroy_stack_interface(interface it);

/// @brief Destroy an interface memorized in the heap. This function
/// practically frees the masks and weights of the interface. In addition it
/// frees also the interface pointer. The arrays to be freed must have been
/// allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] it The pointer to the interface to destroy.
void destroy_heap_interface(interface* it);

#endif /* INTERFACE_H */
