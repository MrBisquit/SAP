#include "main.h"
#include "game_options.h"
#include "../../utils/src/GameUI.h"
#include "game/game.h"

int selected = 0;

typedef struct game_game_option {
    gui_button_t* btn;
    gui_textblock_t* tb;
    gui_checkbox_t* cb;

    const char* name;
} game_game_options_t;

game_game_options_t opts[] = {
    {
        &game_game_options_normal_3x3_button,
        &game_game_options_normal_3x3_textblock,
        &game_game_options_normal_3x3_checkbox,

        "3x3"
    },
    {
        &game_game_options_normal_4x4_button,
        &game_game_options_normal_4x4_textblock,
        &game_game_options_normal_4x4_checkbox,

        "4x4"
    },
    {
        &game_game_options_normal_5x5_button,
        &game_game_options_normal_5x5_textblock,
        &game_game_options_normal_5x5_checkbox,

        "5x5"
    },
    {
        &game_game_options_normal_10x10_button,
        &game_game_options_normal_10x10_textblock,
        &game_game_options_normal_10x10_checkbox,

        "10x10"
    },
    {
        &game_game_options_normal_20x20_button,
        &game_game_options_normal_20x20_textblock,
        &game_game_options_normal_20x20_checkbox,

        "20x20"
    }
};

int total_opts = 5;

int page = 0;

void game_game_options_render_loop() {
    BeginDrawing();
    ClearBackground(gui_get_color("BACKGROUND"));

    if(show_fps) DrawFPS(GetRenderWidth() - 100, 10);

    SetMouseCursor(MOUSE_CURSOR_ARROW);

    gui_draw_textblock(game_game_options_title);
    gui_draw_textblock(game_game_options_subtitle);

    //gui_draw_button(game_game_options_normal_3x3, false);
    //gui_draw_button(game_game_options_normal_4x4, false);
    //gui_draw_button(game_game_options_normal_5x5, false);

    for(int i = 0; i < total_opts; i++) {
        gui_draw_button(*opts[i].btn, false);
        gui_draw_textblock(*opts[i].tb);
        gui_draw_checkbox(*opts[i].cb, false);
    }

    gui_draw_button(game_game_options_back_button, false);

    EndDrawing();
}

void game_game_options_input_loop() {
    Vector2 cursor = GetMousePosition();

    if(IsKeyPressed(KEY_F1)) {
        if(gui_theme == THEME_DARK)
            gui_theme = THEME_LIGHT;
        else
            gui_theme = THEME_DARK;
        
        // Change all of the UI, since it's not dynamic
        game_game_init();
    }

    if(gui_button_pressed(game_game_options_back_button, MOUSE_BUTTON_LEFT, cursor)) {
        game_start_menu();
    }

    /*if(gui_button_pressed(game_game_options_normal_3x3, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_3X3);
    }

    if(gui_button_pressed(game_game_options_normal_4x4, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_4X4);
    }

    if(gui_button_pressed(game_game_options_normal_5x5, MOUSE_BUTTON_LEFT, cursor)) {
        game_start(BOARD_5X5);
    }*/

    for(int i = 0; i < total_opts; i++) {
        if(gui_button_pressed(*opts[i].btn, MOUSE_BUTTON_LEFT, cursor)) {
            selected = i;
            game_game_options_init();
        }
    }
}

void game_game_options_init() {
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

    game_game_options_title = TXB("Noughts And Crosses: Game Options", 50, (Vector2){
        35 + x_offset,
        35 + y_offset
    });

    game_game_options_subtitle = TXB("A SAP Project - Made by William Dawson", 15, (Vector2){
        35 + x_offset,
        35 + 50 + 10 + y_offset
    });

    /*game_game_options_normal_3x3 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 0) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("3x3", 25));

    game_game_options_normal_4x4 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 1) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("4x4", 25));

    game_game_options_normal_5x5 = BTN((RoundRectangle){
        35 + (((50 * 7) + 10) * 2) + x_offset,
        35 + 50 + 10 + (15 + 10) + y_offset,
        50 * 7,
        120,
        0.2
    }, TXT("5x5", 25));*/

    int col = 0;
    int row = 0;
    for(int i = 0; i < total_opts; i++) {
        int x = 35 + x_offset;
        int y = 35 + 50 + 10 + 15 + 10 + y_offset;

        x += (col * (50 * 4)) + (col * 10);
        y += (row * 40) + (row * 10);

        *opts[i].btn = BTN((RoundRectangle){
            x,
            y,
            50 * 4,
            40,
            0.5
        }, TXT("", 0));
        opts[i].btn->hover = gui_get_color("BUTTON"); //opts[i].btn.normal;
        opts[i].btn->normal = gui_get_color("BACKGROUND");

        *opts[i].tb = TXB(opts[i].name, 20, (Vector2){
            opts[i].btn->rect.x + 12.5,
            opts[i].btn->rect.y + 12.5
        });

        *opts[i].cb = CHK((RoundRectangle){
            opts[i].btn->rect.x + opts[i].btn->rect.width - 30,
            opts[i].btn->rect.y + 10,
            20,
            20,
            0.5
        }, true);
        opts[i].cb->checked = selected == i;

        col++;
        if(col == 4) {
            row++;
            col = 0;
        }
    }

    /*game_game_options_normal_3x3_button = BTN((RoundRectangle){
        35 + x_offset,
        35 + 50 + 10 + 15 + 10 + y_offset,
        50 * 14,
        40,
        0.5
    }, TXT("", 0));
    game_game_options_normal_3x3_button.hover = game_game_options_normal_3x3_button.normal;
    game_game_options_normal_3x3_button.normal = gui_get_color("BACKGROUND");
    
    game_game_options_normal_3x3_textblock = TXB("Dark theme", 20, (Vector2){
        game_game_options_normal_3x3_button.rect.x + 12.5,
        game_game_options_normal_3x3_button.rect.y + 12.5
    });

    game_game_options_normal_3x3_checkbox = CHK((RoundRectangle){
        game_options_dark_theme_button.rect.x + game_game_options_normal_3x3_button.rect.width - 30,
        game_options_dark_theme_button.rect.y + 10,
        20,
        20,
        0.5
    }, true);
    game_game_options_normal_3x3_checkbox.checked = gui_theme == THEME_DARK;*/

    game_game_options_back_button = BTN((RoundRectangle){
        35 + x_offset,
        GetRenderHeight() - y_offset - (35 * 2),
        50 * 14,
        40,
        0.5
    }, TXT("Back to main menu", 25));
}

void game_game_options_handle_resize(Rectangle old, Rectangle current) {
    game_game_options_init();
}