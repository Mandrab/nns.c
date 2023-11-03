#ifndef CONNECTION_H
#define CONNECTION_H

/// @brief Enumeration of possible connection states for a nanowire in a
/// Nanowire Network. A nanowire is always in one, and one only, of those
/// states.
typedef enum
{
    NONE,       ///< The nanowire is not connected with any external instrument
                ///< directly.
    SOURCE,     ///< The nanowire is connected to a voltage source.
    GROUND,     ///< The nanowire is grounded.
    LOAD        ///< The nanowire is connected to an external load.
} connection_t;

#endif /* CONNECTION_H */
