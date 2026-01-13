#pragma once

#include "../main.h"
#include "../../../utils/src/GameUI.h"

enum {
    BOARD_4X4 = 0,
    BOARD_5X4 = 1,
    BOARD_6X5 = 2
};

typedef struct game_board_dot {
    int x;
    int y;
    int owned;
} game_board_dot_t;

typedef struct game_board_line {
    Vector2 from;
    Vector2 to;
    int value;
} game_board_line_t;

typedef struct game_board_box {
    game_board_dot_t* dots[4];
    game_board_line_t lines[4];

    int x;
    int y;
} game_board_box_t;

typedef struct game_board {
    game_board_box_t** boxes;
    game_board_dot_t** dots;

    int total_boxes;
    int total_dots;
} game_board_t;

/// @brief Start the game with a specific board size
/// @param size The board size (From the enum)
void game_start(int size);

void game_generate_board(int size);

void game_render_loop();
void game_input_loop();
void game_init();
void game_handle_resize(Rectangle old, Rectangle current);
void game_update_data();

void game_enter_dialog(int type);
void game_render_loop_dialog();
void game_input_loop_dialog();

void game_board_render(game_board_t* board, Rectangle bounds);

extern int player_score;
extern int bot_score;
extern int winner;
extern int turn;

extern char __p_score[3];
extern char __b_score[3];

extern int board_w;
extern int board_h;

extern int dots_x;
extern int dots_y;

extern int dialog_type;
extern bool dialog_visible;
extern bool dialog_cover_entire;

extern bool game_disable_header_buttons;

extern game_board_t board;

// Bounds (Hold F2 to see)
static Rectangle game_game_bounds;
static Rectangle game_game_inner_bounds;
static Rectangle game_info_bounds;

// Top bar
static gui_textblock_t game_game_title;
static gui_button_t game_game_reset_button;
static gui_button_t game_game_exit_button;

// Side bar (Info)
static RoundRectangle game_info_board;
static gui_textblock_t game_info_board_title;
static gui_textblock_t game_info_board_turn;
static gui_textblock_t game_info_board_player_score;
static gui_textblock_t game_info_board_bot_score;

// Dialog structures
typedef struct game_exit_dialog {
    RoundRectangle rect;
    gui_textblock_t title;
    gui_textblock_t description;
    gui_button_t yes;
    gui_button_t no;
} game_exit_dialog_t;

typedef struct game_reset_dialog {
    RoundRectangle rect;
    gui_textblock_t title;
    gui_textblock_t description;
    gui_button_t yes;
    gui_button_t no;
} game_reset_dialog_t;

typedef struct game_start_dialog {
    RoundRectangle rect;
    gui_textblock_t title;
    gui_textblock_t description;
    gui_button_t start;
    gui_button_t cancel;
} game_start_dialog_t;

// Dialogs
static game_exit_dialog_t game_game_exit_dialog;
static game_reset_dialog_t game_game_reset_dialog;
static game_start_dialog_t game_game_start_dialog;