#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <time.h>

// Window properties
#define WINDOW_DEFAULT_WIDTH    800 * 2
#define WINDOW_DEFAULT_HEIGHT   450 * 2
#define WINDOW_TARGET_FPS       60

// Font sizes
#define FONT_TITLE              25
#define FONT_SUBTITLE           20
#define FONT_NORMAL             15
#define FONT_SMALL              10

// Rounding
#define ROUNDING_DEFAULT        0.5f
#define ROUNDING_POPUP          0.05f

#define COLORS_BACKGROUND_LIGHT { .r = 227, .g = 227, .b = 227, .a = 255 }
#define COLORS_BACKGROUND_DARK  { .r = 23,  .g = 23,  .b = 23,  .a = 255 }

typedef enum {
    THEME_LIGHT,
    THEME_DARK
} THEME;

static THEME gui_theme = THEME_DARK;

typedef struct {
    const char* key;
    Color light;
    Color dark;
} ThemeColor;

static const ThemeColor gui_theme_colors[8] = {
    {
        "BACKGROUND",
        { .r = 227, .g = 227, .b = 227, .a = 255 }, // #E3E3E3 (0xE3E3E3)
        { .r = 23,  .g = 23,  .b = 23,  .a = 255 }  // #171717 (0x171717)
    },
    {
        "TEXT",
        { .r = 27,  .g = 27,  .b = 27,  .a = 255 }, // #1B1B1B (0x1B1B1B)
        { .r = 255, .g = 255, .b = 255, .a = 255 }  // #FFFFFF (0xFFFFFF)
    },
    {
        "OVERLAY",
        { .r =   0, .g =   0, .b =   0, .a = 128 }, // #000000 (0x000000)
        { .r =   0, .g =   0, .b =   0, .a = 200 }, // #000000 (0x000000)
    },

    // Button
    {
        "BUTTON",
        { .r = 180, .g = 180, .b = 180, .a = 128 },  // #B4B4B4 (0xB4B4B4)
        { .r = 80,  .g = 80,  .b = 80,  .a = 128 }   // #505050 (0x505050)
    },
    {
        "BUTTON_HOVER",
        { .r = 180, .g = 180, .b = 180, .a = 200 },  // #B4B4B4 (0xB4B4B4)
        { .r = 80,  .g = 80,  .b = 80,  .a = 200 }   // #505050 (0x505050)
    },
    {
        "BUTTON_CLICKED",
        { .r = 180, .g = 180, .b = 180, .a = 240 },  // #B4B4B4 (0xB4B4B4)
        { .r = 80,  .g = 80,  .b = 80,  .a = 240 }   // #505050 (0x505050)
    },
    {
        "BUTTON_DISABLED",
        { .r = 180, .g = 180, .b = 180, .a = 80 },  // #B4B4B4 (0xB4B4B4)
        { .r = 80,  .g = 80,  .b = 80,  .a = 80 }   // #505050 (0x505050)
    },

    // Popups
    {
        "POPUP_BACKGROUND",
        { .r = 227, .g = 227, .b = 227, .a = 255 }, // #E3E3E3 (0xE3E3E3)
        { .r = 50,  .g = 50,  .b = 50,  .a = 255 }, // #323232 (0x323232)
    }
};

/// @brief Gets a colour from the current theme
/// @param name The name of the colour
/// @param theme The theme
/// @return The colour (Or #C824B1 (199, 36, 177) if it's not found)
static Color gui_get_color_theme(const char* name, THEME theme) {
    for(size_t i = 0; i < sizeof(gui_theme_colors) / sizeof(ThemeColor); i++) {
        if(strcmp(name, gui_theme_colors[i].key) == 0)
        {
            if(theme == THEME_LIGHT) {
                return gui_theme_colors[i].light;
            } else {
                return gui_theme_colors[i].dark;
            }
        }
    }
    return (Color) { .r = 199, .g = 36, .b = 177, .a = 255 }; // Neon purple (#C724B1)
}

/// @brief Gets a colour from the current theme
/// @param name The name of the colour
/// @return The colour (Or #C824B1 (199, 36, 177) if it's not found)
static Color gui_get_color(const char* name) {
    return gui_get_color_theme(name, gui_theme);
}

