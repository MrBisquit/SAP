#include "main.h"
#include "menu.h"
#include "../../utils/src/GameUI.h"

// Alpha
float a = 1.0f;
bool in = true;

void game_menu_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    if(a != 0.0f && in)
        a -= 0.075f;

    if(a != 1.0f && !in)
        a += 0.075f;

    if(a <= 0.0f && in)
        a = 0.0f;

    if(a >= 1.0f && !in)
        a = 1.0f;

    gui_draw_textblock(game_menu_title);
    gui_draw_textblock(game_menu_subtitle);
    gui_draw_button(game_menu_play_button, a != 0.0f && in);
    gui_draw_button(game_menu_options_button, a != 0.0f && in);
    gui_draw_button(game_menu_credits_button, a != 0.0f && in);
    gui_draw_button(game_menu_exit_button, a != 0.0f && in);

    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), Fade(gui_get_color("BACKGROUND"), a));

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

    static bool has_exit = false;

    if(a == 1.0f) {
        if(has_exit) exit(0);
    }

    if(gui_button_pressed(game_menu_play_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_game();
    }

    if(gui_button_pressed(game_menu_options_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_options();
    }

    if(gui_button_pressed(game_menu_credits_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_credits();
    }

    if(gui_button_pressed(game_menu_exit_button, MOUSE_BUTTON_LEFT, cursor)) {
        in = false;
        has_exit = true;
        //exit(0);
    }
}

void game_menu_init() {
    //in = true;
    //a = 1.0f;

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

    game_menu_title = TXB("Dots And Boxes", 50, (Vector2){ 35 + x_offset, 35 + y_offset });
    game_menu_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + y_offset
    });

    game_menu_play_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + (10 + 15 + 15) + y_offset,
        50 * 14,
        40 + 50,
        0.25
    }, TXT("Play", 25));

    game_menu_options_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + (40 + 50 + 10) + (10 + 15 + 15) + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("Options", 25));

    game_menu_credits_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + (40 + 50 + 10) + (40 + 10) + (10 + 15 + 15) + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("Credits & About", 25));

    game_menu_exit_button = BTN((RoundRectangle){
        35 + x_offset,
        GetRenderHeight() - y_offset - (35 * 2),
        50 * 14,
        40,
        0.5
    }, TXT("Quit", 25));
}

void game_menu_handle_resize(Rectangle old, Rectangle current) {
    game_menu_init();
}