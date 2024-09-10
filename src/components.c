#include "components.h"


// Function to initialize the grid with empty components
void InitComponentsGrid(Component grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y].type = COMPONENT_EMPTY;  // Set each grid cell to empty
            grid[x][y].resistance = 0;
            grid[x][y].rotation = 0;
        }
    }
}

// Function to rotate the currently selected component
void RotateComponent(AppState* state) {
    if (state->currentAction == ACTION_DRAW) {
        int componentType = state->grid[state->previewX][state->previewY].type - 1;

        // Check for components without rotation
        if (state->componentInfos[componentType].nrOfRotations == 0) return;

        // Check for components with only 1 rotation
        if (state->componentInfos[componentType].nrOfRotations == 1) {
            state->componentRotation += ROTATION_90;
            if (state->componentRotation >= ROTATION_180) state->componentRotation = 0;
        } else {
            // Components with all 3 rotations
            state->componentRotation += ROTATION_90;
            if (state->componentRotation >= ROTATION_360) state->componentRotation = 0;
        }
    }
}