/*******************************************************************************************
*
*   SaveFile v1.0.0 - Save file
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Alexandru Nicolae. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "save_file");

    // save_file: controls initialization
    //----------------------------------------------------------------------------------
    bool Button002Pressed = false;
    bool Button003Pressed = false;
    bool WindowBox001Active = true;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (WindowBox001Active)
            {
                WindowBox001Active = !GuiWindowBox((Rectangle){ 312, 256, 336, 144 }, "Save file");
                Button002Pressed = GuiButton((Rectangle){ 336, 352, 120, 24 }, "Save"); 
                Button003Pressed = GuiButton((Rectangle){ 504, 352, 120, 24 }, "Cancel"); 
                GuiLabel((Rectangle){ 456, 304, 120, 24 }, "Save file?");
            }
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

