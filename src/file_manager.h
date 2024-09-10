#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "components.h"


void saveFile(char *filename, Component grid[GRID_WIDTH][GRID_HEIGHT]);
void loadFile(char *filename, Component grid[GRID_WIDTH][GRID_HEIGHT]);

#endif