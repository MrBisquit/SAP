#pragma once

#include "sap.h"

typedef enum {
    SAP_ERROR_NONE                                      = 0x00000000,
    
    SAP_ERROR_SDL_INIT_ERROR                            = 0x00000100,
    SAP_ERROR_SDL_CREATE_WINDOW_AND_RENDERER_ERROR      = 0x00000101,

    SAP_ERROR_TTF_INIT_ERROR                            = 0x00000200,
    SAP_ERROR_TTF_LOAD_ERROR                            = 0x00000201,

    SAP_SDL_GENERIC                                     = 0xF0000000,
    SAP_SDL_MEMORY_RW_FROMCONSTMEMORY_ERROR             = 0xF0001000
} SAP_ERROR;

#define SAP_NO_ERROR (sap_err_t){ true, "SAP_NO_ERROR", SAP_ERROR_NONE };