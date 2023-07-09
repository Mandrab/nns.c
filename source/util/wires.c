#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "util/distributions.h"
#include "util/tensors.h"
#include "util/wires.h"

// create a structure used by "detect_junctions" function
struct node {
    junction value;
    struct node* tail;
};

wire* drop_wires(datasheet ds)
{
    // create the array of wires to fill
    wire* ws = vector(wire, ds.wires_count);

    // generate the distribution of the wires
    for (int i = 0; i < ds.wires_count; i++)
    {
        float length;

        // generate the (positive) length of the wire
        do
        {
            length = normal_random(ds.length_mean, ds.length_std_dev);
        } while (length <= 0);

        // generate the centroids of the ws
        float xc = (float) rand() / RAND_MAX * ds.package_size;
        float yc = (float) rand() / RAND_MAX * ds.package_size;
        float theta = (float) rand() / RAND_MAX * M_PI;

        // save the information into the data-structure
        ws[i] = (wire)
        {
            { xc, yc },     // centroid
            {               // start edge
                xc - length / 2.0 * cos(theta),
                yc - length / 2.0 * sin(theta)
            },
            {               // end edge
                xc + length / 2.0 * cos(theta),
                yc + length / 2.0 * sin(theta)
            },
            length          // length
        };
    }

    return ws;
}

void detect_junctions(
    datasheet ds, wire* ws,     // inputs
    junction* js, int* js_count // output
)
{
    // create a list to contain the discovered junctions
    struct node* head = NULL;

    // set the junctions counter to 0
    *js_count = 0;

    // iterate over all wire junctions
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < ds.wires_count; i++)
    {
        for (int j = i + 1; j < ds.wires_count; j++)
        {
            point si = ws[i].start_edge, ei = ws[i].end_edge;
            point sj = ws[j].start_edge, ej = ws[j].end_edge;

            // calculate the distance between the point edges on each axis
            float Δxi = si.x - ei.x, Δyi = si.y - ei.y;
            float Δxj = sj.x - ej.x, Δyj = sj.y - ej.y;

            float c = Δxi * Δyj - Δyi * Δxj;

            // if there is no intersection, check the next pair
            if (fabs(c) < 0.01)
            {
                continue;
            }

            float a = si.x * ei.y - si.y * ei.x;
            float b = sj.x * ej.y - sj.y * ej.x;

            // calculate the possible intersection point
            float x = (a * Δxj - b * Δxi) / c;
            float y = (a * Δyj - b * Δyi) / c;

            // exclude intersection points outside the wires area
            if (
                fmin(si.x, ei.x) <= x && x <= fmax(si.x, ei.x) &&
                fmin(sj.x, ej.x) <= x && x <= fmax(sj.x, ej.x) &&
                fmin(si.y, ei.y) <= y && y <= fmax(si.y, ei.y) &&
                fmin(sj.y, ej.y) <= y && y <= fmax(sj.y, ej.y)
            )
            {
                // create a list node and save the junction data in it
                struct node element = { { i, j, (point) { x, y } }, head };

                // set the element as the new head of the list
                head = &element;

                // increment the junctions counter
                *js_count++;
            }
        }
    }

    // create an array to contain the found junctions
    js = vector(junction, *js_count);

    // fill the junctions array
    for (int i = 0; i < *js_count; i++)
    {
        js[i] = (junction) {
            head->value.first_wire,
            head->value.second_wire,
            head->value.position
        };
        head = head->tail;
    }
}

bool** construe_adjacency_matrix(datasheet ds, network_topology nt)
{
    // create the adjacency matrix
    bool** adj = matrix(bool, ds.wires_count, ds.wires_count);

    // set an "adjacency" in presence of each junction
    for (int i = 0; i < nt.js_count; i++)
    {
        adj[nt.Js[i].first_wire][nt.Js[i].second_wire] = true;
    }

    return adj;
}
