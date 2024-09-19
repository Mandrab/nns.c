/**
 * @file connection.h
 * 
 * @brief Defines the enumeration for the possible connection states of an
 * electrode to a specific nanowire in a Nanowire Network.
 * 
 * This header contains an enumeration that represents the different input/
 * output states a nanowire can be in. A nanowire can either be unconnected,
 * connected to a voltage source, grounded, or connected to an external load.
 */
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
