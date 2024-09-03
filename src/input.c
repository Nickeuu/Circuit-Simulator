// input.c
#include "input.h"

void HandleInput(AppState* state) {
    // Mouse
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        state->mousePosition = GetMousePosition();
        if (state->mousePosition.y >= UI_HEIGHT && state->uiState != UI_STATE_DROPDOWN_ACTIVE) {
            state->previewX = (int)(state->mousePosition.x / COMPONENT_SIZE);
            state->previewY = (int)((state->mousePosition.y - UI_HEIGHT) / COMPONENT_SIZE);
            if (state->previewX >= 0 && state->previewX < GRID_WIDTH && state->previewY >= 0 && state->previewY < GRID_HEIGHT) {
                state->isPreviewing = true;
            } else {
                state->isPreviewing = false;
            }
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (state->isPreviewing && state->uiState != UI_STATE_DROPDOWN_ACTIVE) {
            switch (state->currentAction) {
                case ACTION_NONE: break;
                case ACTION_DRAW: HandleDrawAction(state); break;
                case ACTION_DELETE: HandleDeleteAction(state); break;
                case ACTION_EDIT: HandleEditAction(state); break;
                case ACTION_SIMULATE: state->isSimulating = true; break;
            }
        }
        state->isPreviewing = false;
    }

    // Keyboard
    if (IsKeyReleased(KEY_R)) {
        RotateComponent(state);  // Rotate the component if 'R' is pressed
    }
}
