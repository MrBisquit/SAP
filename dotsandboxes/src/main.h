#pragma once

#include "../../utils/src/GameUI.h"

#define AI_NAME "SAP-GameAI-DotsAndBoxes-L3"

typedef struct mode {
    void (*render_loop)();
    void (*input_loop)();
    void (*init)();
    void (*handle_resize)(Rectangle old, Rectangle current);
} mode_t;

enum {
    MODE_MENU = 0,
    MODE_GAME = 1,
    MODE_OPTIONS = 2,
    MODE_CREDITS = 3
};
static int current_mode = MODE_MENU;

void game_start_menu();
void game_start_game();
void game_start_options();
void game_start_credits();

void game_menu_render_loop();
void game_menu_input_loop();
void game_menu_init();
void game_menu_handle_resize(Rectangle old, Rectangle current);

void game_game_render_loop();
void game_game_input_loop();
void game_game_init();

void game_options_render_loop();
void game_options_input_loop();
void game_options_init();
void game_options_handle_resize(Rectangle old, Rectangle current);

void game_credits_render_loop();
void game_credits_input_loop();
void game_credits_init();
void game_credits_handle_resize(Rectangle old, Rectangle current);

static mode_t modes[] = {
    {
        .render_loop = game_menu_render_loop,
        .input_loop = game_menu_input_loop,
        .init = game_menu_init,
        .handle_resize = game_menu_handle_resize
    },
    {
        .render_loop = game_game_render_loop,
        .input_loop = game_game_input_loop,
        .init = game_game_init
    },
    {
        .render_loop = game_options_render_loop,
        .input_loop = game_options_input_loop,
        .init = game_options_init,
        .handle_resize = game_options_handle_resize
    },
    {
        .render_loop = game_credits_render_loop,
        .input_loop = game_credits_input_loop,
        .init = game_credits_init,
        .handle_resize = game_credits_handle_resize
    }
};

static void game_enter_mode(int mode) {
    modes[mode].init();
    gui_replace_loops(modes[mode].render_loop, modes[mode].input_loop);

    // It's not really neccecary
    if(modes[mode].handle_resize)
        gui_replace_handle_resize(modes[mode].handle_resize);
}