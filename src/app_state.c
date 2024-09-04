#include <stdlib.h>
#include "app_state.h"
#include "graphics.h"
#include "input.h"
#include "resources.h"
#include "components.h"

AppState* InitializeAppState(int screenWidth, int screenHeight) {
    AppState* state = (AppState*)malloc(sizeof(AppState));
    if (!state) return NULL;

    state->uiLocation = (Rectangle){0, 0, SCREEN_WIDTH, UI_HEIGHT};
    state->isPreviewing = false;
    state->isEditing = false;
    state->previewX = -1;
    state->previewY = -1;
    state->dropdownBoxActive = 0;
    state->componentRotation = 0;
    state->currentAction = ACTION_NONE;
    state->uiState = UI_STATE_NONE;
    state->isSimulating = false;
    state->saveFileDecision = false;
    state->saveFile = false;

    InitComponentsGrid(state->grid);  // Initialize the grid with empty components

    InitWindow(screenWidth, screenHeight, "Electric circuits simulator");
    LoadResources(state);
    SetTargetFPS(60);

    return state;
}

void CleanupAppState(AppState* state) {
    UnloadResources(state);
    CloseWindow();
    free(state);
}

void Render(AppState* state) {
    RenderGrid(state);
    RenderUI(state);
    RenderPreview(state);
}

void SaveFile(AppState* state) {
    if (state->saveFile) {
        
    }
    return;
}