static void _gui_render_loop() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("No render loop", 10, 10, 15, RED);

    EndDrawing();
}

static void _gui_input_loop() {
    
}

static void (*gui_render_loop)() = &_gui_render_loop;
static void (*gui_input_loop)() = &_gui_input_loop;

static void gui_replace_loops(void (*render_loop)(), void (*input_loop)()) {
    gui_render_loop = render_loop;
    gui_input_loop = input_loop;
}

static void gui_init() {
    while(!WindowShouldClose()) {
        gui_input_loop();
        gui_render_loop();
    }
}

typedef struct RoundRectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
    float rounded;          // Rectangle roundness
} RoundRectangle;

typedef struct gui_text {
    const char* text;
    Color color;
    int fontSize;
} gui_text_t;

typedef struct gui_button {
    RoundRectangle rect;    // Rectangle information
    Color normal;           // Default color
    Color hover;            // Hovering color
    Color clicked;          // Clicking color
    Color disabled;         // Disabled color
    gui_text_t text;        // Button text
    bool enabled;           // Button enabled
} gui_button_t;

typedef struct gui_textblock {
    const char* text;
    Color color;
    int fontSize;
    Vector2 position;
} gui_textblock_t;

/// @brief Generates a text with the default colors
/// @param text The text
/// @param fontSize The font size
/// @return The text struct
static gui_text_t gui_generate_text(const char* text, int fontSize) {
    gui_text_t _text = {
        text,
        gui_get_color("TEXT"),
        fontSize
    };

    return _text;
}

/// @brief Generates a text with the default colors
/// @param text The text
/// @param fontSize The font size
/// @return The text struct
static gui_textblock_t gui_generate_textblock(const char* text, int fontSize, Vector2 position) {
    gui_textblock_t _text = {
        text,
        gui_get_color("TEXT"),
        fontSize,
        position
    };

    return _text;
}

/// @brief Generates a button with the default colors
/// @param rect The button rectangle
/// @param text The text inside the button
/// @return The button struct
static gui_button_t gui_generate_button(RoundRectangle rect, gui_text_t text) {
    gui_button_t button = {
        rect,
        .text = text,
        .normal = gui_get_color("BUTTON"),
        .hover = gui_get_color("BUTTON_HOVER"),
        .clicked = gui_get_color("BUTTON_CLICKED"),
        .disabled = gui_get_color("BUTTON_DISABLED"),
        .enabled = true
    };
    
    return button;
}

#define BTN gui_generate_button
#define TXT gui_generate_text
#define TXB gui_generate_textblock

/// @brief Seeds the random algorithm
static void gui_rand_seed(void) {
    srand(time(0));
}

/// @brief Generates a random number between 2 numbers
/// @note Run `gui_rand_seed` before running this
/// @param from From
/// @param to To
/// @return A number between the 2 numbes provided
static int gui_rand(int from, int to) {
    return (rand() % to) + from;
}

/// @brief Check if 2 rectangles overlap
/// @param a Rectangle A
/// @param b Rectangle B
/// @return `true` if the rectangles overlap, `false` if not
static bool gui_rect_overlap(Rectangle a, Rectangle b) {
    float a_x1 = a.x;
    float a_y1 = a.y;
    float a_x2 = a.x + a.width;
    float a_y2 = a.y + a.height;

    float b_x1 = b.x;
    float b_y1 = b.y;
    float b_x2 = b.x + b.width;
    float b_y2 = b.y + b.height;

    return a_x1 <= b_x2 && a_x2 >= b_x1 && a_y1 <= b_y2 && a_y2 >= b_y1;
}

/// @brief Checks if the point is in the rectangle
/// @param point The point
/// @param rect The rectangle
/// @return `true` if the point is inside the rectangle, `false` if not
static bool gui_point_in_rect(Vector2 point, Rectangle rect) {
    float px = point.x;
    float py = point.y;

    float x1 = rect.x;
    float y1 = rect.y;
    float x2 = rect.x + rect.width;
    float y2 = rect.y + rect.height;

    return px >= x1 && px <= x2 && py >= y1 && py <= y2;
}

