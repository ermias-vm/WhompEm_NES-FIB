#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Snake.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define PI 3.14159265
#define ANIMATION_SPEED 20

enum SnakeAnims
{
    STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT
};

void Snake::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

    SnakeSpeed = 16.f;
    count = 0;
    right = false;
    movementCooldown = 0;
    // ANIMACIONES Snake //

    SnakeSpritesheet.loadFromFile("images/SNAKE.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //(pixels frame, %espació ocupado(ancho,alto), SnakeSpritesheet, shaderProgram)
    SnakeSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &SnakeSpritesheet, &shaderProgram);
    SnakeSprite->setNumberAnimations(8);

    SnakeSprite->setAnimationSpeed(STAND_RIGHT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.75f, 0.50f));

    SnakeSprite->setAnimationSpeed(STAND_LEFT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0f));

    SnakeSprite->setAnimationSpeed(MOVE_RIGHT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.50f));
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.50f));
    SnakeSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.50f));

    SnakeSprite->setAnimationSpeed(MOVE_LEFT, ANIMATION_SPEED);
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.0f));
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50, 0.0f));
    SnakeSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75, 0.0f));

    //changeAnimToRightLeft(*SnakeSprite, STAND_RIGHT);  // Animación inicial
    SnakeSprite->changeAnimation(STAND_LEFT);
    tileMapDispl = tileMapPos;				            // Desplazamiento del tile map (0,0)
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));

}

void Snake::render() {
    int SnakeAnim = SnakeSprite->animation();
    SnakeSprite->render();
}

void Snake::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Snake::update(int deltatime) {
    SnakeSprite->update(deltatime);

    // Control de tiempo para ralentizar el movimiento
    movementCooldown += deltatime;
    if (movementCooldown < 2000) return; // Espera antes de mover
    movementCooldown = 0; // Reinicia el contador

    // Movimiento progresivo
    if (right) {
        if (count == 0) {
            right = false;
            SnakeSprite->changeAnimation(STAND_LEFT);
        }
        else {
            if (SnakeSprite->animation() != MOVE_RIGHT)
                SnakeSprite->changeAnimation(MOVE_RIGHT);

            posSnake.x += SnakeSpeed; // Movimiento progresivo
            --count;
        }
    }
    else {
        if (count == max) {
            right = true;
            SnakeSprite->changeAnimation(STAND_RIGHT);
        }
        else {
            if (SnakeSprite->animation() != MOVE_LEFT)
                SnakeSprite->changeAnimation(MOVE_LEFT);

            posSnake.x -= SnakeSpeed; // Movimiento progresivo
            ++count;
        }
    }

    // Aplicar la posición actualizada
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}


void Snake::setPosition(const glm::vec2& pos) {
    posSnake = pos;
    SnakeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSnake.x), float(tileMapDispl.y + posSnake.y)));
}

glm::vec2 Snake::getPosition() {
    return posSnake;
}