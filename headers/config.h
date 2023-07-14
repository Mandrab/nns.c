#ifndef CONFIG_H
#define CONFIG_H

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

#endif /* CONFIG_H */
