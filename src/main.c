#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define UI_HEIGHT 128
#define COMPONENT_SIZE 32
#define GRID_WIDTH (SCREEN_WIDTH / COMPONENT_SIZE)
#define GRID_HEIGHT ((SCREEN_HEIGHT - UI_HEIGHT) / COMPONENT_SIZE)

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 30
#define BUTTON_Y_POSITION 65
#define BUTTON_X_POSITION_START 170
#define BUTTON_X_POSITION_OFFSET 90

#define NUMBER_OF_COMPONENTS 3
#define ROTATION_90 90
#define ROTATION_180 180
#define ROTATION_270 270
#define ROTATION_360 360

typedef enum {
    COMPONENT_EMPTY,
    COMPONENT_WIRE,
    COMPONENT_RESISTOR,
    COMPONENT_CAPACITOR
} ComponentType;

typedef enum {
    ACTION_NONE,
    ACTION_DRAW,
    ACTION_EDIT,
    ACTION_DELETE
} ActionType;

typedef enum {
    UI_STATE_NONE,
    UI_STATE_DROPDOWN_ACTIVE,
} UIState;

typedef struct {
    const char* name;
    Texture2D texture;
    Texture2D textureRotated90;
    Texture2D textureRotated180;
    Texture2D textureRotated270;
} ComponentInfo;

typedef struct {
    ComponentType type;
    int value;
    int rotation;
} Component;

typedef struct {
    Component grid[GRID_WIDTH][GRID_HEIGHT];
    ComponentInfo componentInfos[NUMBER_OF_COMPONENTS];
    Vector2 mousePosition;
    Rectangle uiLocation;
    bool isPreviewing;
    int previewX, previewY;
    int dropdownBoxActive;
    int componentRotation;
    ActionType currentAction;
    UIState uiState;
} AppState;

AppState appState;

// Function prototypes
void InitializeAppState(void);
void InitComponentsGrid(Component grid[GRID_WIDTH][GRID_HEIGHT]);
void RenderGrid(AppState* state);
void DrawComponent(ComponentType type, AppState* state, int x, int y, int gridY);
void HandleInput(AppState* state);
void HandleMouseInput(AppState* state);
void HandleKeyboardInput(AppState* state);
void LoadResources(AppState* state);
void UnloadResources(AppState* state);
void RenderUI(AppState* state);
void RenderPreview(AppState* state);
void CheckTextureLoad(Texture2D texture, const char* filePath);
void RotateComponent(AppState* state);
Texture2D GetRotatedTexture(int rotation, ComponentInfo* info);
void HandleDrawAction(AppState* state);
void HandleDeleteAction(AppState* state);
void LoadComponentTextures(ComponentInfo* info, const char* basePath);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Electric circuits simulator");

    InitializeAppState();
    LoadResources(&appState);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        HandleInput(&appState);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        RenderGrid(&appState);
        RenderUI(&appState);
        RenderPreview(&appState);

        EndDrawing();
    }

    UnloadResources(&appState);
    CloseWindow();

    return 0;
}

void InitializeAppState(void) {
    appState.uiLocation = (Rectangle){0, 0, SCREEN_WIDTH, UI_HEIGHT};
    appState.isPreviewing = false;
    appState.previewX = -1;
    appState.previewY = -1;
    appState.dropdownBoxActive = 0;
    appState.componentRotation = 0;
    appState.currentAction = ACTION_NONE;
    appState.uiState = UI_STATE_NONE;

    InitComponentsGrid(appState.grid);
}

void InitComponentsGrid(Component grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y].type = COMPONENT_EMPTY;
            grid[x][y].value = 0;
            grid[x][y].rotation = 0;
        }
    }
}

void RenderGrid(AppState* state) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int componentY = (y * COMPONENT_SIZE) + UI_HEIGHT;
            DrawComponent(state->grid[x][y].type, state, x, componentY, y);
            DrawLine(x * COMPONENT_SIZE, UI_HEIGHT, x * COMPONENT_SIZE, SCREEN_HEIGHT, LIGHTGRAY);
            DrawLine(0, y * COMPONENT_SIZE + UI_HEIGHT, SCREEN_WIDTH, y * COMPONENT_SIZE + UI_HEIGHT, LIGHTGRAY);
        }
    }
}

void DrawComponent(ComponentType type, AppState* state, int x, int y, int gridY) {
    if (type != COMPONENT_EMPTY) {
        Texture2D texture = GetRotatedTexture(state->grid[x][gridY].rotation, &state->componentInfos[type - 1]);
        DrawTexture(texture, x * COMPONENT_SIZE, y, WHITE);
    }
}

