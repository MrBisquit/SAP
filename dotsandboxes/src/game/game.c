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

int dots_x = 0;
int dots_y = 0;

int boxes_x = 0;
int boxes_y = 0;

int lines_x = 0;
int lines_y = 0;

int dialog_type = -1;
bool dialog_visible = false;
bool dialog_cover_entire = false;

bool game_disable_header_buttons = false;

game_board_t board;

bool dot_dragging = false;
game_board_dot_t* first_dot;

void game_start(int size) {
    board_size = size;
    game_start_play();
    game_enter_dialog(2);

    if(size == 0) {
        board_w = 250;
        board_h = 250;
    } else if(size == 1) {
        board_w = 350;
        board_h = 350;
    } else if(size == 2) {
        board_w = 450;
        board_h = 450;
    }
}

void game_generate_board(int size) {
    int dots_w = 0;
    int dots_h = 0;

    int boxes_w = 0;
    int boxes_h = 0;

    if(size == 0) {
        dots_w = 4;
        dots_h = 4;
    } else if(size == 1) {
        dots_w = 5;
        dots_h = 4;
    } else if(size == 2) {
        dots_w = 6;
        dots_h = 5;
    }

    boxes_w = dots_w - 1;
    boxes_h = dots_h - 1;

    board.dots = malloc(sizeof(game_board_dot_t*) * dots_h);

    for(int y = 0; y < dots_h; y++) {
        board.dots[y] = malloc(sizeof(game_board_dot_t) * dots_w);
        for(int x = 0; x < dots_w; x++) {
            board.dots[y][x] = (game_board_dot_t){
                x, y, -1
            };
        }
    }

    board.boxes = malloc(sizeof(game_board_box_t*) * boxes_h);

    /*
        This needs to be calculated so that we have the correct amount of lines:
        - Per row, same amount as boxes
        - Per col, same amount as boxes
    */

    int _lines_per_row = boxes_w;
    int _lines_per_col = boxes_h;
    int _lines_rows = boxes_h;
    int _lines_cols = boxes_w;
    int _lines_rows_total = _lines_per_row * _lines_rows;
    int _lines_cols_total = _lines_per_col * _lines_cols;
    int lines_total = _lines_rows_total + _lines_cols_total;

    printf("%d", lines_total);

    //board.lines = malloc(sizeof(game_board_line_t*) * lines_w);

    for(int y = 0; y < boxes_h; y++) {
        board.boxes[y] = malloc(sizeof(game_board_box_t) * boxes_w);
        for(int x = 0; x < boxes_w; x++) {
            board.boxes[y][x] = (game_board_box_t){
                {
                    &board.dots[y][x],
                    &board.dots[y + 1][x],
                    &board.dots[y + 1][x + 1],
                    &board.dots[y][x + 1]
                },
                {
                    /*{
                        &board.dots[y][x],
                        &board.dots[y + 1][x],
                        -1
                    },
                    {
                        &board.dots[y + 1][x],
                        &board.dots[y + 1][x + 1],
                        -1
                    },
                    {
                        &board.dots[y + 1][x + 1],
                        &board.dots[y][x + 1],
                        -1
                    }, {
                        &board.dots[y][x + 1],
                        &board.dots[y][x],
                        -1
                    }*/

                    {
                        // Fill in
                    }
                },
                x, y, -1
            };
        }
    }

    board.total_dots = dots_w * dots_h;
    board.total_boxes = (dots_w - 1) * (dots_h - 1);
    dots_x = dots_w;
    dots_y = dots_h;
    boxes_y = boxes_w;
    boxes_x = boxes_h;
}

void game_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, GetRenderHeight() - 35);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    DrawRectangle(0, 0, GetRenderWidth(), 60, gui_get_color("SECONDARY"));
    gui_draw_textblock(game_game_title);
    gui_draw_button(game_game_reset_button, dialog_cover_entire);
    gui_draw_button(game_game_exit_button, dialog_cover_entire);

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

    if(board.total_dots > 0) {
        game_board_render(&board, game_game_inner_bounds);
    }

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

        if(board.total_dots > 0) {
            int each_w = board_w / dots_x;
            int each_h = board_h / dots_y;

            int dots_offset_x = 20;
            int dots_offset_y = 20;

            for(int y = 0; y < dots_y; y++) {
                for(int x = 0; x < dots_x; x++) {
                    DrawRectangleLines(
                        game_game_inner_bounds.x + (each_w * x) + dots_offset_x,
                        game_game_inner_bounds.y + (each_h * y) + dots_offset_y,
                        10,
                        10,
                        YELLOW
                    );
                }
            }
        }
    }

    EndDrawing();
}

