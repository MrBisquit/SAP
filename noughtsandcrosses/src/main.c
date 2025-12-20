#include "main.h"

sap_err_t error;

int main(int argc, const char* argv[]) {
    sap_init_t init = sap_begin_init();

    sap_font_t RussoOne;
    sap_font_t VT323;

    error = sap_font_load_memory(&RussoOne, _RUSSOONE_REGULAR_TTF, sizeof(_RUSSOONE_REGULAR_TTF));
    //error = sap_font_load_memory(&VT323, _VT323_REGULAR_TTF, sizeof(_VT323_REGULAR_TTF));
    
    // Modify values
    //init.sap_object->resizable = true;
    //init.sap_object->target_fps = 60;

    sap_final_t final = sap_init(init);

    return final.return_code;
}