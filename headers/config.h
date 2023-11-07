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

/* INTERFACE INFORMATION */

#define MEA_ELECTRODES 16
#define MAX_DISTANCE 225     // 15 µm ^ 2 (simplifies calculation)

/* IO INFORMATION */

#define DIRECTORY_FORMAT            "%s/device_%d"
#define NETWORK_FILE_NAME_FORMAT    "%s/device_%d/nn.nns"
#define STATE_FILE_NAME_FORMAT      "%s/device_%d/ns_%d.nns"
#define INTERFACE_FILE_NAME_FORMAT  "%s/device_%d/it_%d.nns"
#define MEA_FILE_NAME_FORMAT        "%s/device_%d/mea_%d.nns"
#define COMPONENT_FILE_NAME_FORMAT  "%s/device_%d/cc_%d.nns"

#endif /* CONFIG_H */