/// @brief Converts a `RoundRectangle` to a `Rectangle`
/// @param rect The round rectangle
/// @return The normal rectangle
static Rectangle gui_round_to_rect(RoundRectangle rect) {
    Rectangle _rect = {
        rect.x,
        rect.y,
        rect.width,
        rect.height
    };

    return _rect;
}

/// @brief Converts a `Rectangle` to a `RoundRectangle`
/// @note This uses `ROUNDING_DEFAULT` for the rounding value
/// @param rect The normal rectangle
/// @return The round rectangle
static RoundRectangle gui_rect_to_round(Rectangle rect) {
    RoundRectangle _rect = {
        rect.x,
        rect.y,
        rect.width,
        rect.height,
        ROUNDING_DEFAULT
    };

    return _rect;
}

/// @brief Checks if a button is being pressed
/// @param button The button object
/// @param mouse_button The mouse button to detect
/// @param point The current cursor position
/// @return Whether or not the button is being pressed
static bool gui_button_pressed(gui_button_t button, int mouse_button, Vector2 point) {
    if(gui_point_in_rect(point, gui_round_to_rect(button.rect)) &&
        IsMouseButtonPressed(mouse_button) && button.enabled)
        return true;
    else
        return false;
}

/// @brief Draws text
/// @param text The text to draw
/// @param posX The X position
/// @param posY The Y position
/// @param fontSize The font size
/// @param color The color of the text to draw
static void gui_draw_text(const char* text, int posX, int posY, int fontSize, Color color) {
    // This seems pointless now, but I plan on adding support for fonts later
    DrawText(text, posX, posY, fontSize, color);
}

/// @brief Draws a textblock
/// @param text The textblock to draw
static void gui_draw_textblock(gui_textblock_t text) {
    // This seems pointless now, but I plan on adding support for fonts later
    DrawText(text.text, text.position.x, text.position.y, text.fontSize, text.color);
}

/// @brief Draws text at the center of a rectangle
/// @param text The text to draw
/// @param rect The rectangle to draw the text in
/// @param fontSize The font size
/// @param color The color of the text to draw
static void gui_draw_text_center(const char* text, Rectangle rect, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    int height = fontSize / 2;

    int x = (rect.x + (rect.width / 2)) - (width / 2);
    int y = (rect.y + (rect.height / 2)) - height;

    gui_draw_text(text, x, y, fontSize, color);
}

/// @brief Draws text at the center of a rectangle with an offset
/// @param text The text to draw
/// @param rect The rectangle to draw the text in
/// @param offset The offset
/// @param fontSize The font size
/// @param color The color of the text to draw
static void gui_draw_text_center_offset(const char* text, Rectangle rect, Vector2 offset, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    int height = fontSize / 2;

    int x = (rect.x + (rect.width / 2)) - (width / 2) + offset.x;
    int y = (rect.y + (rect.height / 2)) - height + offset.y;

    gui_draw_text(text, x, y, fontSize, color);
}

static void gui_draw_rectangle_round(RoundRectangle rect, Color color) {
    DrawRectangleRounded(gui_round_to_rect(rect), rect.rounded, 4 + (4 * rect.rounded), color);
}

static void gui_draw_rectangle_round_outline(RoundRectangle rect, Color color) {
    DrawRectangleRoundedLines(gui_round_to_rect(rect), rect.rounded, 4 + (4 * rect.rounded), color);
}

/// @brief Draws a button
/// @param button The button to draw
/// @param blocked Whether input is blocked (Overrides states)
static void gui_draw_button(gui_button_t button, bool blocked) {
    Vector2 cursor = GetMousePosition();
    Rectangle rect = gui_round_to_rect(button.rect);

    if(blocked) goto normal;

    if(!button.enabled) {
        gui_draw_rectangle_round(button.rect, button.disabled);
    } else if(gui_point_in_rect(cursor, rect)) {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            gui_draw_rectangle_round(button.rect, button.clicked);
            goto end;
        }
        gui_draw_rectangle_round(button.rect, button.hover);
        goto end;
    } else {
        normal:
        gui_draw_rectangle_round(button.rect, button.normal);
        goto end;
    }

    end:
    gui_draw_text_center(button.text.text, rect, button.text.fontSize, button.text.color);
}