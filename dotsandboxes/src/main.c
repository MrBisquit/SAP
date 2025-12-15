#include "main.h"
#include "../../utils/src/GameUI.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, "Dots And Boxes");
    SetWindowMinSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    SetTargetFPS(WINDOW_TARGET_FPS);

    // This system is cool, because it allows us to switch between modes easily
    game_enter_mode(MODE_MENU);
    //gui_replace_loops(&game_render_loop, &game_input_loop);

    gui_init();

    CloseWindow();

    return 0;
}

void game_start_game() {
    game_enter_mode(MODE_GAME);
}

void game_game_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    EndDrawing();
}

void game_game_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_game_init();
    }
}

void game_game_init() {
    
}