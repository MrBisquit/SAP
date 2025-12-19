#pragma once

#include "sap.h"

typedef enum {
    SAP_ERROR_NONE = 0x00000000
} SAP_ERROR;

#define SAP_NO_ERROR (sap_err_t){ true, "SAP_NO_ERROR", SAP_ERROR_NONE }