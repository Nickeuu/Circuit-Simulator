#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Screen and UI dimensions
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define UI_HEIGHT 128
#define COMPONENT_SIZE 32

// Grid dimensions based on screen size and component size
#define GRID_WIDTH (SCREEN_WIDTH / COMPONENT_SIZE)
#define GRID_HEIGHT ((SCREEN_HEIGHT - UI_HEIGHT) / COMPONENT_SIZE)

// UI button dimensions and positions
#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 30
#define BUTTON_Y_POSITION 65
#define BUTTON_X_POSITION_START 170
#define BUTTON_X_POSITION_OFFSET 90

// Number of different components in the simulation
#define NUMBER_OF_COMPONENTS 3

// Rotation angles
#define ROTATION_90 90
#define ROTATION_180 180
#define ROTATION_270 270
#define ROTATION_360 360

// Enumeration for different types of components
typedef enum {
    COMPONENT_EMPTY,
    COMPONENT_WIRE,
    COMPONENT_RESISTOR,
    COMPONENT_CAPACITOR
} ComponentType;

// Enumeration for different user actions
typedef enum {
    ACTION_NONE,
    ACTION_DRAW,
    ACTION_EDIT,
    ACTION_DELETE
} ActionType;

// Enumeration for UI state management
typedef enum {
    UI_STATE_NONE,
    UI_STATE_DROPDOWN_ACTIVE,
} UIState;

// Structure to hold information about each component
typedef struct {
    const char* name;
    Texture2D texture;
    Texture2D textureRotated90;
    Texture2D textureRotated180;
    Texture2D textureRotated270;
} ComponentInfo;

// Structure to hold information about each grid cell
typedef struct {
    ComponentType type;
    int value;
    int rotation;
} Component;

// Main application state structure
typedef struct {
    Component grid[GRID_WIDTH][GRID_HEIGHT];         // Grid of components
    ComponentInfo componentInfos[NUMBER_OF_COMPONENTS]; // Info about available components
    Vector2 mousePosition;                           // Current mouse position
    Rectangle uiLocation;                            // Location of the UI panel
    bool isPreviewing;                               // Flag for preview mode
    bool isEditing;                                  // Flag for edit mode
    int previewX, previewY;                          // Preview position on the grid
    int dropdownBoxActive;                           // Index of the active dropdown box selection
    int componentRotation;                           // Current rotation of the component
    ActionType currentAction;                        // Current action being performed
    UIState uiState;                                 // Current UI state
    int editX, editY;                                // Position being edited
    float editValue;                                 // Value being edited
    char valueText[8];                               // Text buffer for editing values
} AppState;

AppState appState; // Global application state

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
void HandleEditAction(AppState* state);
void LoadComponentTextures(ComponentInfo* info, const char* basePath);

int main(void) {
    // Initialize the window with specified dimensions and title
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Electric circuits simulator");

    // Initialize the application state
    InitializeAppState();
    
    // Load resources such as textures
    LoadResources(&appState);

    // Set the game loop to run at 60 frames per second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        HandleInput(&appState);  // Process input

        BeginDrawing();  // Start drawing

        ClearBackground(RAYWHITE);  // Clear the screen

        RenderGrid(&appState);  // Render the grid of components
        RenderUI(&appState);    // Render the UI
        RenderPreview(&appState);  // Render the preview of the current action

        EndDrawing();  // End drawing
    }

    // Unload resources and close the window
    UnloadResources(&appState);
    CloseWindow();

    return 0;
}

// Function to initialize the application state
void InitializeAppState(void) {
    appState.uiLocation = (Rectangle){0, 0, SCREEN_WIDTH, UI_HEIGHT};
    appState.isPreviewing = false;
    appState.isEditing = false;
    appState.previewX = -1;
    appState.previewY = -1;
    appState.dropdownBoxActive = 0;
    appState.componentRotation = 0;
    appState.currentAction = ACTION_NONE;
    appState.uiState = UI_STATE_NONE;

    InitComponentsGrid(appState.grid);  // Initialize the grid with empty components
}

// Function to initialize the grid with empty components
void InitComponentsGrid(Component grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y].type = COMPONENT_EMPTY;  // Set each grid cell to empty
            grid[x][y].value = 0;
            grid[x][y].rotation = 0;
        }
    }
}

