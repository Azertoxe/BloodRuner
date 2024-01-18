#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400
#define GROUND_HEIGHT 50
#define DINO_SIZE 50
#define CACTUS_SIZE 30
#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 20

typedef struct {
    float x;
    float y;
    float velocity;
    sfBool isJumping;
    sfBool isOnPlatform;
} Dino;

typedef struct {
    float x;
    float y;
} Cactus;

typedef struct {
    float x;
    float y;
} Platform;

void updateDino(Dino *dino, float deltaTime) {
    if (dino->isJumping) {
        dino->y += dino->velocity * deltaTime;
        dino->velocity += 1000.0f * deltaTime;

        if (dino->y >= WINDOW_HEIGHT - GROUND_HEIGHT - DINO_SIZE) {
            dino->y = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_SIZE;
            dino->isJumping = sfFalse;
        }
    } else if (!dino->isOnPlatform) {
        // Descendre automatiquement si le dinosaure n'est pas sur une plateforme
        dino->y += 400.0f * deltaTime;  // Ajustez la vitesse de descente selon vos besoins
    }
}

sfBool checkCollision(Dino *dino, Platform *platform) {
    return dino->x < platform->x + PLATFORM_WIDTH &&
           dino->x + DINO_SIZE > platform->x &&
           dino->y + DINO_SIZE > platform->y &&
           dino->y < platform->y + PLATFORM_HEIGHT;
}

sfBool checkGroundCollision(Dino *dino) {
    return dino->y + DINO_SIZE >= WINDOW_HEIGHT - GROUND_HEIGHT;
}

Platform createRandomPlatform() {
    Platform platform;
    platform.x = WINDOW_WIDTH;
    platform.y = WINDOW_HEIGHT - GROUND_HEIGHT - (rand() % 100 + 20); // Random height between 20 and 120
    return platform;
}

Cactus createRandomCactus() {
    Cactus cactus;
    cactus.x = WINDOW_WIDTH;
    cactus.y = WINDOW_HEIGHT - GROUND_HEIGHT - CACTUS_SIZE;
    return cactus;
}

void drawPlatform(sfRenderWindow *window, Platform *platform) {
    sfRectangleShape *platformShape = sfRectangleShape_create();
    sfRectangleShape_setSize(platformShape, (sfVector2f){PLATFORM_WIDTH, PLATFORM_HEIGHT});
    sfRectangleShape_setFillColor(platformShape, sfColor_fromRGB(100, 100, 100));
    sfRectangleShape_setPosition(platformShape, (sfVector2f){platform->x, platform->y});
    sfRenderWindow_drawRectangleShape(window, platformShape, NULL);
    sfRectangleShape_destroy(platformShape);
}

void drawGame(sfRenderWindow *window, Dino *dino, Platform *platform, Cactus *cactus, int score) {
    sfRenderWindow_clear(window, sfBlack);

    // Dessiner le sol
    sfRectangleShape *ground = sfRectangleShape_create();
    sfRectangleShape_setSize(ground, (sfVector2f){WINDOW_WIDTH, GROUND_HEIGHT});
    sfRectangleShape_setFillColor(ground, sfColor_fromRGB(150, 150, 150));
    sfRectangleShape_setPosition(ground, (sfVector2f){0, WINDOW_HEIGHT - GROUND_HEIGHT});
    sfRenderWindow_drawRectangleShape(window, ground, NULL);
    sfRectangleShape_destroy(ground);

    // Dessiner le dinosaure
    sfRectangleShape *dinoShape = sfRectangleShape_create();
    sfRectangleShape_setSize(dinoShape, (sfVector2f){DINO_SIZE, DINO_SIZE});
    sfRectangleShape_setFillColor(dinoShape, sfWhite);
    sfRectangleShape_setPosition(dinoShape, (sfVector2f){dino->x, dino->y});
    sfRenderWindow_drawRectangleShape(window, dinoShape, NULL);
    sfRectangleShape_destroy(dinoShape);

    // Dessiner la plateforme
    drawPlatform(window, platform);

    // Dessiner le cactus
    sfRectangleShape *cactusShape = sfRectangleShape_create();
    sfRectangleShape_setSize(cactusShape, (sfVector2f){CACTUS_SIZE, CACTUS_SIZE});
    sfRectangleShape_setFillColor(cactusShape, sfGreen);
    sfRectangleShape_setPosition(cactusShape, (sfVector2f){cactus->x, cactus->y});
    sfRenderWindow_drawRectangleShape(window, cactusShape, NULL);
    sfRectangleShape_destroy(cactusShape);

    // Afficher le score en haut de la fenêtre
    sfFont *font = sfFont_createFromFile("arial.ttf");  // Changez le chemin vers le fichier de police si nécessaire
    sfText *scoreText = sfText_create();
    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", score);
    sfText_setString(scoreText, scoreStr);
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 20);
    sfText_setFillColor(scoreText, sfWhite);
    sfText_setPosition(scoreText, (sfVector2f){10, 10});
    sfRenderWindow_drawText(window, scoreText, NULL);
    sfText_destroy(scoreText);

    sfRenderWindow_display(window);
    sfFont_destroy(font);
}

