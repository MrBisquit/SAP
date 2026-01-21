//#include "bot.h"
#include <stdio.h>
#include <memory.h>
#include "../main.h"
#include "game.h"

float game_bot_v2_weights_default[] = {
    0.5f,
    -0.5f
};
int game_bot_v2_weights_default_count = 2;

void game_bot_v2_init_bot(game_bot_v2_t* bot, int size) {
    bot->size = size;

    bot->predictions = calloc(bot->size, sizeof(float*));
    for(int i = 0; i < size; i++)
        bot->predictions[i] = calloc(bot->size, sizeof(float));

    bot->weights = game_bot_v2_weights_default;
    bot->total_weights = game_bot_v2_weights_default_count;

    bot->options = calloc(3, sizeof(game_bot_v2_option_t));
    bot->total_options = 3;
}