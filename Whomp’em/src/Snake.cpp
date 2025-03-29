#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Snake.h"
#include "Game.h"

#define SNAKE_SPEED 16.f // Velocidad de la serpiente (pixeles por segundo)
#define ANIMATION_SPEED 7 // Velocidad de las animaciones (más lento)

enum SnakeAnims {
    STAND_RIGHT,
    STAND_LEFT,
    MOVE_RIGHT,
    MOVE_LEFT
};

void Snake::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    movementDirection = -1; // Empieza moviéndose a la izquierda
    tileMapDispl = tileMapPos;
    timeAccumulator = 0.0f; // Inicializa el acumulador de tiempo
    posSnake = glm::vec2(100.0f, 160.0f); // Posición inicial razonable

    SnakeSpritesheet.loadFromFile("images/SNAKE.png", TEXTURE_PIXEL_FORMAT_RGBA);
    SnakeSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &SnakeSpritesheet, &shaderProgram);
    SnakeSprite->setNumberAnimations(8);

    SnakeSprite->setAnimationSpeed(MOVE_RIGHT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.50f));
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.50f));
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.50f));
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.50f));
    SnakeSprite->setAnimationSpeed(MOVE_LEFT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0, 0.0f));
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.0f));
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50, 0.0f));
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75, 0.0f));

    SnakeSprite->changeAnimation(MOVE_LEFT);
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}

void Snake::render() {
    SnakeSprite->render();
}

void Snake::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Snake::update(int deltaTime) {
    SnakeSprite->update(deltaTime); // Actualiza la animación del sprite continuamente
    timeAccumulator += deltaTime;   // Acumula el tiempo

    const float updateInterval = 100.0f; // Actualiza cada 0.5 segundos
    const float leftBoundary = 1 * 16;   // 16
    const float rightBoundary = 10 * 16; // 1600 (ajustado para que sea más alcanzable)

    if (timeAccumulator >= updateInterval) {
        float displacement = SNAKE_SPEED * (updateInterval / 1000.0f); // Desplazamiento por intervalo
        posSnake.x += (displacement * movementDirection); // Aplica el desplazamiento
        if (posSnake.x <= leftBoundary) {
            movementDirection = 1;
            SnakeSprite->changeAnimation(MOVE_RIGHT);
        }
        else if (posSnake.x >= rightBoundary) {
            movementDirection = -1;
            SnakeSprite->changeAnimation(MOVE_LEFT);
        }

        SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
        timeAccumulator -= updateInterval; // Reinicia el acumulador
    }
}

void Snake::setPosition(const glm::vec2& pos) {
    posSnake = pos;
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}

glm::vec2 Snake::getPosition() {
    return posSnake;
}