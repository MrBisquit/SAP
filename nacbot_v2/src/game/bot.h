#pragma once

#include <src/GameUI.h>
#include "game.h"

enum game_bot_v2_weights {
    // These are the weights the whole board starts with
    game_bot_v2_weight_start = 0,
    // These are the weights deducted from the winning
    // pattern when the player places on a winning pattern
    game_bot_v2_weight_winning_pattern = 1
};

typedef struct game_bot_v2_option {
    int x;
    int y;
    float value;
} game_bot_v2_option_t;

typedef struct game_bot_v2 {
    float** predictions;
    int size;

    float* weights;
    int total_weights;

    game_bot_v2_option_t* options;
    int total_options;
} game_bot_v2_t;

void game_bot_v2_init_bot(game_bot_v2_t* bot, int size);