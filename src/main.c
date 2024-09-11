#include "raylib.h"
#include "app_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "definitions.h"
#include "input.h"
#include "simulator.h"
#include "graphics.h"
#include "action_controller.h"
#include "components.h"
#include "resources.h"
#include "file_manager.h"


int main(void) {
    AppState* appState = InitializeAppState(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!appState) return -1; // Error handling

    while (!WindowShouldClose()) {
        HandleInput(appState);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Render(appState);
        if (appState->isSimulating) {
            Simulate(appState);
            appState->isSimulating = false;
        } 
        EndDrawing();
    }

    while (!appState->saveFileDecision) {
        BeginDrawing();
        RenderSaveFile(appState);
        SaveFile(appState);
        EndDrawing();
    }
    

    CleanupAppState(appState);
    return 0;
}
