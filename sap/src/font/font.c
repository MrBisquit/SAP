#include "font.h"

sap_err_t sap_font_load_file(sap_font_t* font, FILE* file, int size) {
    sap_err_t err = SAP_NO_ERROR;

    TTF_Font* _font = TTF_OpenFont(file, size);
    if(!_font) {
        err.error_code = SAP_ERROR_TTF_LOAD_ERROR;
        err.error = "Could not open TTF font";
        err.success = false;

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load TTF font: %s (%d)",
            err.error, err.error_code);
        return err;
    }

    memcpy(font->font, _font, sizeof(_font));

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loaded TTF font successfully");

    return err;
}

sap_err_t sap_font_load_memory(sap_font_t* font, char data[], int size) {
    sap_err_t err = SAP_NO_ERROR;

    SDL_RWops* rw = SDL_RWFromConstMem(data, size);
    if(!rw) {
        err.error_code = SAP_SDL_MEMORY_RW_FROMCONSTMEMORY_ERROR;
        err.error = "SDL_RWFromConstMem failure";
        err.success = false;

        sap_destroy(); // Critical failure

        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to load TTF font: %s (%d)",
            err.error, err.error_code);
        return err;
    }

    TTF_Font* _font = TTF_OpenFontRW(rw, 1, 24);
    if(!font) {
        err.error_code = SAP_ERROR_TTF_LOAD_ERROR;
        err.error = "Could not open TTF font";
        err.success = false;

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load TTF font: %s (%d)",
            err.error, err.error_code);
        return err;
    }

    memcpy(font->font, _font, sizeof(_font));

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loaded TTF font successfully");

    return err;
}

void sap_font_close(sap_font_t* font) {
    TTF_CloseFont(font->font);
}