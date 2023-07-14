#ifndef DATASHEET_H
#define DATASHEET_H

/// Defines the static characteristics of the device.
typedef const struct
{
    int     wires_count;        ///< Total number of wires in the Nanowire Network.
    double  length_mean;        ///< Average length of the nano-wires in µm.
    double  length_std_dev;     ///< Standard deviation of the nano-wires length in µm.
    int     package_size;       ///< Length of one side of the device package in µm.
    int     generation_seed;    ///< Seed of the random number generator to create reproducible networks.
} datasheet;

#endif /* DATASHEET_H */
