#pragma once

#include "sap.h"

typedef struct sap_size {
    float width;
    float height;
} sap_size_t;

/// @brief This is basically the main object
typedef struct sap {
    bool resizable;

    int target_fps;
    sap_size_t window_size;
    sap_size_t window_min_size;
    sap_size_t window_max_size;

    char* window_title;
} sap_t;

typedef struct sap_err {
    bool success;
    const char* error;
    uint64_t error_code;
} sap_err_t;

/// @brief This struct is returned by sap_init_begin
typedef struct sap_init {
    bool success;
    sap_t* sap_object;
} sap_init_t;

/// @brief This struct is returned by sap_init
typedef struct sap_final {
    sap_t* sap_object;
    int return_code;

    sap_err_t error;
} sap_final_t;