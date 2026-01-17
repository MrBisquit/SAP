#pragma once

#include "../main.h"
#include <src/GameUI.h>

enum {
    BOARD_3X3 = 0,
    BOARD_4X4 = 1,
    BOARD_5X5 = 2
};

typedef enum BOARD_PLACE {
    BOARD_PLACE_BLANK   = -1,
    BOARD_PLACE_X       = 0,
    BOARD_PLACE_O       = 1,
    BOARD_PLACE_TIE     = 2     // Seems dumb being in this enum but it's important
} BOARD_PLACE;

typedef struct game_bot_run {
    float** predictions;
    int size;

    int total_runs;

    Vector2* options;
    int total_options;
} game_bot_run_t;

typedef struct game_bot {
    float** predictions;
    int size;

    Vector2* options;
    int total_options;

    game_bot_run_t* last_run;
} game_bot_t;

typedef struct game_board_button {
    gui_button_t button;

    int x;
    int y;
} game_board_button_t;

typedef struct game_board {
    BOARD_PLACE** places;
    game_board_button_t* board_buttons;
    int size;
    int buttons;

    game_bot_t* bot;
} game_board_t;

/// @brief Start the game with a specific board size
/// @param size The board size (From the enum)
void game_start(int size);

void game_generate_board(int size);

void game_render_loop();
void game_input_loop();
void game_init();
void game_handle_resize(Rectangle old, Rectangle current);

void game_enter_dialog(int type);
void game_render_loop_dialog();
void game_input_loop_dialog();

void game_buttons_generate(game_board_t* board, Rectangle bounds);
void game_board_render(game_board_t* board, Rectangle bounds);

extern BOARD_PLACE winner;
extern BOARD_PLACE turn;

extern int board_w;
extern int board_h;

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