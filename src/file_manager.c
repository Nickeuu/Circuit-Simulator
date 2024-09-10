#include "file_manager.h"

void saveFile(char *filename, Component grid[GRID_WIDTH][GRID_HEIGHT]) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL){
        perror("Failed to open file for saving");
        exit(1);
    }

    fwrite(grid, sizeof(Component), GRID_WIDTH * GRID_HEIGHT, file);
    fclose(file);
}

void loadFile(char *filename, Component grid[GRID_WIDTH][GRID_HEIGHT]) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        perror("Failed to open file for loading");
        exit(1);
    }

    fread(grid, sizeof(Component), GRID_WIDTH * GRID_HEIGHT, file);
    fclose(file);
}
