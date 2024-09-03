#include "raylib.h"
#include "app_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "definitions.h"
#include "input.h"
#include "simulator.h"


int main(void) {
    AppState* appState = InitializeAppState(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!appState) return -1; // Error handling

    while (!WindowShouldClose()) {
        HandleInput(appState);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Render(appState);
        if (appState->isSimulating) Simulator(appState);
        EndDrawing();
    }

    CleanupAppState(appState);
    return 0;
}
