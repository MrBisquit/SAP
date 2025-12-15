#include "main.h"
#include "credits.h"
#include "../../utils/src/GameUI.h"

void game_credits_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    gui_draw_textblock(game_credits_title);
    gui_draw_textblock(game_credits_subtitle);

    gui_draw_textblock(game_credits_about);

    gui_draw_button(game_credits_back_button, false);
    
    EndDrawing();
}

void game_credits_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_credits_init();
    }

    if(gui_button_pressed(game_credits_back_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_menu();
    }
}

void game_credits_init() {
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

    game_credits_title = TXB("Dots And Boxes: Credits", 50, (Vector2){
        35 + x_offset,
        35 + y_offset
    });

    game_credits_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + y_offset
    });

    game_credits_about = TXB(game_credits_about_text, 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + (15 * 10) + y_offset
    });

    game_credits_back_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + (15 * 10) + (7 * 15) + 125 + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("Back to main menu", 25));
}

void game_credits_handle_resize(Rectangle old, Rectangle current) {
    game_credits_init();
}