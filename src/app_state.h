#ifndef APP_STATE_H
#define APP_STATE_H

#include "definitions.h"
#include "raylib.h"



typedef struct AppState {
    Component grid[GRID_WIDTH][GRID_HEIGHT];         // Grid of components
    ComponentInfo componentInfos[NUMBER_OF_COMPONENTS]; // Info about available components
    Vector2 mousePosition;                           // Current mouse position
    Rectangle uiLocation;                            // Location of the UI panel
    bool isPreviewing;                               // Flag for preview mode
    bool isEditing;                                  // Flag for edit mode
    bool isSimulating;                               // Flag for simulating mode
    int previewX, previewY;                          // Preview position on the grid
    int dropdownBoxActive;                           // Index of the active dropdown box selection
    int componentRotation;                           // Current rotation of the component
    ActionType currentAction;                        // Current action being performed
    UIState uiState;                                 // Current UI state
    int editX, editY;                                // Position being edited
    float editValue;                                 // Value being edited
    char valueText[8];                               // Text buffer for editing values
} AppState;

AppState* InitializeAppState(int screenWidth, int screenHeight);
void CleanupAppState(AppState* state);
void Render(AppState* state);

#endif
