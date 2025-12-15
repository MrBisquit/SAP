#include "main.h"
#include "../../utils/src/GameUI.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, "Dots And Boxes");
    SetTargetFPS(WINDOW_TARGET_FPS);

    // This system is cool, because it allows us to switch between modes easily
    game_enter_mode(MODE_MENU);
    //gui_replace_loops(&game_render_loop, &game_input_loop);

    gui_init();

    CloseWindow();

    return 0;
}



void game_game_render_loop() {
    BeginDrawing();
    ClearBackground(BLACK);

    

    EndDrawing();
}

void game_game_input_loop() {
    
}

void game_game_init() {
    
}