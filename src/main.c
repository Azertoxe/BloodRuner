#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH 550
#define WINDOW_HEIGHT 800
#define GROUND_HEIGHT 50
#define DINO_SIZE 50
#define CACTUS_SIZE 30
#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 20
#define MAX_PLATFORMS 4

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
        dino->y += 400.0f * deltaTime;
    }
}

sfBool checkCollision(Dino *dino, Platform *platform) {
    return dino->x < platform->x + PLATFORM_WIDTH &&
           dino->x + DINO_SIZE > platform->x &&
           dino->y + DINO_SIZE > platform->y &&
           dino->y < platform->y + PLATFORM_HEIGHT &&
           dino->velocity >= 0.0f;
}

sfBool checkGroundCollision(Dino *dino) {
    return dino->y + DINO_SIZE >= WINDOW_HEIGHT - GROUND_HEIGHT;
}

Platform createRandomPlatform() {
    Platform platform;
    platform.x = WINDOW_WIDTH;
    platform.y = WINDOW_HEIGHT - GROUND_HEIGHT - (rand() % 100 + 60);
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

void drawGame(sfRenderWindow *window, Dino *dino, Platform *platform, Cactus *cactus, int score, sfSprite *backgroundSprite) {
    sfRenderWindow_clear(window, sfBlack);

    // Move the background
    sfVector2f backgroundPosition = sfSprite_getPosition(backgroundSprite);
    sfSprite_setPosition(backgroundSprite, (sfVector2f){backgroundPosition.x - 1.0f, backgroundPosition.y});

    // Draw the background
    sfRenderWindow_drawSprite(window, backgroundSprite, NULL);

    // Draw the ground
    sfRectangleShape *ground = sfRectangleShape_create();
    sfRectangleShape_setSize(ground, (sfVector2f){WINDOW_WIDTH, GROUND_HEIGHT});
    sfRectangleShape_setFillColor(ground, sfRed);
    sfRectangleShape_setPosition(ground, (sfVector2f){0, WINDOW_HEIGHT - GROUND_HEIGHT});
    sfRenderWindow_drawRectangleShape(window, ground, NULL);
    sfRectangleShape_destroy(ground);

    // Draw the dinosaur
    sfSprite *dinoSprite = sfSprite_create();
    sfTexture *dinoTexture = sfTexture_createFromFile("asset/goutte.png", NULL);
    sfSprite_setTexture(dinoSprite, dinoTexture, sfTrue);
    sfSprite_setPosition(dinoSprite, (sfVector2f){dino->x, dino->y - 34});
    sfRenderWindow_drawSprite(window, dinoSprite, NULL);
    sfSprite_destroy(dinoSprite);
    sfTexture_destroy(dinoTexture);

    // Draw the platform
    drawPlatform(window, platform);

    // Draw the cactus
    sfRectangleShape *cactusShape = sfRectangleShape_create();
    sfSprite *cactusSprite = sfSprite_create();
    sfTexture *cactusTexture = sfTexture_createFromFile("asset/spike.png", NULL);
    sfSprite_setTexture(cactusSprite, cactusTexture, sfTrue);
    sfSprite_setPosition(cactusSprite, (sfVector2f){cactus->x, cactus->y+10});
    sfRenderWindow_drawSprite(window, cactusSprite, NULL);
    sfSprite_destroy(cactusSprite);
    sfTexture_destroy(cactusTexture);

    // Display the score at the top of the window
    sfFont *font = sfFont_createFromFile("arial.ttf");
    sfText *scoreText = sfText_create();
    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", score);
    sfText_setString(scoreText, scoreStr);
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 20);
    sfText_setFillColor(scoreText, sfBlack);
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

void initializeGame(sfRenderWindow **window, Dino *dino, Platform *platform, Cactus *cactus, sfClock **clock, sfSprite **backgroundSprite) {
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    *window = sfRenderWindow_create(mode, "Dino Game", sfResize | sfClose, NULL);

    if (!(*window)) {
        exit(EXIT_FAILURE);
    }

    sfRenderWindow_setFramerateLimit(*window, 60);

    // Load the background texture and sprite
    sfTexture *backgroundTexture = sfTexture_createFromFile("asset/Fond.png", NULL);
    *backgroundSprite = sfSprite_create();
    sfSprite_setTexture(*backgroundSprite, backgroundTexture, sfTrue);

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
    sfSprite *backgroundSprite;

    int score = 0;
    float scoreTimer = 0.0f;

    initializeGame(&window, &dino, &platform, &cactus, &clock, &backgroundSprite);

    while (sfRenderWindow_isOpen(window)) {
        handleEvents(window, &dino);

        sfTime deltaTime = sfClock_restart(clock);
        updateDino(&dino, sfTime_asSeconds(deltaTime));

        // Check collision with the ground
        if (checkGroundCollision(&dino)) {
            dino.y = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_SIZE;
            dino.isJumping = sfFalse;
            dino.isOnPlatform = sfFalse;
        }

        // Check collision with the platform
        if (checkCollision(&dino, &platform) && dino.y < platform.y) {
            dino.y = platform.y - DINO_SIZE;
            dino.isJumping = sfFalse;
            dino.isOnPlatform = sfTrue;
        } else {
            dino.isOnPlatform = sfFalse;
        }

        // Update the score every 0.1 seconds
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

        if (platform.x + PLATFORM_WIDTH < 0) {
            platform = createRandomPlatform();
        } else {
            platform.x -= 300.0f * sfTime_asSeconds(deltaTime);
        }

        drawGame(window, &dino, &platform, &cactus, score, backgroundSprite);
    }

    sfRenderWindow_destroy(window);
    sfClock_destroy(clock);
    sfSprite_destroy(backgroundSprite);

    return EXIT_SUCCESS;
}
