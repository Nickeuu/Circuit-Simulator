#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "definitions.h"
#include "app_state.h"
#include <stdio.h>

void Simulate(AppState* state);
void FoundProbe(AppState *state, int *x, int *y);
void CalculateCircuitPath (AppState *state, int *x, int *y);

#endif