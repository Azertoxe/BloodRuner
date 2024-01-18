/*
** Bloodruner Project
*/

#include <SFML/Graphics.h>
#include <stdlib.h>
#include "../include/game.h"

static const int SCREEN_SIZE_H = 1080;
static const int SCREEN_SIZE_W = 850;

static void init_window(runer_t *runer)
{

}

void call_init_fuction(runer_t *runer)
{
    init_window(runer);
}

void game_loop(runer_t *runer)
{
    while (sfRenderWindow_isOpen(runer->window)) {
        
    }
}

int main(int ac, char **ab)
{
    runer_t runer;

    call_init_fuction(&runer);
    game_loop(&runer);
}