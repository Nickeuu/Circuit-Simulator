#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "raylib.h"

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
#define NUMBER_OF_COMPONENTS 8

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
    COMPONENT_CAPACITOR,
    COMPONENT_BATTERY,
    COMPONENT_WIRE_CORNER,
    COMPONENT_WIRE_INTERSECTION_3WAY,
    COMPONENT_WIRE_INTERSECTION_4WAY,
    COMPONENT_PROBE
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
    int nrOfRotations;
    Texture2D texture;
    Texture2D textureRotated90;
    Texture2D textureRotated180;
    Texture2D textureRotated270;
} ComponentInfo;

// Structure to hold information about each grid cell
typedef struct {
    ComponentType type;
    float resistance;
    int rotation;
} Component;

#endif