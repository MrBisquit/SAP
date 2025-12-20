#pragma once

// Include headers
#ifndef SAP_NO_RAYLIB
    //#include <raylib.h>
#endif
#ifndef SAP_NO_SDL2
    #include <SDL.h>
    #include <SDL2/SDL_ttf.h>
#endif
#ifndef SAP_NO_STD
    #include <stdint.h>
    #include <stdbool.h>
#endif

#include "types.h"
#include "error_table.h"

extern sap_t __sap_obj;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Surface* surface;
extern SDL_Event event;

/// @brief Begins the initialisation
/// @return A `sap_init_t` object
sap_init_t sap_begin_init();

/// @brief Finalises the initialisation and begins loops
/// @warning Nothing after this will run unless the window exits
/// @return A `sap_final_t` object
sap_final_t sap_init(sap_init_t init);