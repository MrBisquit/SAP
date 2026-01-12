#include <stdio.h>
#include "../main.h"
#include "game.h"
#include "../../../utils/src/GameUI.h"

static int board_size = -1;

int player_score = 0;
int bot_score = 0;
int winner = -1;
int turn = 0;

extern char __p_score[3] = "000";
extern char __b_score[3] = "000";

int board_w = 200;
int board_h = 200;

int dialog_type = -1;
bool dialog_visible = false;
bool dialog_cover_entire = false;

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
    gui_draw_button(game_game_exit_button, dialog_visible);

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
    gui_draw_textblock(game_info_board_turn);
    gui_draw_textblock(game_info_board_player_score);
    gui_draw_textblock(game_info_board_bot_score);
    if(turn == 0) {
        gui_draw_text(
            "You",
            game_info_bounds.x + game_info_bounds.width - (20 * 3),
            game_info_board_turn.position.y,
            20,
            BLUE
        );
    } else {
        gui_draw_text(
            "Bot",
            game_info_bounds.x + game_info_bounds.width - (20 * 3),
            game_info_board_turn.position.y,
            20,
            RED
        );
    }

    gui_draw_text(
        __p_score,
        game_info_bounds.x + game_info_bounds.width - (20 * 3),
        game_info_board_player_score.position.y,
        20,
        gui_get_color("TEXT")
    );

    gui_draw_text(
        __b_score,
        game_info_bounds.x + game_info_bounds.width - (20 * 3),
        game_info_board_bot_score.position.y,
        20,
        gui_get_color("TEXT")
    );

    if(dialog_visible) {
        if(dialog_cover_entire) {
            DrawRectangle(
                0, 0, GetRenderWidth(), GetRenderHeight(),
                gui_get_color("OVERLAY")
            );
        } else {
            DrawRectangle(
                0, 60, GetRenderWidth(), GetRenderHeight() - 60,
                gui_get_color("OVERLAY")
            );
        }

        game_render_loop_dialog();
    }

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

        DrawRectangleLines(
            game_game_inner_bounds.x,
            game_game_inner_bounds.y,
            game_game_inner_bounds.width,
            game_game_inner_bounds.height,
            BLUE
        );
    }

    EndDrawing();
}

void game_input_loop() {
    if(dialog_visible) {
        game_input_loop_dialog();
        return;
    }

    Vector2 cursor = GetMousePosition();

    if(gui_button_pressed(game_game_exit_button, MOUSE_BUTTON_LEFT, cursor)) {
        //game_start_menu();
        game_enter_dialog(0);
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

    // This will change depending on the board size
    game_game_inner_bounds = (Rectangle) {
        game_game_bounds.x + (game_game_bounds.width / 2) - (board_w / 2),
        game_game_bounds.y + (game_game_bounds.height / 2) - (board_h / 2),
        board_w,
        board_h
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

    game_info_board_turn = TXB("Turn:", 20, (Vector2){
        game_info_bounds.x + 20,
        game_info_bounds.y + 20 + 20 + 10
    });

    game_info_board_player_score = TXB("Your score:", 20, (Vector2){
        game_info_bounds.x + 20,
        game_info_bounds.y + 20 + ((20 * 2) + (5 * 3))
    });

    game_info_board_bot_score = TXB("Bot score:", 20, (Vector2){
        game_info_bounds.x + 20,
        game_info_bounds.y + 20 + ((20 * 3) + (5 * 4))
    });

    game_update_data();
}

void game_handle_resize(Rectangle old, Rectangle current) {
    game_init();
}

void game_update_data() {
    sprintf(__p_score, "%03d", player_score);
    sprintf(__b_score, "%03d", bot_score);
}

void game_enter_dialog(int type) {
    if(type == -1) {
        dialog_type = -1;
        dialog_visible = false;
        dialog_cover_entire = false;
        return;
    }

    dialog_type = type;

    if(type == 0) {
        dialog_visible = true;
        dialog_cover_entire = true;

        int w = 400;
        int h = 200;

        game_game_exit_dialog.rect = (RoundRectangle) {
            (GetRenderWidth() / 2) - (w / 2),
            (GetRenderHeight() / 2) - (h / 2),
            w,
            h,
            0.125
        };

        game_game_exit_dialog.title = TXB("Are you sure?", 25, (Vector2){
            game_game_exit_dialog.rect.x + 25,
            game_game_exit_dialog.rect.y + 25
        });

        game_game_exit_dialog.description = TXB(
            "This will stop the current game and lose any\ncurrent scores.",
            15, (Vector2){
                game_game_exit_dialog.rect.x + 25,
                game_game_exit_dialog.rect.y + 25 + 25 + 10
            }
        );

        game_game_exit_dialog.yes = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_exit_dialog.rect.x + game_game_exit_dialog.rect.width - (100 * 2) - (15 * 2) + 5,
            game_game_exit_dialog.rect.y + game_game_exit_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("Yes", 20));

        game_game_exit_dialog.no = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_exit_dialog.rect.x + game_game_exit_dialog.rect.width - 100 - 15,
            game_game_exit_dialog.rect.y + game_game_exit_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("No", 20));
    }
}

void game_render_loop_dialog() {
    if(dialog_type == 0) {
        gui_draw_rectangle_round(game_game_exit_dialog.rect, gui_get_color("POPUP_BACKGROUND"));
        gui_draw_textblock(game_game_exit_dialog.title);
        gui_draw_textblock(game_game_exit_dialog.description);
        gui_draw_button(game_game_exit_dialog.yes, false);
        gui_draw_button(game_game_exit_dialog.no, false);
    }
}

void game_input_loop_dialog() {
    Vector2 cursor = GetMousePosition();

    if(dialog_type == 0) {
        if(gui_button_pressed(game_game_exit_dialog.yes, MOUSE_BUTTON_LEFT, cursor)) {
            game_start_menu();
        }

        if(gui_button_pressed(game_game_exit_dialog.no, MOUSE_BUTTON_LEFT, cursor)) {
            game_enter_dialog(-1);
        }
    }
}