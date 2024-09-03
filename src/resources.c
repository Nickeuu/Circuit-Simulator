// resources.c
#include "resources.h"

void LoadResources(AppState* state) {
    int id = 0;
    // Wire
    state->componentInfos[id].nrOfRotations = 1;     // Save for each component how many rotations for resource management
    LoadComponentTextures(&state->componentInfos[id], "resources/wire", &state);
    id++;
    // Resistor
    state->componentInfos[id].nrOfRotations = 1;
    LoadComponentTextures(&state->componentInfos[id], "resources/resistor", &state);
    id++;
    // Capacitor
    state->componentInfos[id].nrOfRotations = 1;
    LoadComponentTextures(&state->componentInfos[id], "resources/capacitor", &state);
    id++;
    // Battery
    state->componentInfos[id].nrOfRotations = 3;
    LoadComponentTextures(&state->componentInfos[id], "resources/battery", &state);
    id++;
    // Wire corner
    state->componentInfos[id].nrOfRotations = 3;
    LoadComponentTextures(&state->componentInfos[id], "resources/wire_corner", &state);
    id++;
    // Wire intersection 3 way
    state->componentInfos[id].nrOfRotations = 3;
    LoadComponentTextures(&state->componentInfos[id], "resources/wire_intersection_3way", &state);
    id++;
    // Wire intersection 4 way
    state->componentInfos[id].nrOfRotations = 0;
    LoadComponentTextures(&state->componentInfos[id], "resources/wire_intersection_4way", &state);
    id++;
    // Probe
    state->componentInfos[id].nrOfRotations = 0;
    LoadComponentTextures(&state->componentInfos[id], "resources/probe", &state);
    id++;
}

void LoadComponentTextures(ComponentInfo* info, const char* basePath, AppState* state) {
    char filePath[128];
    
    snprintf(filePath, sizeof(filePath), "%s.png", basePath);
    info->texture = LoadTexture(filePath);
    CheckTextureLoad(info->texture, filePath, &state);
    if (info->nrOfRotations == 0) return;        // If only one texture for the component, return

    snprintf(filePath, sizeof(filePath), "%s_rotated.png", basePath);
    info->textureRotated90 = LoadTexture(filePath);
    CheckTextureLoad(info->textureRotated90, filePath, &state);
    if (info->nrOfRotations == 1) return;       // only 2 textures, return

    snprintf(filePath, sizeof(filePath), "%s_rotated1.png", basePath);
    info->textureRotated180 = LoadTexture(filePath);
    CheckTextureLoad(info->textureRotated180, filePath, &state);

    snprintf(filePath, sizeof(filePath), "%s_rotated2.png", basePath);
    info->textureRotated270 = LoadTexture(filePath);
    CheckTextureLoad(info->textureRotated270, filePath, &state);


}

void CheckTextureLoad(Texture2D texture, const char* filePath, AppState* state) {
    if (texture.id == 0) {
        UnloadResources(&state);
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
