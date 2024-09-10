#include "simulator.h"

void Simulate(AppState *state) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            // Test for probe
            if (state->grid[x][y].type == COMPONENT_PROBE) {
                FoundProbe(state, &x, &y);
                printf("X: %d\n", x);
            }
        }
    }
}

void FoundProbe(AppState *state, int *x, int *y) {
    // Check for component neighbour x+1
    *x = *x+1;
    switch (state->grid[*x][*y].type)
    {
    case COMPONENT_WIRE:
        if (state->grid[*x][*y].rotation == 0) {
            // Wire going from the probe
            CalculateCircuitPath(state, &x, &y);
            printf("We have a wire!\n");
        }
        break;
    
    case COMPONENT_WIRE_CORNER:
        if (state->grid[*x][*y].rotation == ROTATION_180 || state->grid[*x][*y].rotation == ROTATION_270) {
            // Wire corner going from the probe
            CalculateCircuitPath(state, &x, &y);
            printf("We have a wire corner!\n");
        }
        break;

    case COMPONENT_WIRE_INTERSECTION_3WAY:
        if (state->grid[*x][*y].rotation != ROTATION_90) {
            // Wire intersection 3 way going from the probe
            CalculateCircuitPath(state, &x, &y);
            printf("We have a wire intersection 3 way!\n");
        }
        break;

    case COMPONENT_WIRE_INTERSECTION_4WAY:
        // Wire intersection 4 way going from the probe
        CalculateCircuitPath(state, &x, &y);
        printf("We have a wire intersection 4 way!\n");

        break;
    
    default:
        printf("Not supported\n");
        *x = *x-1;
        break;
    }
};

 

/*
    Linked list



*/