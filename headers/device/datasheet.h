#ifndef DATASHEET_H
#define DATASHEET_H

/// @brief Defines the static characteristics of the device.
typedef struct
{
    int     wires_count;        ///< Total number of wires in the Nanowire
                                ///< Network.
    double  length_mean;        ///< Average length of the nano-wires in µm.
    double  length_std_dev;     ///< Standard deviation of the nano-wires
                                ///< length in µm.
    int     package_size;       ///< Length of one side of the device package
                                ///< in µm.
    int     generation_seed;    ///< Seed of the random number generator to
                                ///< create reproducible networks.
} datasheet;

/// @brief Compare two datasheets according to the number of nanowires, the
/// size of their package and their creation seed.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the datasheets contain the same data; > 0 if e1 has, in order,
/// more nanowires, a bigger size or a bigger seed than e2; < 0 otherwise.
int dscmp(const void* e1, const void* e2);

#endif /* DATASHEET_H */
