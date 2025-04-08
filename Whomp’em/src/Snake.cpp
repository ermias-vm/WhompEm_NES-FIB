#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Snake.h"
#include "Game.h"

#define SNAKE_SPEED 30
#define ANIMATION_SPEED 7

enum SnakeAnims {
    STAND_RIGHT,
    STAND_LEFT,
    MOVE_RIGHT,
    MOVE_LEFT
};

Snake::Snake() : SnakeSprite(nullptr), map(nullptr), movementDirection(0), timeAccumulator(0.f), isJumping(false), hasJumpedOnce(false), jumpTime(0.f) {
}

Snake::~Snake() {
    if (SnakeSprite) delete SnakeSprite;
}

void Snake::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int direction, glm::vec2 pos) {
    movementDirection = direction;
    if (direction == 1) {
        minpos = pos.x;
        maxpos = pos.x + 16.0f * 16.0f;
    }
    else {
        maxpos = pos.x;
        minpos = pos.x - 16.0f * 16.0f;
    }
    tileMapDispl = tileMapPos;
    timeAccumulator = 0.0f;
    posSnake = pos;
    spawnPos = posSnake;
    SnakeSpritesheet.loadFromFile("images/sprites/snakeFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

    if (direction == -1) SnakeSprite->changeAnimation(MOVE_LEFT);
    else if (direction == 1) SnakeSprite->changeAnimation(MOVE_RIGHT);
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}

bool Snake::shouldDisappear() const {
    return false;
}

void Snake::render() {
    SnakeSprite->render();
}

void Snake::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Snake::update(int deltaTime) {
    SnakeSprite->update(deltaTime);
    timeAccumulator += deltaTime;

    if (isJumping) {
        jumpTime += deltaTime;
        float t = jumpTime / jumpDuration;
        float direction = (movementDirection == 1) ? 1.0f : -1.0f;

        if (t >= 1.0f) {
            posSnake.x = jumpStartPos.x + (jumpWidth * direction);
            posSnake.y = jumpStartPos.y;
            isJumping = false;
        }
        else {
            posSnake.x = jumpStartPos.x + (jumpWidth * t * direction);
            posSnake.y = jumpStartPos.y + (4.0f * jumpHeight * t * t) - (4.0f * jumpHeight * t);
        }

        SnakeSprite->setPosition(glm::vec2(
            float(tileMapDispl.x + posSnake.x),
            float(tileMapDispl.y + posSnake.y)
        ));
    }
    else {
        const float updateInterval = 100.0f;

        if (timeAccumulator >= updateInterval) {
            float displacement = SNAKE_SPEED * (updateInterval / 1000.0f);
            posSnake.x += (displacement * movementDirection);
            SnakeSprite->setPosition(glm::vec2(
                float(tileMapDispl.x + posSnake.x),
                float(tileMapDispl.y + posSnake.y)
            ));
            timeAccumulator -= updateInterval;
            if (posSnake.x >= maxpos) {
                movementDirection = -1;
                SnakeSprite->changeAnimation(MOVE_LEFT);
            }
            else if (posSnake.x <= minpos) {
                movementDirection = 1;
                SnakeSprite->changeAnimation(MOVE_RIGHT);
            }
        }
    }
}

void Snake::snakeJump() {
    if (!isJumping && !hasJumpedOnce) { // Solo salta si no está saltando y no ha saltado antes
        isJumping = true;
        hasJumpedOnce = true; // Marca que ya ha saltado
        jumpTime = 0.0f;
        jumpStartPos = posSnake;

        if (movementDirection == 1) {
            SnakeSprite->changeAnimation(MOVE_RIGHT);
        }
        else {
            SnakeSprite->changeAnimation(MOVE_LEFT);
        }
    }
}

int Snake::getMovementDirection() {
    return movementDirection;
}

void Snake::setPosition(const glm::vec2& pos) {
    posSnake = pos;
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}

glm::vec2 Snake::getPosition() {
    return posSnake;
}