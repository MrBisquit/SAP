#include <stdio.h>
#include <memory.h>
#include "../main.h"
#include "game.h"
#include "../../../utils/src/GameUI.h"

// Tons of global values because it's just easiser
static int board_size = -1;
static int board_size_squares = 0;

bool use_3_options = true;

BOARD_PLACE winner = BOARD_PLACE_BLANK;
BOARD_PLACE turn = BOARD_PLACE_X;

int board_w = -1;
int board_h = -1;

int dialog_type = -1;
bool dialog_visible = false;
bool dialog_cover_entire = false;

bool game_disable_header_buttons = false;

game_board_t board;

void game_start(int size) {
    board_size = size;
    game_start_play();
    game_enter_dialog(2);

    // Set the board sizes
    if(size == BOARD_3X3) {
        board_w = 250;
        board_h = 250;
        board_size_squares = 3;
    } else if(size == BOARD_4X4) {
        board_w = 350;
        board_h = 350;
        board_size_squares = 4;
    } else if(size == BOARD_5X5) {
        board_w = 450;
        board_h = 450;
        board_size_squares = 5;
    }
}

void game_generate_board(int size) {
    int boxes = 0;

    // These are for setting the board width and height in points
    if(size == BOARD_3X3) {
        boxes = 3;
    } else if(size == BOARD_4X4) {
        boxes = 4;
    } else if(size == BOARD_5X5) {
        boxes = 5;
    }

    gui_rand_seed();

    board.places = calloc(boxes, sizeof(BOARD_PLACE*));
    board.size = boxes;

    for(int y = 0; y < boxes; y++)
        board.places[y] = calloc(boxes, sizeof(BOARD_PLACE));

    board.bot = malloc(sizeof(game_bot_t));
    board.bot->predictions = calloc(boxes, sizeof(float*));
    board.bot->size = boxes;

    for(int y = 0; y < boxes; y++)
        board.bot->predictions[y] = calloc(boxes, sizeof(float));

    for(int y = 0; y < boxes; y++) {
        for(int x = 0; x < boxes; x++) {
            board.places[y][x] = BOARD_PLACE_BLANK;
        }
    }

    game_generate_lines(&board);
}

