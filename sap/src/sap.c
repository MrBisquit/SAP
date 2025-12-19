#include "sap.h"

sap_t __sap_obj;

sap_init_t sap_begin_init() {
    sap_init_t init = {
        &__sap_obj,
        false
    };

    return init;
}

sap_final_t sap_init() {
    sap_final_t final = {
        &__sap_obj,
        0
    };

    return final;
}