// Function to render the grid of components
void RenderGrid(AppState* state) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int componentY = (y * COMPONENT_SIZE) + UI_HEIGHT;
            DrawComponent(state->grid[x][y].type, state, x, componentY, y);  // Draw each component
            DrawLine(x * COMPONENT_SIZE, UI_HEIGHT, x * COMPONENT_SIZE, SCREEN_HEIGHT, LIGHTGRAY);  // Draw vertical grid lines
            DrawLine(0, y * COMPONENT_SIZE + UI_HEIGHT, SCREEN_WIDTH, y * COMPONENT_SIZE + UI_HEIGHT, LIGHTGRAY);  // Draw horizontal grid lines
        }
    }
}

// Function to draw a specific component at a given position
void DrawComponent(ComponentType type, AppState* state, int x, int y, int gridY) {
    if (type != COMPONENT_EMPTY) {
        Texture2D texture = GetRotatedTexture(state->grid[x][gridY].rotation, &state->componentInfos[type - 1]);
        DrawTexture(texture, x * COMPONENT_SIZE, y, WHITE);  // Draw the component's texture
    }
}

// Function to get the texture corresponding to a given rotation
Texture2D GetRotatedTexture(int rotation, ComponentInfo* info) {
    switch (rotation) {
        case ROTATION_90: return info->textureRotated90;
        case ROTATION_180: return info->textureRotated180;
        case ROTATION_270: return info->textureRotated270;
        default: return info->texture;
    }
}

// Function to rotate the currently selected component
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

// Function to handle input from the user
void HandleInput(AppState* state) {
    HandleMouseInput(state);  // Handle mouse input
    HandleKeyboardInput(state);  // Handle keyboard input
}

// Function to handle mouse input
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
                case ACTION_NONE: break;
                case ACTION_DRAW: HandleDrawAction(state); break;
                case ACTION_DELETE: HandleDeleteAction(state); break;
                case ACTION_EDIT: HandleEditAction(state); break;
            }
        }
        state->isPreviewing = false;
    }
}

// Function to handle keyboard input
void HandleKeyboardInput(AppState* state) {
    if (IsKeyReleased(KEY_R)) {
        RotateComponent(state);  // Rotate the component if 'R' is pressed
    }
}

// Function to handle the drawing action (placing a component on the grid)
void HandleDrawAction(AppState* state) {
    state->grid[state->previewX][state->previewY].type = state->dropdownBoxActive + 1;
    state->grid[state->previewX][state->previewY].rotation = state->componentRotation;
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

// Function to render the UI elements
void RenderUI(AppState* state) {
    if (!state->isEditing) GuiUnlock();
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
 
    if (state->isEditing) {
        GuiUnlock();
        
        // Close the window if the X button is clicked
        if (GuiWindowBox((Rectangle){300, 200, 250, 150}, "Edit Component")) {
            state->isEditing = false;
        } else {
            // Text box for component value input
            if (GuiTextBox((Rectangle){310, 250, 120, 30}, state->valueText, sizeof(state->valueText), true)) {
                state->editValue = atof(state->valueText);  // Convert text input to float
            }

            // Apply button to save changes
            if (GuiButton((Rectangle){310, 300, 100, 30}, "Apply")) {
                state->grid[state->editX][state->editY].value = (int)state->editValue;  // Cast to int if needed
                state->isEditing = false;
            }

            // Cancel button to discard changes
            if (GuiButton((Rectangle){420, 300, 100, 30}, "Cancel")) {
                state->isEditing = false;
            }
        }
        
        GuiLock();
    }
}

// Function to render the preview of the current action
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

// Function to load resources such as textures for components
void LoadResources(AppState* state) {
    LoadComponentTextures(&state->componentInfos[0], "resources/wire");
    LoadComponentTextures(&state->componentInfos[1], "resources/resistor");
    LoadComponentTextures(&state->componentInfos[2], "resources/capacitor");
}

// Function to load textures for a component from a base path
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

// Function to check if a texture loaded correctly
void CheckTextureLoad(Texture2D texture, const char* filePath) {
    if (texture.id == 0) {
        UnloadResources(&appState);
        CloseWindow();
        exit(EXIT_FAILURE);
    }
}

// Function to unload resources such as textures when closing the program
void UnloadResources(AppState* state) {
    for (int i = 0; i < NUMBER_OF_COMPONENTS; i++) {
        UnloadTexture(state->componentInfos[i].texture);
        UnloadTexture(state->componentInfos[i].textureRotated90);
        UnloadTexture(state->componentInfos[i].textureRotated180);
        UnloadTexture(state->componentInfos[i].textureRotated270);
    }
}
