#include "main.h"
#include "menu.h"
#include "../../utils/src/GameUI.h"

void game_menu_render_loop() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(0, 0);

    gui_draw_button(game_menu_play_button, false);

    EndDrawing();
}

void game_menu_input_loop() {
    
}

void game_menu_init() {
    game_menu_play_button = BTN((RoundRectangle){
        10,
        10,
        100,
        40,
        0
    }, TXT("Play", "25"));
}