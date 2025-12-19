#pragma once

// Include headers
#ifndef SAP_NO_RAYLIB
    #include <raylib.h>
#endif
#ifndef SAP_NO_STDINT
    #include <stdint.h>
#endif

#include "types.h"
#include "error_table.h"

extern sap_t __sap_obj;

/// @brief Begins the initialisation
/// @return A `sap_init_t` object
sap_init_t sap_begin_init();

/// @brief Finalises the initialisation and begins loops
/// @warning Nothing after this will run unless the window exits
/// @return A `sap_final_t` object
sap_final_t sap_init();