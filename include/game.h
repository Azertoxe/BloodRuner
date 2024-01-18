#ifndef RUNNER_H_
#define RUNNER_H_


#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Config.h>
#include <SFML/Network.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct runer_s {
    sfRenderWindow *window;
    sfEvent event;
    sfClock *clock;
    sfTime time;
    float seconds;
} runer_t;



#endif /* !RUNNER_H_ */
