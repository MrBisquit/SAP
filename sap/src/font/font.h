#pragma once

#include "../sap.h"

/// @brief Loads a TTF font from a file on disk
/// @param font The font pointer
/// @param file The TTF file pointer
/// @param size The size of the font
/// @return `sap_err_t`
sap_err_t sap_font_load_file(sap_font_t* font, FILE* file, int size);

/// @brief Loads a TTF font from memory (embedded data)
/// @param font The font pointer
/// @param data The TTF font data in memory
/// @param size The size of the font
/// @return `sap_err_t`
sap_err_t sap_font_load_memory(sap_font_t* font, char data[], int size);

/// @brief Closes a TTF font
/// @param font The font pointer
void sap_font_close(sap_font_t* font);