void handleEvents(sfRenderWindow *window, Dino *dino) {
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed) {
            sfRenderWindow_close(window);
        }

        if (event.type == sfEvtKeyPressed && event.key.code == sfKeySpace && !dino->isJumping) {
            dino->isJumping = sfTrue;
            dino->velocity = -600.0f;
        }
    }
}

void initializeGame(sfRenderWindow **window, Dino *dino, Platform *platform, Cactus *cactus, sfClock **clock) {
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    *window = sfRenderWindow_create(mode, "Dino Game", sfResize | sfClose, NULL);

    if (!(*window)) {
        exit(EXIT_FAILURE);
    }

    sfRenderWindow_setFramerateLimit(*window, 60);

    dino->x = 100.0f;
    dino->y = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_SIZE;
    dino->velocity = 0.0f;
    dino->isJumping = sfFalse;
    dino->isOnPlatform = sfFalse;

    *platform = createRandomPlatform();
    cactus->x = WINDOW_WIDTH;

    *clock = sfClock_create();
}

int main() {
    sfRenderWindow *window;
    Dino dino;
    Platform platform;
    Cactus cactus;
    sfClock *clock;

    int score = 0;
    float scoreTimer = 0.0f; // Temps écoulé depuis la dernière mise à jour du score

    initializeGame(&window, &dino, &platform, &cactus, &clock);

    while (sfRenderWindow_isOpen(window)) {
        handleEvents(window, &dino);

        sfTime deltaTime = sfClock_restart(clock);
        updateDino(&dino, sfTime_asSeconds(deltaTime));

        // Vérifier la collision avec le sol
        if (checkGroundCollision(&dino)) {
            dino.y = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_SIZE;
            dino.isJumping = sfFalse;
            dino.isOnPlatform = sfFalse;  // Réinitialiser isOnPlatform si le dinosaure n'est plus sur le sol
        }

        // Vérifier la collision avec la plateforme
        if (checkCollision(&dino, &platform) && dino.y < platform.y) {
            dino.y = platform.y - DINO_SIZE;
            dino.isJumping = sfFalse;
            dino.isOnPlatform = sfTrue;
        } else {
            dino.isOnPlatform = sfFalse;
        }

        // Mettre à jour le score toutes les 0.1 seconde
        scoreTimer += sfTime_asSeconds(deltaTime);
        if (scoreTimer >= 0.1f) {
            score++;
            scoreTimer = 0.0f;
        }

        if (cactus.x < 0) {
            cactus = createRandomCactus();
        } else {
            cactus.x -= 300.0f * sfTime_asSeconds(deltaTime);
        }

        // Réinitialiser la plateforme si elle est hors de l'écran
        if (platform.x + PLATFORM_WIDTH < 0) {
            platform = createRandomPlatform();
        } else {
            platform.x -= 300.0f * sfTime_asSeconds(deltaTime);
        }

        drawGame(window, &dino, &platform, &cactus, score);
    }

    sfRenderWindow_destroy(window);
    sfClock_destroy(clock);

    return EXIT_SUCCESS;
}
