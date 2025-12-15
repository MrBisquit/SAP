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