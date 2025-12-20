#include "../main.h"
#include "game.h"
#include "../../../utils/src/GameUI.h"

void game_start(int size) {
    game_start_play();
}

void game_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    

    EndDrawing();
}

void game_input_loop() {
    //Vector2 cursor = GetMousePosition();

    
}

void game_init() {

}

void game_handle_resize(Rectangle old, Rectangle current) {

}