// graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "definitions.h"
#include "app_state.h"
#include "stdlib.h"


void RenderGrid(AppState* state);
void RenderUI(AppState* state);
void RenderPreview(AppState* state);
void DrawComponent(ComponentType type, AppState* state, int x, int y, int gridY);
Texture2D GetRotatedTexture(int rotation, ComponentInfo* info);
void RenderSaveFile(AppState* state);

#endif
