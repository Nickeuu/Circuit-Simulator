// resources.h
#ifndef RESOURCES_H
#define RESOURCES_H


#include "definitions.h"
#include "app_state.h"
#include "stdlib.h"
#include <stdio.h>

void LoadResources(AppState* state);
void UnloadResources(AppState* state);
void RenderFileSave(AppState* state);

#endif
