#include "../main.h"
#include "game.h"
#include "../../../utils/src/GameUI.h"

static int board_size = -1;

void game_start(int size) {
    board_size = size;
    game_start_play();
}

void game_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, GetRenderHeight() - 35);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    DrawRectangle(0, 0, GetRenderWidth(), 60, gui_get_color("SECONDARY"));
    gui_draw_textblock(game_game_title);
    gui_draw_button(game_game_exit_button, false);

    if(board_size == -1) {
        gui_draw_text_center(
            "Failed to start game: Invalid board size.",
            (Rectangle){ 0, 0, GetRenderWidth(), GetRenderHeight() },
            20,
            RED
        );

        EndDrawing();

        return;
    }

    gui_draw_rectangle_round(game_info_board, gui_get_color("SECONDARY"));
    gui_draw_textblock(game_info_board_title);

    // Debug
    if(IsKeyDown(KEY_F2)) {
        DrawRectangleLines(
            game_game_bounds.x,
            game_game_bounds.y,
            game_game_bounds.width,
            game_game_bounds.height,
            PINK
        );

        DrawRectangleLines(
            game_info_bounds.x,
            game_info_bounds.y,
            game_info_bounds.width,
            game_info_bounds.height,
            PURPLE
        );
    }

    EndDrawing();
}

void game_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(gui_button_pressed(game_game_exit_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_menu();
    }
}

void game_init() {
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

    float w_chunk = GetRenderWidth() / 3;

    game_game_bounds = (Rectangle) {
        // X Y Width Height
        x_offset / 2 + 10,
        y_offset / 2 + 60 + 10,
        w_chunk * 2 - x_offset,
        GetRenderHeight() - (y_offset + 60 + 10) - 10
    };

    game_info_bounds = (Rectangle) {
        // X Y Width Height
        game_game_bounds.x + (w_chunk * 2) + 10,
        y_offset / 2 + 60 + 10,
        w_chunk - 30 - x_offset,
        GetRenderHeight() - (y_offset + 60 + 10) - 10
    };

    game_game_title = TXB("Dots And Boxes", 25, (Vector2){
        20,
        20
    });

    game_game_exit_button = BTN((RoundRectangle){
        // X Y Width Height Rounded
        GetRenderWidth() - 100 - 15,
        15,
        100,
        30,
        0.5
    }, TXT("Exit", 20));

    game_info_board = gui_rect_to_round(game_info_bounds);
    game_info_board.rounded = 0.05;

    game_info_board_title = TXB("Game Information", 20, (Vector2){
        game_info_bounds.x + 20,
        game_info_bounds.y + 20
    });
}

void game_handle_resize(Rectangle old, Rectangle current) {
    game_init();
}