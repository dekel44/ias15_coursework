#include <stdlib.h>
#include "rebound.h"

struct reb_simulation* sim;

// Initialize the IAS15 simulation with three bodies in a figure-8 configuration
void initialize_simulation() {
    sim = reb_simulation_create();
    sim->integrator = REB_INTEGRATOR_IAS15;
    sim->dt = 1e-3;
    sim->ri_ias15.min_dt = 1e-30;

    struct reb_particle p1 = {
        .m  = 1.0,
        .x  = -0.97000436, .y  =  0.24308753, .z  = 0,
        .vx =  0.466203685, .vy =  0.43236573, .vz = 0
    };
    struct reb_particle p2 = {
        .m  = 1.0,
        .x  =  0.97000436, .y  = -0.24308753, .z  = 0,
        .vx =  0.466203685, .vy =  0.43236573, .vz = 0
    };
    struct reb_particle p3 = {
        .m  = 1.0,
        .x  =  0.0,         .y  =  0.0,         .z  = 0,
        .vx = -0.93240737, .vy = -0.86473146, .vz = 0
    };

    reb_simulation_add(sim, p1);
    reb_simulation_add(sim, p2);
    reb_simulation_add(sim, p3);

    reb_simulation_update_tree(sim);
}

// Advance the simulation by dt
void step_simulation(double dt) {
    reb_simulation_integrate(sim, sim->t + dt);
}

// Return pointer to static array: [x1,y1,z1, x2,y2,z2, x3,y3,z3]
double* get_positions() {
    static double positions[9];
    for (int i = 0; i < 3; i++) {
        positions[i*3 + 0] = sim->particles[i].x;
        positions[i*3 + 1] = sim->particles[i].y;
        positions[i*3 + 2] = sim->particles[i].z;
    }
    return positions;
}

// Overwrite body i’s parameters and refresh the tree
void set_body(int i,
              double m,  double x,  double y,  double z,
              double vx, double vy, double vz) {
    if (i < 0 || i >= sim->N) return;
    sim->particles[i].m  = m;
    sim->particles[i].x  = x;
    sim->particles[i].y  = y;
    sim->particles[i].z  = z;
    sim->particles[i].vx = vx;
    sim->particles[i].vy = vy;
    sim->particles[i].vz = vz;
    reb_simulation_update_tree(sim);
}

#ifdef TEST_NATIVE
#include <stdio.h>
int main(void) {
    initialize_simulation();
    for (int i = 0; i < 5; i++) {
        step_simulation(0.01);
        double* pos = get_positions();
        printf("Step %d: Body0 = (%.3e, %.3e, %.3e)\n",
               i, pos[0], pos[1], pos[2]);
    }
    return 0;
}
#endif
