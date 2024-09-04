// graphics.c
#include "graphics.h"
#include "raygui.h"

    // resistor_edit: controls initialization
    //----------------------------------------------------------------------------------
    bool ResistorEdit_ButtonSave = false;
    bool ResistorEdit_ButtonCancel = false;
    bool ResistorEdit_Window = true;
    bool ResistorEdit_EditMode = false;
    char ResistorEdit_Resistance[128] = "";
    //----------------------------------------------------------------------------------

    // save_file: controls initialization
    //----------------------------------------------------------------------------------
    bool SaveFile_ButtonSave = false;
    bool SaveFile_ButtonCancel = false;
    bool SaveFile_Window = true;
    //----------------------------------------------------------------------------------


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

void RenderUI(AppState* state) {
    if (!state->isEditing) GuiUnlock();

    GuiPanel(state->uiLocation, "Control Panel");

    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiDropdownBox((Rectangle){ 25, 65, 125, 30 }, "Wire;Resistor;Capacitor;Battery;Wire corner;Wire intersection (3 way);Wire intersection (4 way);Probe", 
        &state->dropdownBoxActive, state->uiState == UI_STATE_DROPDOWN_ACTIVE))
        state->uiState = (state->uiState == UI_STATE_DROPDOWN_ACTIVE) ? UI_STATE_NONE : UI_STATE_DROPDOWN_ACTIVE;

    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Draw"))
        state->currentAction = ACTION_DRAW;
    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START + BUTTON_X_POSITION_OFFSET, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Edit"))
        state->currentAction = ACTION_EDIT;
    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START + 2 * BUTTON_X_POSITION_OFFSET, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Delete"))
        state->currentAction = ACTION_DELETE;
    if (GuiButton((Rectangle){ BUTTON_X_POSITION_START + 3 * BUTTON_X_POSITION_OFFSET, BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT }, "Simulate"))
        state->isSimulating = !state->isSimulating;
 
    if (state->isEditing) {
        GuiUnlock();

        switch (state->grid[state->editX][state->editY].type)
        {
            // Resistor edit
            //----------------------------------------------------------------------------------
        case COMPONENT_RESISTOR:
            if (ResistorEdit_Window)
            {
                ResistorEdit_Window = !GuiWindowBox((Rectangle){ 312, 256, 336, 144 }, "Edit Resistor");
                ResistorEdit_ButtonSave = GuiButton((Rectangle){ 336, 352, 120, 24 }, "Save"); 
                ResistorEdit_ButtonCancel = GuiButton((Rectangle){ 504, 352, 120, 24 }, "Cancel"); 
                if (GuiTextBox((Rectangle){ 408, 304, 144, 24 }, state->valueText, sizeof(state->valueText), ResistorEdit_EditMode)) {
                    ResistorEdit_EditMode = !ResistorEdit_EditMode;
                    state->editValue = atof(state->valueText);  // Convert text input to float
                } 
                GuiLabel((Rectangle){ 336, 304, 120, 24 }, "Resistance");

                // Handle save
                if (ResistorEdit_ButtonSave) {
                    state->grid[state->editX][state->editY].resistance = state->editValue;  // Cast to int if needed
                    state->isEditing = false;
                }

                // Handle cancel
                if (ResistorEdit_ButtonCancel) state->isEditing = false;

                // Handle windows close
                if (!ResistorEdit_Window) state->isEditing = false;
            }
            
            break;
            //----------------------------------------------------------------------------------
        
        default:
            state->isEditing = false;
            break;
        }
            

        
        // // Close the window if the X button is clicked
        // if (GuiWindowBox((Rectangle){300, 200, 250, 150}, "Edit Component")) {
        //     state->isEditing = false;
        // } else {
        //     // Text box for component value input
        //     if (GuiTextBox((Rectangle){310, 250, 120, 30}, state->valueText, sizeof(state->valueText), true)) {
        //         state->editValue = atof(state->valueText);  // Convert text input to float
        //     }

        //     // Apply button to save changes
        //     if (GuiButton((Rectangle){310, 300, 100, 30}, "Apply")) {
        //         state->grid[state->editX][state->editY].value = (int)state->editValue;  // Cast to int if needed
        //         state->isEditing = false;
        //     }

        //     // Cancel button to discard changes
        //     if (GuiButton((Rectangle){420, 300, 100, 30}, "Cancel")) {
        //         state->isEditing = false;
        //     }
        // }
        
        GuiLock();
    }
}

// Function to render the preview of the current action
void RenderPreview(AppState* state) {
    if (!state->isPreviewing || state->previewX < 0 || state->previewY < 0) return;

    int previewHeight = state->previewY * COMPONENT_SIZE + UI_HEIGHT;
    if (state->currentAction == ACTION_DRAW) {
        Texture2D previewTexture = GetRotatedTexture(state->componentRotation, &state->componentInfos[state->dropdownBoxActive]);
        DrawTexture(previewTexture, state->previewX * COMPONENT_SIZE, previewHeight, Fade(WHITE, 0.5f));
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

    int rot = rotation;

    // Check for no rotations
    if (info->nrOfRotations == 0) return info->texture;

    // Check for one rotation
    if (info->nrOfRotations == 1 && rot > 90) {
        rot -= 180;
    }

    switch (rot) {
        case ROTATION_90: return info->textureRotated90;
        case ROTATION_180: return info->textureRotated180;
        case ROTATION_270: return info->textureRotated270;
        default: return info->texture;
    }
}

void RenderSaveFile(AppState* state) {
    // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (SaveFile_Window)
            {
                SaveFile_Window = !GuiWindowBox((Rectangle){ 312, 256, 336, 144 }, "Save file");
                SaveFile_ButtonSave = GuiButton((Rectangle){ 336, 352, 120, 24 }, "Save"); 
                SaveFile_ButtonCancel = GuiButton((Rectangle){ 504, 352, 120, 24 }, "Cancel"); 
                GuiLabel((Rectangle){ 456, 304, 120, 24 }, "Save file?");

                // Handle save
                if (SaveFile_ButtonSave) {
                    state->saveFile = true;
                    state->saveFileDecision = true;
                }

                // Handle cancel
                if (SaveFile_ButtonCancel) {
                    state->saveFile = false;
                    state->saveFileDecision = true;
                }
            }
            //----------------------------------------------------------------------------------
    return;
}