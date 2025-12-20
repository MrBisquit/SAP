#include "../../sap/src/sap.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
    sap_init_t init = sap_begin_init();
    
    // Modify values
    //init.sap_object->resizable = true;
    //init.sap_object->target_fps = 60;

    sap_final_t final = sap_init(init);

    return final.return_code;
}