void game_input_loop() {
    if(dialog_visible) {
        game_input_loop_dialog();

        if(dialog_cover_entire)
            return;
    }

    Vector2 cursor = GetMousePosition();

    if(gui_button_pressed(game_game_exit_button, MOUSE_BUTTON_LEFT, cursor)) {
        if(dialog_type == 2)
            game_start_menu();
        else
            game_enter_dialog(0);
    }

    if(gui_button_pressed(game_game_reset_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_enter_dialog(1);
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

    game_game_reset_button = BTN((RoundRectangle){
        // X Y Width Height Rounded
        GetRenderWidth() - 100 - 15 - 150 - 5,
        15,
        150,
        30,
        0.5
    }, TXT("Reset game", 20));

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
    game_enter_dialog(dialog_type);
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
        game_disable_header_buttons = false;
        return;
    }

    dialog_type = type;

    if(type == 0) {
        dialog_visible = true;
        dialog_cover_entire = true;
        game_disable_header_buttons = false;

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
    } else if(type == 1) {
        dialog_visible = true;
        dialog_cover_entire = true;
        game_disable_header_buttons = false;

        int w = 400;
        int h = 200;

        game_game_reset_dialog.rect = (RoundRectangle) {
            (GetRenderWidth() / 2) - (w / 2),
            (GetRenderHeight() / 2) - (h / 2),
            w,
            h,
            0.125
        };

        game_game_reset_dialog.title = TXB("Are you sure?", 25, (Vector2){
            game_game_reset_dialog.rect.x + 25,
            game_game_reset_dialog.rect.y + 25
        });

        game_game_reset_dialog.description = TXB(
            "This will reset the current game and lose any\ncurrent scores.",
            15, (Vector2){
                game_game_reset_dialog.rect.x + 25,
                game_game_reset_dialog.rect.y + 25 + 25 + 10
            }
        );

        game_game_reset_dialog.yes = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_reset_dialog.rect.x + game_game_reset_dialog.rect.width - (100 * 2) - (15 * 2) + 5,
            game_game_reset_dialog.rect.y + game_game_reset_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("Yes", 20));

        game_game_reset_dialog.no = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_reset_dialog.rect.x + game_game_reset_dialog.rect.width - 100 - 15,
            game_game_reset_dialog.rect.y + game_game_reset_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("No", 20));
    } else if(type == 2) {
        dialog_visible = true;
        dialog_cover_entire = false;
        game_disable_header_buttons = true;

        int w = 400;
        int h = 200;

        /// @todo Adjust the Y position to get it centered vertically
        game_game_start_dialog.rect = (RoundRectangle) {
            (GetRenderWidth() / 2) - (w / 2),
            (GetRenderHeight() / 2) - (h / 2) - (60 / 4),
            w,
            h,
            0.125
        };

        game_game_start_dialog.title = TXB("Dots And Boxes", 25, (Vector2){
            game_game_start_dialog.rect.x + 25,
            game_game_start_dialog.rect.y + 25
        });

        game_game_start_dialog.description = TXB(
            "Press start when you are ready to start the\ngame.",
            15, (Vector2){
                game_game_start_dialog.rect.x + 25,
                game_game_start_dialog.rect.y + 25 + 25 + 10
            }
        );

        game_game_start_dialog.start = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_start_dialog.rect.x + game_game_start_dialog.rect.width - (100 * 2) - (15 * 2) + 5,
            game_game_start_dialog.rect.y + game_game_start_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("Start", 20));

        game_game_start_dialog.cancel = BTN((RoundRectangle){
            // X Y Width Height Rounded
            game_game_start_dialog.rect.x + game_game_start_dialog.rect.width - 100 - 15,
            game_game_start_dialog.rect.y + game_game_start_dialog.rect.height - 30 - 15,
            100,
            30,
            0.5
        }, TXT("Cancel", 20));
    }

    game_init();
}

void game_render_loop_dialog() {
    if(dialog_type == 0) {
        gui_draw_rectangle_round(game_game_exit_dialog.rect, gui_get_color("POPUP_BACKGROUND"));
        gui_draw_textblock(game_game_exit_dialog.title);
        gui_draw_textblock(game_game_exit_dialog.description);
        gui_draw_button(game_game_exit_dialog.yes, false);
        gui_draw_button(game_game_exit_dialog.no, false);
    } else if(dialog_type == 1) {
        gui_draw_rectangle_round(game_game_reset_dialog.rect, gui_get_color("POPUP_BACKGROUND"));
        gui_draw_textblock(game_game_reset_dialog.title);
        gui_draw_textblock(game_game_reset_dialog.description);
        gui_draw_button(game_game_reset_dialog.yes, false);
        gui_draw_button(game_game_reset_dialog.no, false);
    } else if(dialog_type == 2) {
        gui_draw_rectangle_round(game_game_start_dialog.rect, gui_get_color("POPUP_BACKGROUND"));
        gui_draw_textblock(game_game_start_dialog.title);
        gui_draw_textblock(game_game_start_dialog.description);
        gui_draw_button(game_game_start_dialog.start, false);
        gui_draw_button(game_game_start_dialog.cancel, false);
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
    } else if(dialog_type == 1) {
        if(gui_button_pressed(game_game_reset_dialog.yes, MOUSE_BUTTON_LEFT, cursor)) {
            game_generate_board(board_size);
            player_score = 0;
            bot_score = 0;
            game_update_data();

            game_enter_dialog(-1);
        }

        if(gui_button_pressed(game_game_reset_dialog.no, MOUSE_BUTTON_LEFT, cursor)) {
            game_enter_dialog(-1);
        }
    } else if(dialog_type == 2) {
        if(gui_button_pressed(game_game_start_dialog.start, MOUSE_BUTTON_LEFT, cursor)) {
            game_generate_board(board_size);

            game_enter_dialog(-1);
        }

        if(gui_button_pressed(game_game_start_dialog.cancel, MOUSE_BUTTON_LEFT, cursor)) {
            game_start_menu();
        }
    }
}

