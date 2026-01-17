#pragma once

#define GCAT_L1 "This project is a part of the SAP (Simple AI Projects) Project on GitHub by MrBisquit.\n"
#define GCAT_L2 "To view SAP, and the source code for this program, visit: https://github.com/MrBisquit/SAP\n\n"
#define GCAT_L3 "This project was built with Raylib.\n\n"
#define GCAT_L4 "The AI algorithm used in this game (Noughts And Crosses) is called \"" AI_NAME "\".\n"
#define GCAT_L5 "Thank you for playing the game!"

const char* game_credits_about_text = GCAT_L1 GCAT_L2 GCAT_L3 GCAT_L4 GCAT_L5;

static gui_textblock_t game_credits_title;
static gui_textblock_t game_credits_subtitle;

static gui_textblock_t game_credits_about;

static gui_button_t game_credits_back_button;