#pragma once

#include "../../utils/src/GameUI.h"



typedef struct mode {
    void (*render_loop)();
    void (*input_loop)();
    void (*init)();
    void (*handle_resize)(Rectangle old, Rectangle current);
} mode_t;

enum {
    MODE_MENU = 0,
    MODE_GAME = 1
};
static int current_mode = MODE_MENU;

void game_start_game();

void game_menu_render_loop();
void game_menu_input_loop();
void game_menu_init();
void game_menu_handle_resize(Rectangle old, Rectangle current);

void game_game_render_loop();
void game_game_input_loop();
void game_game_init();

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
    }
};

static void game_enter_mode(int mode) {
    modes[mode].init();
    gui_replace_loops(modes[mode].render_loop, modes[mode].input_loop);
    gui_replace_handle_resize(modes[mode].handle_resize);
}