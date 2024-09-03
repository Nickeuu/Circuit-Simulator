#include "action_controller.h"


// Function to handle the drawing action (placing a component on the grid)
void HandleDrawAction(AppState* state) {
    state->grid[state->previewX][state->previewY].type = state->dropdownBoxActive + 1;
    state->grid[state->previewX][state->previewY].rotation = state->componentRotation;
    state->grid[state->previewX][state->previewY].value = 0;
}

// Function to handle the delete action (removing a component from the grid)
void HandleDeleteAction(AppState* state) {
    state->grid[state->previewX][state->previewY].type = COMPONENT_EMPTY;
    state->grid[state->previewX][state->previewY].rotation = 0;
    state->grid[state->previewX][state->previewY].value = 0;
}

// Function to handle the edit action (editing the value of a component)
void HandleEditAction(AppState* state) {
    if (!state->isEditing && state->grid[state->previewX][state->previewY].type != COMPONENT_EMPTY) {
        state->isEditing = true;
        state->editX = state->previewX;
        state->editY = state->previewY;
        state->editValue = state->grid[state->editX][state->editY].value; // Load current value

        // Initialize text buffer with the current value
        snprintf(state->valueText, sizeof(state->valueText), "%d", state->grid[state->editX][state->editY].value);
    }
}