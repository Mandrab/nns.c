#include <math.h>

#include "tests.h"
#include "interface/mea.h"
#include "util/errors.h"

#define TOLERANCE 5

typedef struct
{
    int input_node;
    int output_node;
    int resistance;
} ior_t;

/**
 * Testing the following circuit:
 * 
 *      ---------------------
 *     |                    |
 *     |  *2   *1   *    *  |
 *     |                    |
 *     |  *    *    *    *  |
 *     |                    |
 *     |  *    *    *    *  |
 *     |                    |
 *     |  *    *    *    *1 |
 *     |                    |
 *     ----------------------
 *
 * The '*' are the electrodes, the numbers the quantity of neighbor nanowires.
 */
void test_mea_connection()
{
    wire Ws[5] = {
        (wire) { 67.0,  68.0 },
        (wire) { 65.0,  65.0 },     // electrode 0
        (wire) { 65.0,  189.0 },    // electrode 1
        (wire) { 437.0, 438.0 },    // electrode 16
        (wire) { 500.0, 500.0 },
    };

    const datasheet ds = { .wires_count = 5, .package_size = 500 };
    const network_topology nt = { Ws };

    const MEA mea = connect_MEA(ds, nt);

    for (int i = 0; i < MEA_ELECTRODES; i++)
    {
        assert(mea.Ps[i].x > 0, -1, "Unexpected value of 'mea.Ps[%d].x'. Expected > 0, Result %f", i, mea.Ps[i].x);
        assert(mea.Ps[i].y > 0, -1, "Unexpected value of 'mea.Ps[%d].y'. Expected > 0, Result %f", i, mea.Ps[i].y);

        assert(mea.Ps[i].x < 500, -1, "Unexpected value of 'mea.Ps[%d].x'. Expected < 500, Result %f", i, mea.Ps[i].x);
        assert(mea.Ps[i].y < 500, -1, "Unexpected value of 'mea.Ps[%d].y'. Expected < 500, Result %f", i, mea.Ps[i].y);

        char str[100];
        snprintf(str, 100, "mea.e2n[%d]", i);

        switch (i)
        {
            case 0:
                assert(mea.e2n[i] == 1, -1, INT_ERROR, str, 1, mea.e2n[i]);
                break;
            case 1:
                assert(mea.e2n[i] == 2, -1, INT_ERROR, str, 2, mea.e2n[i]);
                break;
            case 15:
                assert(mea.e2n[i] == 3, -1, INT_ERROR, str, 3, mea.e2n[i]);
                break;
            default:
                assert(mea.e2n[i] == -1, -1, INT_ERROR, str, -1, mea.e2n[i]);
        }
    }
}

int interface_mea()
{
    test_mea_connection();

    return 0;
}
