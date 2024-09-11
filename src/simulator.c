#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PATHS 100

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    double resistances[MAX_PATHS];
    int count;
} ResistanceList;

// Function to calculate the parallel resistance of two resistors
double parallel_resistance(double r1, double r2) {
    if (r1 == 0 || r2 == 0) {
        return 0; // Short circuit condition
    }
    return 1.0 / ((1.0 / r1) + (1.0 / r2));
}

// Function to calculate the equivalent parallel resistance for a list of resistances
double calculate_parallel_equivalent(ResistanceList *list) {
    double inverse_total = 0.0;
    for (int i = 0; i < list->count; i++) {
        if (list->resistances[i] == 0) {
            return 0; // Short circuit if any path has zero resistance
        }
        inverse_total += 1.0 / list->resistances[i];
    }
    if (inverse_total == 0) {
        return 0; // Avoid division by zero
    }
    return 1.0 / inverse_total;
}

// Helper function to check if a position is within grid bounds
bool is_within_bounds(int x, int y) {
    return (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT);
}

// Function to determine valid directions based on the component type and rotation
void get_valid_directions(Component component, bool *validDirections) {
    int rotation = component.rotation % 360; // Normalize rotation to [0, 90, 180, 270]

    // Reset all directions
    validDirections[0] = validDirections[1] = validDirections[2] = validDirections[3] = false;

    printf("[DEBUG] Component Type: %d, Rotation: %d\n", component.type, rotation);

    switch (component.type) {
        case COMPONENT_WIRE:
            if (rotation == 0 || rotation == 180) {
                validDirections[2] = true; // Left
                validDirections[3] = true; // Right
                printf("[DEBUG] Wire: Horizontal (Left-Right)\n");
            } else if (rotation == 90 || rotation == 270) {
                validDirections[0] = true; // Up
                validDirections[1] = true; // Down
                printf("[DEBUG] Wire: Vertical (Up-Down)\n");
            }
            break;

        case COMPONENT_WIRE_CORNER:
            if (rotation == 0) {
                validDirections[3] = true; // Right
                validDirections[0] = true; // Up
                printf("[DEBUG] Corner: Right-Up\n");
            } else if (rotation == 90) {
                validDirections[3] = true; // Right
                validDirections[1] = true; // Down
                printf("[DEBUG] Corner: Right-Down\n");
            } else if (rotation == 180) {
                validDirections[2] = true; // Left
                validDirections[1] = true; // Down
                printf("[DEBUG] Corner: Left-Down\n");
            } else if (rotation == 270) {
                validDirections[2] = true; // Left
                validDirections[0] = true; // Up
                printf("[DEBUG] Corner: Left-Up\n");
            }
            break;

        case COMPONENT_WIRE_INTERSECTION_3WAY:
            if (rotation == 0) {
                validDirections[0] = true; // Up
                validDirections[2] = true; // Left
                validDirections[3] = true; // Right
                printf("[DEBUG] 3-Way Intersection: Up, Left, Right\n");
            } else if (rotation == 90) {
                validDirections[0] = true; // Up
                validDirections[1] = true; // Down
                validDirections[3] = true; // Right
                printf("[DEBUG] 3-Way Intersection: Up, Down, Right\n");
            } else if (rotation == 180) {
                validDirections[1] = true; // Down
                validDirections[2] = true; // Left
                validDirections[3] = true; // Right
                printf("[DEBUG] 3-Way Intersection: Down, Left, Right\n");
            } else if (rotation == 270) {
                validDirections[0] = true; // Up
                validDirections[1] = true; // Down
                validDirections[2] = true; // Left
                printf("[DEBUG] 3-Way Intersection: Up, Down, Left\n");
            }
            break;

        case COMPONENT_WIRE_INTERSECTION_4WAY:
            // 4-Way intersections always allow all directions
            validDirections[0] = true; // Up
            validDirections[1] = true; // Down
            validDirections[2] = true; // Left
            validDirections[3] = true; // Right
            printf("[DEBUG] 4-Way Intersection: All Directions\n");
            break;
    }
}

