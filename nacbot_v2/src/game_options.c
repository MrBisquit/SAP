#include "main.h"
#include "game_options.h"
#include "../../utils/src/GameUI.h"
#include "game/game.h"

void game_game_options_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    gui_draw_textblock(game_game_options_title);
    gui_draw_textblock(game_game_options_subtitle);

    gui_draw_button(game_game_options_normal_3x3, false);
    gui_draw_button(game_game_options_normal_4x4, false);
    gui_draw_button(game_game_options_normal_5x5, false);

    gui_draw_button(game_game_options_back_button, false);

    EndDrawing();
}

void game_game_options_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_game_init();
    }

    if(gui_button_pressed(game_game_options_back_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_menu();
    }

    if(gui_button_pressed(game_game_options_normal_3x3, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_3X3);
    }

    if(gui_button_pressed(game_game_options_normal_4x4, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_4X4);
    }

    if(gui_button_pressed(game_game_options_normal_5x5, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_5X5);
    }
}

void game_game_options_init() {
    int x_offset = 0;
    int y_offset = 0;

    if(GetRenderWidth() >= WINDOW_DEFAULT_WIDTH ||
        GetRenderHeight() >= WINDOW_DEFAULT_HEIGHT) {
        x_offset = GetRenderWidth() - WINDOW_DEFAULT_WIDTH;
        y_offset = GetRenderHeight() - WINDOW_DEFAULT_HEIGHT;

        if(x_offset > 300)
            x_offset = 300;
        
        if(y_offset > 300)
            y_offset = 300;
    }

    game_game_options_title = TXB("Noughts And Crosses: Game Options", 50, (Vector2){
        35 + x_offset,
        35 + y_offset
    });

    game_game_options_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + y_offset
    });

    game_game_options_normal_3x3 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 0) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("3x3", 25));

    game_game_options_normal_4x4 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 1) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("4x4", 25));

    game_game_options_normal_5x5 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 2) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("5x5", 25));

    game_game_options_back_button = BTN((RoundRectangle){
        35 + x_offset,
        GetRenderHeight() - y_offset - (35 * 2),
        50 * 14,
        40,
        0.5
    }, TXT("Back to main menu", 25));
}

void game_game_options_handle_resize(Rectangle old, Rectangle current) {
    game_game_options_init();
}