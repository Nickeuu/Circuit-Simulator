#include "raylib.h"
#include "app_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Screen and UI dimensions
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(void) {
    AppState* appState = InitializeAppState(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!appState) return -1; // Error handling

    while (!WindowShouldClose()) {
        HandleInput(appState);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Render(appState);
        EndDrawing();
    }

    CleanupAppState(appState);
    return 0;
}
