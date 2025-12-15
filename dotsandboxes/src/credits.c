#include "main.h"
#include "options.h"
#include "../../utils/src/GameUI.h"

void game_credits_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    

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
}

void game_credits_init() {
    
}

void game_credits_handle_resize(Rectangle old, Rectangle current) {
    
}