/*
** Bloodruner Project
*/

#include <SFML/Graphics.h>
#include <stdlib.h>
#include "../include/game.h"

void game_loop(runer_t *runer)
{
    while (sfRenderWindow_isOpen(runer->Window->window)) {
        
    }
}

int main(int ac, char **ab)
{
    runer_t runer;

    call_init_fuction(&runer);
    game_loop(&runter);
}