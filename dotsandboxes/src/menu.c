#include "main.h"
#include "menu.h"
#include "../../utils/src/GameUI.h"

void game_menu_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    DrawFPS(GetRenderWidth() - 100, 10);

    gui_draw_textblock(game_menu_title);
    gui_draw_textblock(game_menu_subtitle);
    gui_draw_button(game_menu_play_button, false);
    gui_draw_button(game_menu_options_button, false);
    gui_draw_button(game_menu_credits_button, false);

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

    if(gui_button_pressed(game_menu_options_button, MOUSE_BUTTON_LEFT, cursor)) {
        
    }

    if(gui_button_pressed(game_menu_credits_button, MOUSE_BUTTON_LEFT, cursor)) {
        
    }
}

void game_menu_init() {
    game_menu_title = TXB("Dots And Boxes", 50, (Vector2){ 35, 35 });
    game_menu_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){ 35, 35 + 50 + 10 });

    game_menu_play_button = BTN((RoundRectangle){
        35,
        35 + 50 + 10 + (10 + 15 + 15),
        50 * 14,
        40 + 50,
        0.25
    }, TXT("Play", 25));

    game_menu_options_button = BTN((RoundRectangle){
        35,
        35 + 50 + 10 + (40 + 50 + 10) + (10 + 15 + 15),
        50 * 14,
        40,
        0.5
    }, TXT("Options", 25));

    game_menu_credits_button = BTN((RoundRectangle){
        35,
        35 + 50 + 10 + (40 + 50 + 10) + (40 + 10) + (10 + 15 + 15),
        50 * 14,
        40,
        0.5
    }, TXT("Credits", 25));
}