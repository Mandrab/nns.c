#include <float.h>
#include <math.h>
#include <string.h>

#include "interface/mea.h"
#include "util/tensors.h"

MEA connect_MEA(const datasheet ds, const network_topology nt)
{
    // count how many electrodes per side exist
    int n = sqrt(MEA_ELECTRODES);

    // define the distance between each electrode and consider a little padding
    double d = ds.package_size / n;

    // create the mea data-structure to store the electrodes positions
    MEA mea;

    // define electrodes positions
    for (int i = 0, k = 0; i < n; i++)
    {
        // the x is the padding from the border plus
        // the distance from the previous electrodes
        double x = d / 2 + d * i;

        for (int j = 0; j < n; j++, k++)
        {
            // the y is the padding from the border plus
            // the distance from the previous electrodes
            mea.Ps[k] = (point) { x, d / 2 + d * j };
        }
    }

    // set all the electrodes as unconnected
    memset(mea.e2n, 0xff, MEA_ELECTRODES * sizeof(int));
    memset(mea.ct,  NONE, MEA_ELECTRODES * sizeof(int));

    // map the electrodes index to the index of the
    // nearest nanowire (considering the centroid)
    #pragma omp parallel for
    for (int i = 0; i < MEA_ELECTRODES; i++)
    {
        double min_distance = DBL_MAX; 
        for (int j = 0; j < ds.wires_count; j++)
        {
            // calculate the electrode to nanowire-centroid distance
            double distance = squared_distance(mea.Ps[i], nt.Ws[j].centroid);

            // if a nearest nanowire has been found, memorize its index
            if (distance < min_distance && distance < MAX_DISTANCE)
            {
                mea.e2n[i] = j;
                min_distance = distance;
            }
        }
    }

    return mea;
}

interface mea2interface(const MEA mea)
{
    interface it = { };

    // determine the size of the arrays in the data-structure
    for (int i = 0; i < MEA_ELECTRODES; i++)
    {
        it.sources_count += mea.ct[i] == SOURCE;
        it.grounds_count += mea.ct[i] == GROUND;
        it.loads_count   += mea.ct[i] == LOAD;
    }

    // allocate the required memory
    it.sources_index = vector(int,    it.sources_count);
    it.grounds_index = vector(int,    it.grounds_count);
    it.loads_index   = vector(int,    it.loads_count);
    it.loads_weight  = vector(double, it.loads_count);

    // according to the state of each electrode, set the interface arrays
    for (int i = 0, s = 0, g = 0, l = 0; i < MEA_ELECTRODES; i++)
    {
        switch (mea.ct[i])
        {
            case SOURCE:
                it.sources_index[s++] = mea.e2n[i];
                break;
            case GROUND:
                it.grounds_index[g++] = mea.e2n[i];
                break;
            case LOAD:
                it.loads_index[l]     = mea.e2n[i];
                it.loads_weight[l++]  = mea.ws[i];
                break;
            default:
        }
    }

    return it;
}