void game_board_render(game_board_t* board, Rectangle bounds) {
    Vector2 cursor = GetMousePosition();

    int each_w = board_w / dots_x;
    int each_h = board_h / dots_y;

    int dots_offset_x = 20;
    int dots_offset_y = 20;

    board->boxes[0][0].owned = 1;
    board->boxes[0][0].lines[0].owned = 1;
    board->boxes[0][0].lines[1].owned = 1;
    board->boxes[0][0].lines[2].owned = 1;
    board->boxes[0][0].lines[3].owned = 1;
    board->boxes[0][1].owned = 0;

    for(int y = 0; y < boxes_y; y++) {
        for(int x = 0; x < boxes_x; x++) {
            Color c;

            if(board->boxes[y][x].owned == -1) {
                c.a = 0;
            } else if(board->boxes[y][x].owned == 0) {
                c = BLUE;
                c.a /= 2;
            } else {
                c = RED;
                c.a /= 2;
            }

            int _x = bounds.x + (each_w * x) + dots_offset_x;
            int _y = bounds.y + (each_h * y) + dots_offset_y;
            int _w = each_w;
            int _h = each_h;

            DrawRectangle(_x, _y, _w, _h, c);

            for(int i = 0; i < 4; i++) {
                Color _c;

                int _fx = board->boxes[y][x].lines[i].from->circle.x;
                int _fy = board->boxes[y][x].lines[i].from->circle.y;
                int _tx = board->boxes[y][x].lines[i].to->circle.x;
                int _ty = board->boxes[y][x].lines[i].to->circle.y;

                if(board->boxes[y][x].lines[i].owned == -1) {
                    _c = gui_get_color("BUTTON");
                    _c.a = 255;
                    _c.r /= 2;
                    _c.g /= 2;
                    _c.b /= 2;
                } else if(board->boxes[y][x].lines[i].owned == 0) {
                    _c = BLUE;
                } else {
                    _c = RED;
                }

                DrawLineEx(
                    (Vector2) { _fx, _fy },
                    (Vector2) { _tx, _ty },
                    7.5,
                    _c
                );
            }
        }
    }

    for(int y = 0; y < dots_y; y++) {
        for(int x = 0; x < dots_x; x++) {
            Color c;

            if(board->dots[y][x].owned == -1) {
                c = gui_get_color("BUTTON"); // Just because it's a bright colour
            } else if(board->dots[y][x].owned == 0) {
                c = BLUE;
            } else {
                c = RED;
            }

            int _x = bounds.x + (each_w * x) + dots_offset_x;
            int _y = bounds.y + (each_h * y) + dots_offset_y;
            int _w = 10;
            int _h = 10;

            //DrawCircle(_x + (_w / 2), _y + (_h / 2), 10, c);

            board->dots[y][x].circle = CIR((Vector2){ _x, _y}, 10, 0, true);

            if(gui_point_in_circle(cursor, board->dots[y][x].circle) &&
                !dialog_visible &&
                board->dots[y][x].owned == -1) {
                c.r *= 2;
                c.g *= 2;
                c.b *= 2;
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }

            if(gui_point_in_circle(cursor, board->dots[y][x].circle) &&
                !dialog_visible &&
                board->dots[y][x].owned == -1 &&
                IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if(dot_dragging) {
                    c.r *= 2;
                    c.g *= 2;
                    c.b *= 2;
                    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                } else {
                    dot_dragging = true;
                    first_dot = &board->dots[y][x];
                }
            }

            if(IsMouseButtonUp(MOUSE_BUTTON_LEFT) && dot_dragging)
                dot_dragging = false;

            if(dot_dragging) {
                DrawLineEx(
                    (Vector2){ first_dot->circle.x, first_dot->circle.y },
                    cursor,
                    5,
                    BLUE
                );
            }

            c.a = 255;
            gui_draw_circle(board->dots[y][x].circle, c);
        }
    }
}