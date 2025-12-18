#pragma once

#include "../main.h"
#include "../../../utils/src/GameUI.h"

enum {
    BOARD_4X4 = 0,
    BOARD_5X4 = 1,
    BOARD_6X5 = 2
};

typedef struct game_board_dot {
    Vector2 pos;
} game_board_dot_t;

typedef struct game_board_line {
    Vector2 from;
    Vector2 to;
    int value;
} game_board_line_t;

typedef struct game_board_box {
    game_board_dot_t dots[4];
    game_board_line_t lines[4];

    int x;
    int y;
} game_board_box_t;

typedef struct game_board {
    game_board_box_t** boxes;
} game_board_t;

/// @brief Start the game with a specific board size
/// @param size The board size (From the enum)
void game_start(int size);

void game_render_loop();
void game_input_loop();
void game_init();
void game_handle_resize(Rectangle old, Rectangle current);