// Recursive DFS to find all paths from current position to end probe
void dfs(AppState *state, int currentX, int currentY, int endX, int endY,
         bool visited[GRID_WIDTH][GRID_HEIGHT], double currentResistance,
         ResistanceList *resList) {

    // If out of bounds or already visited, return
    if (!is_within_bounds(currentX, currentY) || visited[currentX][currentY]) {
        return;
    }

    Component currentComponent = state->grid[currentX][currentY];

    // If it's empty, return
    if (currentComponent.type == COMPONENT_EMPTY) {
        return;
    }

    // Log the current position and component type
    printf("[DEBUG] Visiting X: %d, Y: %d, Component Type: %d\n", currentX, currentY, currentComponent.type);

    // If it's a resistor, add its resistance and log the resistance value
    if (currentComponent.type == COMPONENT_RESISTOR) {
        printf("[DEBUG] Found Resistor: Adding Resistance %f\n", currentComponent.resistance);
        currentResistance += currentComponent.resistance;
    }

    // If it's a probe, save the current resistance
    if (currentComponent.type == COMPONENT_PROBE && !(currentX == endX && currentY == endY)) {
        resList->resistances[resList->count++] = currentResistance;
        printf("[DEBUG] Found End Probe at X: %d, Y: %d, Total Resistance: %f\n", currentX, currentY, currentResistance);
        return;
    }

    // Mark the current position as visited
    visited[currentX][currentY] = true;

    // Get valid directions based on the current component's rotation
    bool validDirections[4]; // [Up, Down, Left, Right]
    get_valid_directions(currentComponent, validDirections);

    // Explore all valid directions
    int directions[4][2] = {
        {currentX, currentY - 1}, // Up
        {currentX, currentY + 1}, // Down
        {currentX - 1, currentY}, // Left
        {currentX + 1, currentY}  // Right
    };

    for (int i = 0; i < 4; i++) {
        if (validDirections[i]) {
            printf("[DEBUG] Moving to Direction %d (X: %d, Y: %d)\n", i, directions[i][0], directions[i][1]);
            int nextX = directions[i][0];
            int nextY = directions[i][1];
            dfs(state, nextX, nextY, endX, endY, visited, currentResistance, resList);
        }
    }

    // Unmark the current position for other paths
    visited[currentX][currentY] = false;
}

// Function to find all probes in the grid
void find_probes(AppState *state, Position probes[], int *probe_count) {
    *probe_count = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (state->grid[x][y].type == COMPONENT_PROBE) {
                probes[*probe_count].x = x;
                probes[*probe_count].y = y;
                (*probe_count)++;
                printf("[DEBUG] Found Probe at X: %d, Y: %d\n", x, y);
                if (*probe_count >= MAX_PATHS) {
                    return;
                }
            }
        }
    }
}

// Function to calculate total equivalent resistance between two probes
double calculate_total_resistance(AppState *state, Position start, Position end) {
    bool visited[GRID_WIDTH][GRID_HEIGHT] = {false};
    ResistanceList resList;
    resList.count = 0;

    printf("[DEBUG] Starting DFS from Probe at X: %d, Y: %d to Probe at X: %d, Y: %d\n", start.x, start.y, end.x, end.y);

    dfs(state, start.x, start.y, end.x, end.y, visited, 0.0, &resList);

    if (resList.count == 0) {
        printf("[DEBUG] No path found between probes.\n");
        return -1.0; // Indicate no connection
    }

    // Log all the found resistances
    printf("[DEBUG] Paths Found: %d\n", resList.count);
    for (int i = 0; i < resList.count; i++) {
        printf("[DEBUG] Path %d Resistance: %f\n", i + 1, resList.resistances[i]);
    }

    double totalResistance = calculate_parallel_equivalent(&resList);
    return totalResistance;
}

// Function to initiate resistance calculation between probes
void FollowPath(AppState *state) {
    Position probes[MAX_PATHS];
    int probe_count = 0;

    // Find all probes in the grid
    find_probes(state, probes, &probe_count);

    if (probe_count < 2) {
        printf("[DEBUG] Need at least two probes to calculate resistance.\n");
        return;
    }

    // For simplicity, calculate resistance between the first two probes
    Position start = probes[0];
    Position end = probes[1];

    printf("[DEBUG] Calculating resistance between Probe 1 (X: %d, Y: %d) and Probe 2 (X: %d, Y: %d)\n",
           start.x, start.y, end.x, end.y);

    double totalResistance = calculate_total_resistance(state, start, end);

    if (totalResistance >= 0) {
        printf("[DEBUG] Total Equivalent Resistance: %f Ohms\n", totalResistance);
    } else {
        printf("[DEBUG] Probes are not connected.\n");
    }
}

// Updated Simulate function
void Simulate(AppState *state) {
    FollowPath(state);
}
