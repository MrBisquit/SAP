#include "main.h"
#include "options.h"
#include "../../utils/src/GameUI.h"

void game_options_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    gui_draw_textblock(game_options_title);
    gui_draw_textblock(game_options_subtitle);

    gui_draw_button(game_options_dark_theme_button, false);
    gui_draw_textblock(game_options_dark_theme_textblock);
    gui_draw_checkbox(game_options_dark_theme_checkbox, false);

    gui_draw_button(game_options_light_theme_button, false);
    gui_draw_textblock(game_options_light_theme_textblock);
    gui_draw_checkbox(game_options_light_theme_checkbox, false);

    gui_draw_button(game_options_fps_button, false);
    gui_draw_textblock(game_options_fps_textblock);
    gui_draw_checkbox(game_options_fps_checkbox, false);

    gui_draw_button(game_options_back_button, false);

    EndDrawing();
}

void game_options_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_options_init();
    }

    if(gui_button_pressed(game_options_back_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_menu();
    }

    if(gui_button_pressed(game_options_dark_theme_button, MOUSE_BUTTON_LEFT, cursor)) {
        gui_theme = THEME_DARK;
        
        game_options_init();
    }

    if(gui_button_pressed(game_options_light_theme_button, MOUSE_BUTTON_LEFT, cursor)) {
        gui_theme = THEME_LIGHT;
        
        game_options_init();
    }

    if(gui_button_pressed(game_options_fps_button, MOUSE_BUTTON_LEFT, cursor)) {
        show_fps = !show_fps;
        
        game_options_init();
    }
}

void game_options_init() {
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

    game_options_title = TXB("Noughts And Crosses: Options", 50, (Vector2){
        35 + x_offset,
        35 + y_offset
    });

    game_options_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + y_offset
    });

    // Dark theme
    game_options_dark_theme_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + 15 + 10 + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("", 0));
    game_options_dark_theme_button.hover = game_options_dark_theme_button.normal;
    game_options_dark_theme_button.normal = gui_get_color("BACKGROUND");

    game_options_dark_theme_textblock = TXB("Dark theme", 20, (Vector2){
        game_options_dark_theme_button.rect.x + 12.5,
        game_options_dark_theme_button.rect.y + 12.5
    });

    game_options_dark_theme_checkbox = CHK((RoundRectangle){
        game_options_dark_theme_button.rect.x + game_options_dark_theme_button.rect.width - 30,
        game_options_dark_theme_button.rect.y + 10,
        20,
        20,
        0.5
    }, true);
    game_options_dark_theme_checkbox.checked = gui_theme == THEME_DARK;

    // Light theme
    game_options_light_theme_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + 15 + 10 + 40 + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("", 0));
    game_options_light_theme_button.hover = game_options_light_theme_button.normal;
    game_options_light_theme_button.normal = gui_get_color("BACKGROUND");

    game_options_light_theme_textblock = TXB("Light theme", 20, (Vector2){
        game_options_light_theme_button.rect.x + 12.5,
        game_options_light_theme_button.rect.y + 12.5
    });

    game_options_light_theme_checkbox = CHK((RoundRectangle){
        game_options_light_theme_button.rect.x + game_options_light_theme_button.rect.width - 30,
        game_options_light_theme_button.rect.y + 10,
        20,
        20,
        0.5
    }, true);
    game_options_light_theme_checkbox.checked = gui_theme == THEME_LIGHT;

    // Show FPS
    game_options_fps_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + 15 + 10 + 40 + 40 + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("", 0));
    game_options_fps_button.hover = game_options_fps_button.normal;
    game_options_fps_button.normal = gui_get_color("BACKGROUND");

    game_options_fps_textblock = TXB("Show FPS", 20, (Vector2){
        game_options_fps_button.rect.x + 12.5,
        game_options_fps_button.rect.y + 12.5
    });

    game_options_fps_checkbox = CHK((RoundRectangle){
        game_options_fps_button.rect.x + game_options_fps_button.rect.width - 30,
        game_options_fps_button.rect.y + 10,
        20,
        20,
        0.5
    }, false);
    game_options_fps_checkbox.checked = show_fps;

    game_options_back_button = BTN((RoundRectangle){
        35 + x_offset,
        GetRenderHeight() - y_offset - (35 * 2),
        50 * 14,
        40,
        0.5
    }, TXT("Back to main menu", 25));
}

void game_options_handle_resize(Rectangle old, Rectangle current) {
    game_options_init();
}