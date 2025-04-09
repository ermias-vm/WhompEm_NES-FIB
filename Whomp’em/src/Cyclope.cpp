#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"
#include "Scene.h"
#include "Cyclope.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64.0f
#define JUMP_WIDTH 96.0f
#define PI 3.14159265

#define FLY_ANIM_SPEED 50
#define STAND_ANIM_SPEED 100
#define JUMP_ANIM_SPEED 50

#define JUMP_WIDTH 96.0f

enum CyclopeAnims {
    STAND_RIGHT, STAND_LEFT, JUMP_RIGHT, JUMP_LEFT, FLY_RIGHT, FLY_LEFT
};




void Cyclope::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    tileMapDispl = tileMapPos;
	isLookingLeft = isJumping = false;  
    // Cargar spritesheet
    cyclopeSpritesheet.loadFromFile("images/sprites/cyclopeFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // Crear sprite
    cyclopeSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &cyclopeSpritesheet, &shaderProgram);
    cyclopeSprite->setNumberAnimations(6);

    // Animación STAND_RIGHT
    cyclopeSprite->setAnimationSpeed(STAND_RIGHT, STAND_ANIM_SPEED);
    cyclopeSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

    // Animación STAND_LEFT
    cyclopeSprite->setAnimationSpeed(STAND_LEFT, STAND_ANIM_SPEED);
    cyclopeSprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));

    // Animación JUMP_RIGHT
    cyclopeSprite->setAnimationSpeed(JUMP_RIGHT, JUMP_ANIM_SPEED);
    cyclopeSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.25f));

    // Animación JUMP_LEFT
    cyclopeSprite->setAnimationSpeed(JUMP_LEFT, JUMP_ANIM_SPEED);
    cyclopeSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.25f));

    // Animación FLY_RIGHT
    cyclopeSprite->setAnimationSpeed(FLY_RIGHT, FLY_ANIM_SPEED);
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.0f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.50f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.5f));

    // Animación FLY_LEFT
    cyclopeSprite->setAnimationSpeed(FLY_LEFT, FLY_ANIM_SPEED);
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.0f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.25f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.50f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.25f, 0.75f));

    cyclopeSprite->changeAnimation(STAND_RIGHT);
}

void Cyclope::update(int deltaTime) {
    cyclopeSprite->update(deltaTime);

    if (!isJumping) {
        // Calcular la posición del jugador
        glm::vec2 playerPos = Game::instance().getScene()->getPlayerPos();
        targetPos = playerPos;

        // Iniciar un salto hacia el jugador
        startJump();
    }
    else {
        // Actualizar el salto
        jumpAngle += JUMP_ANGLE_STEP;

        if (jumpAngle >= 180) {
            // El salto ha terminado
            isJumping = false;
            posCyclope.y = startY; // Asegurarse de que el Cyclope esté en el suelo
            cyclopeSprite->changeAnimation(STAND_RIGHT); // Volver a la animación de estar parado
        }
        else {
            // Calcular la posición intermedia durante el salto
            float directionX = (targetPos.x - posCyclope.x) > 0 ? 1.0f : -1.0f;
            posCyclope.x = jumpStartPos.x + (JUMP_WIDTH * sin(PI * jumpAngle / 180.f) * directionX);
            posCyclope.y = jumpStartPos.y - (JUMP_HEIGHT * sin(PI * jumpAngle / 180.f));

            // Cambiar a la animación de vuelo después de unos milisegundos
            if (jumpAngle > 10) {
                cyclopeSprite->changeAnimation(directionX > 0 ? FLY_RIGHT : FLY_LEFT);
            }
        }

        // Actualizar la posición del sprite
        cyclopeSprite->setPosition(glm::vec2(
            float(tileMapDispl.x + posCyclope.x),
            float(tileMapDispl.y + posCyclope.y)
        ));
    }
}

void Cyclope::render() {
    cyclopeSprite->render();
}

void Cyclope::setPosition(const glm::vec2& pos) {
    posCyclope = pos;
    cyclopeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posCyclope.x), float(tileMapDispl.y + posCyclope.y)));
}

glm::vec2 Cyclope::getPosition() const {
    return posCyclope;
}

void Cyclope::startJump() {
    isJumping = true;
    jumpAngle = 0;
    jumpStartPos = posCyclope;
    startY = posCyclope.y;

    // Determinar la dirección del salto
    float directionX = (targetPos.x - posCyclope.x) > 0 ? 1.0f : -1.0f;

    // Cambiar a la animación de salto
    cyclopeSprite->changeAnimation(directionX > 0 ? JUMP_RIGHT : JUMP_LEFT);
}

void Cyclope::setTileMap(TileMap* tileMap) {
    map = tileMap;
}