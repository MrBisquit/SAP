#include "sap.h"

sap_t __sap_obj;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Event event;

sap_init_t sap_begin_init() {
    __sap_obj = (sap_t){
        true,

        60,
        (sap_size_t){ 800 * 2, 450 * 2 },
        (sap_size_t){ 800 * 2, 450 * 2 }
    };

    sap_init_t init = {
        &__sap_obj,
        false
    };

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialized SDL successfully");
        init.success = true;
    }

    return init;
}

sap_final_t sap_init(sap_init_t init) {
    sap_final_t final = {
        &__sap_obj,
        0
    };

    if(SDL_CreateWindowAndRenderer(__sap_obj.window_size.width, __sap_obj.window_size.height, __sap_obj.resizable ? SDL_WINDOW_RESIZABLE : NULL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        final.return_code = 3;
        goto end;
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Created window and renderer successfully");
    }

    while(true) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // Test rectangle
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_FRect rect = {
            .x = 10,
            .y = 10,
            .w = 100,
            .h = 100
        };
        SDL_RenderDrawRectF(renderer, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Break in poll loop (Likely caused by SDL_QUIT event)");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Successfully destroyed window and renderer");

    SDL_Quit();

end:
    return final;
}