void game_generate_lines(game_board_t* board) {
    int total_down = board->size;
    int total_across = board->size;
    int total_diagonal = 2;
    int total = total_down + total_across + total_diagonal;

    board->total_lines = total;
    board->lines = calloc(total, sizeof(Point*));
    
    for(int i = 0; i < total; i++) {
        board->lines[i] = calloc(board->size, sizeof(Point));
    }

    int i = 0;
    
    /*
        i = Index of the line (Not important)
        j = Position on the point
        k = Individual point
    */

    // Down
    for(int j = 0; j < total_down; j++) {
        for(int k = 0; k < board->size; k++) {
            board->lines[i][k] = (Point) { .x = j, .y = k };
            //printf("(%d) Down X %d Y %d\n", i, j, k);
        }

        i++;
    }

    // Across
    for(int j = 0; j < total_across; j++) {
        for(int k = 0; k < board->size; k++) {
            board->lines[i][k] = (Point) { .x = k, .y = j };
            //printf("(%d) Across X %d Y %d\n", i, k, j);
        }

        i++;
    }

    // Diagonal (TL --> BR)
    for(int j = 0; j < board->size; j++) {
        board->lines[i][j] = (Point) { .x = j, .y = j };
        //printf("(%d) TL->BR X %d Y %d\n", i, j, j);
    }
    i++;

    // Diagonal (TR --> BL)
    for(int j = 0; j < board->size; j++) {
        board->lines[i][j] = (Point) { .x = board->size - j - 1, .y = j };
        //printf("(%d) TR->BL X %d Y %d\n", i, board->size - j - 1, j);
    }

    // Check
    //printf("\nChecking...\n");
    /*for(int j = 0; j < total; j++) {
        printf("(%d)\n", j);
        for(int k = 0; k < board->size; k++) {
            printf("X %d Y %d\n", board->lines[j][k].x, board->lines[j][k].y);
        }
    }*/
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
    if(turn == 0) {
        gui_draw_text(
            "X (You)",
            game_info_bounds.x + game_info_bounds.width - (20 * 5),
            game_info_board_turn.position.y,
            20,
            BLUE
        );
    } else {
        gui_draw_text(
            "O (Bot)",
            game_info_bounds.x + game_info_bounds.width - (20 * 5),
            game_info_board_turn.position.y,
            20,
            RED
        );
    }

    if(board.size > 0) {
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

    if(turn == BOARD_PLACE_O) {
        game_run_bot(&board, BOARD_PLACE_O);
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

    game_game_title = TXB("Noughts And Crosses", 25, (Vector2){
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

    if(board_size_squares > 0)
        game_buttons_generate(&board, game_game_inner_bounds);
}

void game_handle_resize(Rectangle old, Rectangle current) {
    game_init();
    game_enter_dialog(dialog_type);
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

        game_game_start_dialog.title = TXB("Noughts And Crosses", 25, (Vector2){
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
            game_init();

            game_enter_dialog(-1);
        }

        if(gui_button_pressed(game_game_reset_dialog.no, MOUSE_BUTTON_LEFT, cursor)) {
            game_enter_dialog(-1);
        }
    } else if(dialog_type == 2) {
        if(gui_button_pressed(game_game_start_dialog.start, MOUSE_BUTTON_LEFT, cursor)) {
            game_generate_board(board_size);
            game_init();

            game_enter_dialog(-1);
        }

        if(gui_button_pressed(game_game_start_dialog.cancel, MOUSE_BUTTON_LEFT, cursor)) {
            game_start_menu();
        }
    }
}

BOARD_PLACE game_check_winner(game_board_t* board) {
    //printf("\nChecking for winner...\n");
    int total_empty = 0;
    for(int y = 0; y < board->size; y++) {
        for(int x = 0; x < board->size; x++) {
            if(board->places[y][x] == BOARD_PLACE_BLANK)
                total_empty++;
        }
    }

    if(total_empty == 0)
        return BOARD_PLACE_TIE;

    BOARD_PLACE player = BOARD_PLACE_BLANK;

    /*for(int i = 0; i < board->total_lines; i++) {
        for(int j = 0; j < board->size; j++) {
            printf("X %d Y %d\n", board->lines[i][j].x, board->lines[i][j].x);
        }
    }*/

    for(int i = 0; i < board->total_lines; i++) {
        player = board->places[board->lines[i][0].y][board->lines[i][0].x];
        bool win = true;
        for(int j = 0; j < board->size; j++) {
            BOARD_PLACE now_player = board->places[board->lines[i][j].y][board->lines[i][j].x];
            if(player != now_player) win = false;
            player = now_player;

            //printf("X %d Y %d V %d\n", board->lines[i][j].x, board->lines[i][j].y, now_player);
        }
        if(win == true && player != BOARD_PLACE_BLANK) return player;
    }

    return BOARD_PLACE_BLANK;
}

void game_buttons_generate(game_board_t* board, Rectangle bounds) {
    Vector2 cursor = GetMousePosition();

    int each_w = board_w / board_size_squares;
    int each_h = board_h / board_size_squares;

    int offset_x = bounds.x;
    int offset_y = bounds.y;

    int stop_x = bounds.x + bounds.width;
    int stop_y = bounds.y + bounds.height;

    float border = 3;

    if(board->board_buttons)
        free(board->board_buttons);
    board->board_buttons = calloc(board_size_squares * board_size_squares, sizeof(game_board_button_t));
    board->buttons = board_size_squares * board_size_squares;
    int i = 0;
    
    for(int y = 0; y < board_size_squares; y++) {
        for(int x = 0; x < board_size_squares; x++) {
            Color normal = gui_get_color("BUTTON");
            Color hover = gui_get_color("BUTTON_HOVER");
            Color clicked = gui_get_color("BUTTON_CLICKED");
            Color disabled = gui_get_color("BUTTON_DISABLED");
            gui_text_t text = TXT("", 30);
            bool enabled = true;
            if(board->places[y][x] == BOARD_PLACE_BLANK) {
                normal = gui_get_color("BACKGROUND");
            } else {
                enabled = false;
                if(board->places[y][x] == BOARD_PLACE_X) {
                    text = TXT("X", 30);
                    disabled = BLUE;
                    disabled.a = 128;
                } else {
                    text = TXT("O", 30);
                    disabled = RED;
                    disabled.a = 128;
                } 
            }

            board->board_buttons[i] = (game_board_button_t){
                (gui_button_t) {
                    (RoundRectangle) {
                        offset_x + (each_w * x),
                        offset_y + (each_h * y),
                        each_w,
                        each_h,
                        0
                    },
                    normal, hover, clicked, disabled,
                    text,
                    enabled
                },
                x, y
            };

            i++;
        }
    }
}

void game_board_render(game_board_t* board, Rectangle bounds) {
    Vector2 cursor = GetMousePosition();

    int each_w = board_w / board_size_squares;
    int each_h = board_h / board_size_squares;

    int offset_x = bounds.x;
    int offset_y = bounds.y;

    int stop_x = bounds.x + bounds.width;
    int stop_y = bounds.y + bounds.height;

    float border = 3;
    Color c = gui_get_color("TEXT");

    for(int i = 0; i < board->buttons; i++) {
        gui_draw_button(board->board_buttons[i].button, dialog_visible);
        if(gui_button_pressed(board->board_buttons[i].button, MOUSE_BUTTON_LEFT, cursor) &&
            turn == BOARD_PLACE_X) { 
                game_place(board, board->board_buttons[i].x, board->board_buttons[i].y, BOARD_PLACE_X);
        }
    }

    for(int y = 0; y < board_size_squares; y++) {
        if(y > 0 && y < board_size_squares) {
            DrawLineEx(
                (Vector2) {
                    offset_x,
                    offset_y + (each_h * y)
                },
                (Vector2) {
                    //offset_x + (each_w * board_size_squares),
                    stop_x,
                    offset_y + (each_h * y)
                },
                border,
                c
            );
        }

        for(int x = 0; x < board_size_squares; x++) {
            if(x > 0 && x < board_size_squares) {
                DrawLineEx(
                    (Vector2) {
                        offset_x + (each_w * x),
                        offset_y
                    },
                    (Vector2) {
                        offset_x + (each_w * x),
                        stop_y
                        //offset_y + (each_h * board_size_squares)
                    },
                    border,
                    c
                );
            }
        }
    }
}

void game_place(game_board_t* board, int x, int y, BOARD_PLACE player) {
    if(board->places[y][x] != BOARD_PLACE_BLANK)
        return;

    board->places[y][x] = player;
    printf("Set X %d Y %d to %d\n", x, y, player);

    game_buttons_generate(board, game_game_inner_bounds);

    turn = player == BOARD_PLACE_X ? BOARD_PLACE_O : BOARD_PLACE_X;

    BOARD_PLACE winner = game_check_winner(board);
    if(winner != BOARD_PLACE_BLANK) {
        game_start_menu();
    }
}

void game_run_bot(game_board_t* board, BOARD_PLACE as) {
    printf("RUN Board:\n");
    for(int y = 0; y < board->size; y++) {
        for(int x = 0; x < board->size; x++) {
            if(board->places[y][x] == BOARD_PLACE_BLANK)
                printf("_ ");
            else if(board->places[y][x] == BOARD_PLACE_X)
                printf("X ");
            else if(board->places[y][x] == BOARD_PLACE_O)
                printf("O ");
            else
                printf("? ");
        }
        printf("\n");
    }

    game_bot_run_t run = game_bot_run(board, as);
    game_place(board, run.chosen_option.x, run.chosen_option.y, as);
}

Point game_bot_check_blocks(game_board_t* board, BOARD_PLACE as) {
    BOARD_PLACE op = as == BOARD_PLACE_X ? BOARD_PLACE_O : BOARD_PLACE_X;
    /*for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t used = 0;
        Point unused = { -1, -1 };
        for (uint8_t j = 0; j < 3; j++)
        {
            if(board->places[pos[i][j][0]][pos[i][j][1]] == op) used++;
            else unused = (Point){ pos[i][j][0], pos[i][j][1] };
        }
        if(used == 2 && board->places[unused.y][unused.x] == BOARD_PLACE_BLANK) return unused;
    }*/

    for(int i = 0; i < board->total_lines; i++) {
        uint8_t used = 0;
        Point unused = { -1, -1 };
        for(int j = 0; j < board->size; j++) {
            if(board->places[board->lines[i][j].x][board->lines[i][j].y] == op) used++;
            else unused = (Point){ board->lines[i][j].x, board->lines[i][j].y };
        }
        if(used == 2 && board->places[unused.y][unused.x] == BOARD_PLACE_BLANK) return unused;
    }
    return (Point) { -1, -1 };
}

Point game_bot_check_win(game_board_t* board, BOARD_PLACE as) {
    /*for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t used = 0;
        Point unused;
        for (uint8_t j = 0; j < 3; j++)
        {
            if(board->places[pos[i][j][0]][pos[i][j][1]] == as) used++;
            else unused = (Point){ pos[i][j][0], pos[i][j][1] };
        }
        if(used == 2 && board->places[unused.y][unused.x] == BOARD_PLACE_BLANK) return unused;
    }*/

    for(int i = 0; i < board->total_lines; i++) {
        uint8_t used = 0;
        Point unused = { -1, -1 };
        for(int j = 0; j < board->size; j++) {
            if(board->places[board->lines[i][j].x][board->lines[i][j].y] == as) used++;
            else unused = (Point){ board->lines[i][j].x, board->lines[i][j].y };
        }
        if(used == 2 && board->places[unused.y][unused.x] == BOARD_PLACE_BLANK) return unused;
    }
    return (Point){ -1, -1 };
}

game_bot_run_t game_bot_run(game_board_t* board, BOARD_PLACE as) {
    int size = board->size;
    game_bot_run_t run = {
        NULL, size,

        0,
    
        NULL, NULL, 0,

        NULL, false, false
    };
    run.options = calloc(3, sizeof(Point));
    run.options_values = calloc(3, sizeof(float));
    run.total_options = 3;

    for(int i = 0; i < run.total_options; i++) {
        run.options[i].x = -1;
        run.options[i].y = -1;
        run.options_values[i] = -1;
    }
    
    // Options go right (good) --> left (bad)
    // If 3 options mode is turned off, it will just use the leftmost option,
    // if not, it will simply pick one randomly from the 3 options

    Point win = game_bot_check_win(board, as);
    Point block = game_bot_check_blocks(board, as);

    // However, if a win or block is available, it will just completely
    // ignore any next steps, and immediately go for that option

    if(win.x != -1 && win.y != -1) {
        run.chosen_option = win;
        run.had_immediate_win = true;
        return run;
    }

    if(block.x != -1 && block.y != -1) {
        run.chosen_option = block;
        run.had_immediate_block = true;
        return run;
    }

    run.chosen_option = (Point){ 0, 0 };

    run.predictions = calloc(board->size, sizeof(float*));
    for(int i = 0; i < board->size; i++)
        run.predictions[i] = calloc(board->size, sizeof(float));

    game_bot_board_t** bot = calloc(board->size, sizeof(game_bot_board_t*));
    for(int i = 0; i < board->size; i++)
        bot[i] = calloc(board->size, sizeof(game_bot_board_t));

    for(int y = 0; y < board->size; y++) {
        for(int x = 0; x < board->size; x++) {
            if(board->places[y][x] == BOARD_PLACE_BLANK) {
                printf(".");

                bot[y][x].valid = true;
                game_bot_simulate(board, &bot[y][x], as, as, (Point){ x, y });
                run.total_runs += bot[y][x].total;

                int w = bot[y][x].wins;
                int l = bot[y][x].losses;
                int t = bot[y][x].ties;

                float fw = (float)w;
                float fl = (float)l;
                float ft = (float)t;

                run.predictions[y][x] = fw / (fw + fl + ft);
                //run.predictions[y][x] = (float)((float)w / ((float)w + (float)l));

                printf("W %d L %d T %d V %f\n", w, l, t,  run.predictions[y][x]);
            } else {
                bot[y][x].valid = false;
                run.predictions[y][x] = -1;

                printf("W %f L %f T %f\n", 0, 0, 0);
            }

            //printf("%f\t", bot[y][x]);
        }
        printf("\n");
    }

    free(bot);

    for(int y = 0; y < board->size; y++) {
        for(int x = 0; x < board->size; x++) {
            bool added = game_bot_add_option(board, &run, (Point){ x, y }, run.predictions[y][x]);

            //if(bot[y][x].valid)
                //printf("X %d Y %d:\t%f\n", x, y, run.predictions[y][x]);
            //else
                //printf("X %d Y %d:\t(i)\t%d\n", x, y, board->places[y][x]);
        }
    }

    int selected = gui_rand(0, run.total_options);
    printf("Selected X %d Y %d V %f\n", run.options[selected].x,
        run.options[selected].y, run.options_values[selected]);
    if(run.options_values[selected] == -1) {
        // No available option, so assume there are no options
        printf("No available options\n");
        for(int y = 0; y < board->size; y++) {
            for(int x = 0; x < board->size; x++) {
                if(board->places[y][x] == BOARD_PLACE_BLANK) {
                    run.chosen_option = (Point){ x, y };
                    return run;
                }
            }
        }
    } else {
        run.chosen_option = run.options[selected];
        return run;
    }

    return run;
}

void game_bot_simulate(game_board_t* board,
    game_bot_board_t* bot,
    BOARD_PLACE as,
    BOARD_PLACE active,
    Point start) {
    /// @bug The board data is being corrupted somewhere in this function
    // Copy active board
    game_board_t b = *board;
    //memcpy_s(&b, sizeof(game_board_t), board, sizeof(game_board_t));
    //memcpy(&b, board, sizeof(game_board_t));
    //game_utils_copy_board(board, &b);

    b.places[start.y][start.x] = active; // as;

    BOARD_PLACE winner = game_check_winner(&b);
    if(winner != BOARD_PLACE_BLANK) {
        if(as == active) {
            // If it's the bots move
            if(winner == as) bot->wins++;
            if(winner == BOARD_PLACE_TIE) bot->ties++;
            else bot->losses++;
        } else {
            // If it's the players move
            if(winner == as) bot->losses++;
            if(winner == BOARD_PLACE_TIE) bot->ties++;
            else bot->wins++;
        }

        bot->total++;

        // Reset
        b.places[start.y][start.x] = BOARD_PLACE_BLANK;
        return;
    }

    BOARD_PLACE next;
    // Switch turn
    if(active == BOARD_PLACE_X)
        next = BOARD_PLACE_O;
    else
        next = BOARD_PLACE_O;

    for(int y = 0; y < board->size; y++) {
        for(int x = 0; x < board->size; x++) {
            if(b.places[y][x] == BOARD_PLACE_BLANK) {
                game_bot_simulate(&b, bot, as, next, (Point){ x, y });
            }
        }
    }

    b.places[start.y][start.x] = BOARD_PLACE_BLANK;
    //game_utils_free_board(&b);
}

bool game_bot_add_option(game_board_t* board, game_bot_run_t* run, Point option, float option_value) {
    for(int i = 0; i < run->total_options; i++) {
        if(option_value > run->options_values[i]) {
            if(i == run->total_options) {
                run->options[i] = option;
                run->options_values[i] = option_value;
                
                return true;
            }
            else {
                // Push back and add
                for(int j = run->total_options; j > i; j--) {
                    run->options[j] = run->options[j - 1];
                    run->options[j] = run->options[j - 1];
                }
                run->options[i] = option;
                run->options_values[i] = option_value;

                return true;
            }
        }
    }
                
    return false;
}

void game_utils_copy_board(game_board_t* src, game_board_t* dest) {
    dest->total_lines = src->total_lines;
    //dest->buttons = src->buttons;
    dest->size = src->size;
    
    //dest->board_buttons = calloc(dest->buttons, sizeof(game_board_button_t));
    //for(int i = 0; i < dest->buttons; i++) {
    //    memcpy(&dest->board_buttons[i], &src->board_buttons[i], sizeof(game_board_button_t));
    //}

    // No point in not copying the pointers for this
    //memcpy(dest->lines, src->lines, sizeof(Point**));
    dest->lines = src->lines;
    //memcpy(dest->bot, src->bot, sizeof(game_bot_t));
    dest->bot = src->bot;

    dest->places = calloc(dest->size, sizeof(BOARD_PLACE*));
    for(int i = 0; i < dest->size; i++) {
        dest->places[i] = calloc(dest->size, sizeof(BOARD_PLACE));
        for(int j = 0; j < dest->size; j++) {
            dest->places[i][j] = src->places[i][j];
        }
    }
}

void game_utils_free_board(game_board_t* board) {
    for(int i = 0; i < board->size; i++) {
        free(board->places[i]);
    }
    free(board->places);
}