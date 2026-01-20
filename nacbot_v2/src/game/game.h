#pragma once

#include "../main.h"
#include <src/GameUI.h>
#include <stdint.h>

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

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct game_bot_board {
    int wins;
    int losses;
    int ties;

    int total;

    bool valid;
} game_bot_board_t;

typedef struct game_bot_run {
    float** predictions;
    int size;

    int total_runs;

    Point* options;
    float* options_values;
    int total_options;

    Point chosen_option;

    bool had_immediate_win;
    bool had_immediate_block;
} game_bot_run_t;

typedef struct game_bot {
    float** predictions;
    int size;

    Point* options;
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

    /// @brief This is instead of the pos variable as seen below
    Point** lines;
    int total_lines;
} game_board_t;

/// @brief Start the game with a specific board size
/// @param size The board size (From the enum)
void game_start(int size);

void game_generate_board(int size);
void game_generate_lines(game_board_t* board);

void game_render_loop();
void game_input_loop();
void game_init();
void game_handle_resize(Rectangle old, Rectangle current);

void game_enter_dialog(int type);
void game_render_loop_dialog();
void game_input_loop_dialog();

BOARD_PLACE game_check_winner(game_board_t* board);

void game_buttons_generate(game_board_t* board, Rectangle bounds);
void game_board_render(game_board_t* board, Rectangle bounds);

void game_place(game_board_t* board, int x, int y, BOARD_PLACE player);
void game_run_bot(game_board_t* board, BOARD_PLACE as);
void game_bot_simulate(game_board_t* board,
    game_bot_board_t* bot,
    BOARD_PLACE as,
    BOARD_PLACE active,
    Point start);
bool game_bot_add_option(game_board_t* board, game_bot_run_t* run, Point option, float option_value);

/*static uint8_t pos[8][3][2] = {
    { { 0, 0 }, { 1, 0 }, { 2, 0 } }, // Top line       (A)
    { { 0, 1 }, { 1, 1 }, { 2, 1 } }, // Middle line    (A)
    { { 0, 2 }, { 1, 2 }, { 2, 2 } }, // Bottom line    (A)
    { { 0, 0 }, { 0, 1 }, { 0, 2 } }, // Left line      (D)
    { { 1, 0 }, { 1, 1 }, { 1, 2 } }, // Middle line    (D)
    { { 2, 0 }, { 2, 1 }, { 2, 2 } }, // Right line     (D)
    { { 0, 0 }, { 1, 1 }, { 2, 2 } }, // Top left  -> Bottom right
    { { 2, 0 }, { 1, 1 }, { 0, 2 } }  // Top right -> Bottom left
};*/

// This is the function that actually runs the bot
Point game_bot_check_blocks(game_board_t* board, BOARD_PLACE as);
Point game_bot_check_win(game_board_t* board, BOARD_PLACE as);
game_bot_run_t game_bot_run(game_board_t* board, BOARD_PLACE as);

// Copies the board
void game_utils_copy_board(game_board_t* src, game_board_t* dest);
void game_utils_free_board(game_board_t* board);

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