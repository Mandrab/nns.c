#ifndef CONFIG_H
#define CONFIG_H

/* SIMULATION INFORMATION */

// minimum and maximum conductance of the network
#define Y_MIN 0.001
#define Y_MAX 0.1

// standard update time of the simulator
// NOTE: changing it requires changing all the subsequent constants
#define TAU 0.05

#define ETA_P 10
#define ETA_D 1
#define KP 0.0001
#define KD 0.5

/* IO INFORMATION */

#define NETWORK_FILE_NAME_FORMAT "nanowire_network.%d.dat"
#define STATE_FILE_NAME_FORMAT "network_state.%d.dat"
#define INTERFACE_FILE_NAME_FORMAT "interface.%d.dat"

#endif /* CONFIG_H */
