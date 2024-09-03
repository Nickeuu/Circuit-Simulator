#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "definitions.h"
#include "raylib.h"
#include "app_state.h"


void InitComponentsGrid(Component grid[GRID_WIDTH][GRID_HEIGHT]);
void RotateComponent(AppState* state);

#endif