Texture2D GetRotatedTexture(int rotation, ComponentInfo* info) {
    switch (rotation) {
        case ROTATION_90: return info->textureRotated90;
        case ROTATION_180: return info->textureRotated180;
        case ROTATION_270: return info->textureRotated270;
        default: return info->texture;
    }
}

void RotateComponent(AppState* state) {
    if (state->currentAction == ACTION_DRAW) {
        if (!(state->dropdownBoxActive >= 0 && state->dropdownBoxActive <= 2)) {
            state->componentRotation += ROTATION_90;
            if (state->componentRotation == ROTATION_360) state->componentRotation = 0;
        } else {
            state->componentRotation += ROTATION_90;
            if (state->componentRotation == ROTATION_180) state->componentRotation = 0;
        }
    }
}

void HandleInput(AppState* state) {
    HandleMouseInput(state);
    HandleKeyboardInput(state);
}

void HandleMouseInput(AppState* state) {
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
                case ACTION_DRAW: HandleDrawAction(state); break;
                case ACTION_DELETE: HandleDeleteAction(state); break;
                // Handle editing logic here if needed
            }
        }
        state->isPreviewing = false;
    }
}

void HandleKeyboardInput(AppState* state) {
    if (IsKeyReleased(KEY_R)) {
        RotateComponent(state);
    }
}

void HandleDrawAction(AppState* state) {
    state->grid[state->previewX][state->previewY].type = state->dropdownBoxActive + 1;
    state->grid[state->previewX][state->previewY].rotation = state->componentRotation;
}

void HandleDeleteAction(AppState* state) {
    state->grid[state->previewX][state->previewY].type = COMPONENT_EMPTY;
    state->grid[state->previewX][state->previewY].rotation = 0;
}

void RenderUI(AppState* state) {
    GuiPanel(appState.uiLocation, "Control Panel");

    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiDropdownBox((Rectangle){ 25, 65, 125, 30 }, "Wire;Resistor;Capacitor", &appState.dropdownBoxActive, appState.uiState == UI_STATE_DROPDOWN_ACTIVE))
        appState.uiState = (appState.uiState == UI_STATE_DROPDOWN_ACTIVE) ? UI_STATE_NONE : UI_STATE_DROPDOWN_ACTIVE;

    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Draw"))
        appState.currentAction = ACTION_DRAW;
    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START + BUTTON_X_POSITION_OFFSET, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Edit"))
        appState.currentAction = ACTION_EDIT;
    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START + 2 * BUTTON_X_POSITION_OFFSET, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Delete"))
        appState.currentAction = ACTION_DELETE;
}

void RenderPreview(AppState* state) {
    if (!state->isPreviewing || state->previewX < 0 || state->previewY < 0) return;

    int previewHeight = state->previewY * COMPONENT_SIZE + UI_HEIGHT;
    if (state->currentAction == ACTION_DRAW) {
        if (state->dropdownBoxActive >= 0 && state->dropdownBoxActive <= 2) {
            Texture2D previewTexture = (state->componentRotation == 0 || state->componentRotation == ROTATION_180) ?
                state->componentInfos[state->dropdownBoxActive].texture :
                state->componentInfos[state->dropdownBoxActive].textureRotated90;
            DrawTexture(previewTexture, state->previewX * COMPONENT_SIZE, previewHeight, Fade(WHITE, 0.5f));
        }
    }
}

void LoadResources(AppState* state) {
    LoadComponentTextures(&state->componentInfos[0], "resources/wire");
    LoadComponentTextures(&state->componentInfos[1], "resources/resistor");
    LoadComponentTextures(&state->componentInfos[2], "resources/capacitor");
}

void LoadComponentTextures(ComponentInfo* info, const char* basePath) {
    char filePath[128];
    
    snprintf(filePath, sizeof(filePath), "%s.png", basePath);
    info->texture = LoadTexture(filePath);

    snprintf(filePath, sizeof(filePath), "%s_rotated.png", basePath);
    info->textureRotated90 = LoadTexture(filePath);

    // Load other rotations similarly if available
    CheckTextureLoad(info->texture, filePath);
    CheckTextureLoad(info->textureRotated90, filePath);
}

void CheckTextureLoad(Texture2D texture, const char* filePath) {
    if (texture.id == 0) {
        UnloadResources(&appState);
        CloseWindow();
        exit(EXIT_FAILURE);
    }
}

void UnloadResources(AppState* state) {
    for (int i = 0; i < NUMBER_OF_COMPONENTS; i++) {
        UnloadTexture(state->componentInfos[i].texture);
        UnloadTexture(state->componentInfos[i].textureRotated90);
        UnloadTexture(state->componentInfos[i].textureRotated180);
        UnloadTexture(state->componentInfos[i].textureRotated270);
    }
}
