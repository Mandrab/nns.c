#include <math.h>

#include "tests.h"
#include "util/measures.h"
#include "util/errors.h"
#include "util/tensors.h"

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
 *     0 --- 1kΩ --- 1 --- 1kΩ --- 2
 *     |             |             |
 *    1kΩ           1kΩ           1kΩ
 *     |             |             |
 *     3     -- 1kΩ -4- 1kΩ --     5
 *     |    |                |
 *    1kΩ   6                7
 *     |    -- 1kΩ --- 1kΩ --
 *     8            |
 *                  9
 *
 * All the resistances are 1kΩ.
 */
void test_resistive_distance()
{
    double nsYs[10];
    double nsVs[10];
    int ccIs[10] = { 1, 3, 12, 14, 25, 38, 46, 47, 69, 79 };

    for (int i = 0; i < 10; i++)
    {
        nsYs[i] = 0.001;
    }

    datasheet ds = { 10 };
    network_state ns = { nsYs, nsVs };
    connected_component cc = { 10, 10, 0, 0, ccIs };

    // array of input/output nodes and their resistance
    ior_t iors[100] = {
        { 0, 0, 0    },
        { 0, 1, 1000 },
        { 0, 2, 2000 },
        { 0, 3, 1000 },
        { 0, 4, 2000 },
        { 0, 5, 3000 },
        { 0, 6, 2750 },
        { 0, 7, 2750 },
        { 0, 8, 2000 },
        { 0, 9, 3000 },

        { 1, 0, 1000 },
        { 1, 1, 0    },
        { 1, 2, 1000 },
        { 1, 3, 2000 },
        { 1, 4, 1000 },
        { 1, 5, 2000 },
        { 1, 6, 1750 },
        { 1, 7, 1750 },
        { 1, 8, 3000 },
        { 1, 9, 2000 },

        { 2, 0, 2000 },
        { 2, 1, 1000 },
        { 2, 2, 0    },
        { 2, 3, 3000 },
        { 2, 4, 2000 },
        { 2, 5, 1000 },
        { 2, 6, 2750 },
        { 2, 7, 2750 },
        { 2, 8, 4000 },
        { 2, 9, 3000 },

        { 3, 0, 1000 },
        { 3, 1, 2000 },
        { 3, 2, 3000 },
        { 3, 3, 0    },
        { 3, 4, 3000 },
        { 3, 5, 4000 },
        { 3, 6, 3750 },
        { 3, 7, 3750 },
        { 3, 8, 1000 },
        { 3, 9, 4000 },

        { 4, 0, 2000 },
        { 4, 1, 1000 },
        { 4, 2, 2000 },
        { 4, 3, 3000 },
        { 4, 4, 0    },
        { 4, 5, 3000 },
        { 4, 6, 750  },
        { 4, 7, 750  },
        { 4, 8, 4000 },
        { 4, 9, 1000 },

        { 5, 0, 3000 },
        { 5, 1, 2000 },
        { 5, 2, 1000 },
        { 5, 3, 4000 },
        { 5, 4, 3000 },
        { 5, 5, 0    },
        { 5, 6, 3750 },
        { 5, 7, 3750 },
        { 5, 8, 5000 },
        { 5, 9, 4000 },

        { 6, 0, 2750 },
        { 6, 1, 1750 },
        { 6, 2, 2750 },
        { 6, 3, 3750 },
        { 6, 4, 750  },
        { 6, 5, 3750 },
        { 6, 6, 0 },
        { 6, 7, 1000 },
        { 6, 8, 4750 },
        { 6, 9, 750  },

        { 7, 0, 2750 },
        { 7, 1, 1750 },
        { 7, 2, 2750 },
        { 7, 3, 3750 },
        { 7, 4, 750  },
        { 7, 5, 3750 },
        { 7, 6, 1000 },
        { 7, 7, 0 },
        { 7, 8, 4750 },
        { 7, 9, 750  },

        { 8, 0, 2000 },
        { 8, 1, 3000 },
        { 8, 2, 4000 },
        { 8, 3, 1000 },
        { 8, 4, 4000 },
        { 8, 5, 5000 },
        { 8, 6, 4750 },
        { 8, 7, 4750 },
        { 8, 8, 0    },
        { 8, 9, 5000 },

        { 9, 0, 3000 },
        { 9, 1, 2000 },
        { 9, 2, 3000 },
        { 9, 3, 4000 },
        { 9, 4, 1000 },
        { 9, 5, 4000 },
        { 9, 6, 750  },
        { 9, 7, 750  },
        { 9, 8, 5000 },
        { 9, 9, 0    }
    };

    for (int i = 0; i < 100; i++)
    {
        int a = iors[i].input_node;
        int b = iors[i].output_node;
        int r = iors[i].resistance;

        char str[100];
        snprintf(str, 100, "R_%d,%d", a, b);

        double result = resistive_distance(ds, ns, cc, a, b);
        assert(fabs(result - r) < TOLERANCE, -1, DOUBLE_ERROR, str, r, result);
    }
}

int util_measures()
{
    test_resistive_distance();

    return 0;
}
