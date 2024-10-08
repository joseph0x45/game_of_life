#include <raylib.h>
#include <stdio.h>

#define TARGET_FPS 10
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800
#define CELL_SIZE 15
#define GRID_ROWS (WINDOW_HEIGHT / CELL_SIZE)
#define GRID_COLS (WINDOW_WIDTH / CELL_SIZE)

typedef enum {
  DEAD,
  ALIVE,
} CellState;

typedef enum {
  PAUSED,
  RUNNING,
} SimulationState;

typedef struct {
  SimulationState state;
  int cells[GRID_ROWS][GRID_COLS];
} Simulation;

void init_simulation(Simulation *s) {
  s->state = PAUSED;
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      s->cells[row][col] = DEAD;
    }
  }
}

void draw_simulation(Simulation *s) {
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      Color color = s->cells[row][col] ? GREEN : GRAY;
      DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1,
                    CELL_SIZE - 1, color);
    }
  }
}

void draw_simulation_info(Simulation *s) {
  int alive_cells = 0;
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      if (s->cells[row][col]) {
        alive_cells++;
      }
    }
  }
  char simulation_state[100];
  sprintf(simulation_state, "Simulation state: %s",
          s->state ? "RUNNING" : "PAUSED");
  DrawText(simulation_state, 10, 10, 10, RED);
  char alive_cells_info[100];
  sprintf(alive_cells_info, "Population: %d", alive_cells);
  DrawText(alive_cells_info, 10, 20, 10, RED);
}

void process_input(Simulation *s) {
  if (IsKeyReleased(KEY_SPACE)) {
    s->state = s->state == RUNNING ? PAUSED : RUNNING;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse_position = GetMousePosition();
    int col = mouse_position.x / CELL_SIZE;
    int row = mouse_position.y / CELL_SIZE;
    s->cells[row][col] = s->cells[row][col] ? DEAD : ALIVE;
  }
}

void process_simulation(Simulation *s) {
  int temp_grid[GRID_ROWS][GRID_COLS] = {0};
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      temp_grid[row][col] = s->cells[row][col];
    }
  }
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      int cell = s->cells[row][col];
      int alive_cells = 0;
      // get alive neighbours
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0)
            continue;
          int neighbor_row = row + j;
          int neighbor_col = col + i;
          if ((neighbor_row >= 0 && neighbor_row < GRID_ROWS) &&
              (neighbor_col >= 0 && neighbor_col < GRID_COLS)) {
            if (s->cells[neighbor_row][neighbor_col]) {
              alive_cells++;
            }
          }
        }
      }
      if (cell) {
        if (alive_cells < 2) {
          temp_grid[row][col] = DEAD;
        }
        if (alive_cells == 2 || alive_cells == 3) {
          temp_grid[row][col] = ALIVE;
        }
        if (alive_cells > 3) {
          temp_grid[row][col] = DEAD;
        }
      } else {
        if (alive_cells == 3) {
          temp_grid[row][col] = ALIVE;
        }
      }
    }
  }
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      s->cells[row][col] = temp_grid[row][col];
    }
  }
}

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gayme of life");
  Simulation s = {0};
  init_simulation(&s);
  SetTargetFPS(TARGET_FPS);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    process_input(&s);
    if (s.state == RUNNING) {
      process_simulation(&s);
    }
    draw_simulation(&s);
    draw_simulation_info(&s);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
