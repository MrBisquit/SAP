#include "main.h"
#include "options.h"
#include "../../utils/src/GameUI.h"

void game_options_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    

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
}

void game_options_init() {
    
}

void game_options_handle_resize(Rectangle old, Rectangle current) {
    
}