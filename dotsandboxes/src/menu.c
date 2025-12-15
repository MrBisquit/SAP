#include "main.h"
#include "menu.h"
#include "../../utils/src/GameUI.h"

void game_menu_render_loop() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(GetRenderWidth() - 100, 10);

    gui_draw_button(game_menu_play_button, false);

    EndDrawing();
}

void game_menu_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_menu_init();
    }

    if(gui_button_pressed(game_menu_play_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_game();
    }
}

void game_menu_init() {
    game_menu_play_button = BTN((RoundRectangle){
        10,
        10,
        100,
        40,
        0
    }, TXT("Play", 25));
}