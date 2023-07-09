#ifndef DATASHEET_H
#define DATASHEET_H

typedef const struct
{
    int     wires_count;
    double  length_mean;
    double  length_std_dev;
    int     package_size;
    int     generation_seed;
} datasheet;

#endif /* DATASHEET_H */
