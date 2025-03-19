#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 3
#define PI 3.14159265
#define ANIMATION_SPEED 12

enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, JUMP_RIGHT, JUMP_LEFT, FALL_RIGHT, FALL_LEFT,
    CROUCH_RIGHT, CROUCH_LEFT, DAMAGED_RIGHT, DAMAGED_LEFT, COVER_RIGHT, COVER_LEFT, HOLD_SPEAR_R, HOLD_SPEAR_L,
	CHARGE_RIGHT, CHARGE_LEFT, ATTACK_RIGHT, ATTACK_LEFT, DIE_RIGHT, DIE_LEFT
};


/**
 * @brief Inicializa el jugador configurando su sprite y animaciones.
 *
 * @param tileMapPos Posición del tile map para calcular el desplazamiento del sprite.
 * @param shaderProgram Programa de shader empleado para renderizar el sprite.
 */
void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bCrouching = false;
	lookingRight = true;
	damaged = false;

	spritesheet.loadFromFile("images/playerFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//(pixels frame, %espació ocupado(ancho,alto), spritesheet, shaderProgram)
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(14);

	// ANIMACIONES

	sprite->setAnimationSpeed(STAND_RIGHT, ANIMATION_SPEED);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(STAND_LEFT, ANIMATION_SPEED);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.1f, 0.0f));
		
	sprite->setAnimationSpeed(MOVE_RIGHT, ANIMATION_SPEED);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.05f));

	sprite->setAnimationSpeed(MOVE_LEFT, ANIMATION_SPEED);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0, 0.10f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1, 0.10f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2, 0.10f));

	sprite->setAnimationSpeed(JUMP_RIGHT, ANIMATION_SPEED);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.15f));

	sprite->setAnimationSpeed(JUMP_LEFT, ANIMATION_SPEED);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.1f, 0.15f));

    sprite->setAnimationSpeed(CROUCH_RIGHT, ANIMATION_SPEED);
    sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.0f, 0.15f));

    sprite->setAnimationSpeed(CROUCH_LEFT, ANIMATION_SPEED);
    sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.1f, 0.15f));
    
    sprite->setAnimationSpeed(FALL_RIGHT, ANIMATION_SPEED);
    sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.0f, 0.15f));

    sprite->setAnimationSpeed(FALL_LEFT, ANIMATION_SPEED);
    sprite->addKeyframe(FALL_LEFT, glm::vec2(0.1f, 0.15f));
	

	////

	sprite->changeAnimation(STAND_RIGHT); 	// Animación inicial

	tileMapDispl = tileMapPos;				// Desplazamiento del tile map (0,0)
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);

    // Movimiento horizontal cuando no está saltando ni agachado
    if (!bJumping && !bCrouching) {
        if (Game::instance().getKey(GLFW_KEY_LEFT)) {
            lookingRight = false;  // El jugador mira a la izquierda
            if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
            posPlayer.x -= 2;
            if (map->collisionMoveLeft(posPlayer, glm::ivec2(22, 32))) {
                posPlayer.x += 2;
                sprite->changeAnimation(STAND_LEFT);
            }
        }
        else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
            lookingRight = true;  // El jugador mira a la derecha
            if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
            posPlayer.x += 2;
            if (map->collisionMoveRight(posPlayer, glm::ivec2(22, 32))) {
                posPlayer.x -= 2;
                sprite->changeAnimation(STAND_RIGHT);
            }
        }
        else {
            if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
            else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
        }
    }
    
    // Gestión del salto
    if (bJumping) {
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180) {  // El salto ha terminado
            bJumping = false;
            posPlayer.y = startY;
            if (lookingRight) sprite->changeAnimation(STAND_RIGHT);
            else sprite->changeAnimation(STAND_LEFT);
        }
        else {  // El jugador está en el aire (subiendo o bajando)
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(PI * jumpAngle / 180.f));

            if (jumpAngle > 90) {  // Bajando
                if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
                    bJumping = false;
                    if (lookingRight) sprite->changeAnimation(STAND_RIGHT);
                    else sprite->changeAnimation(STAND_LEFT);
                }
                else bJumping = true;
            }

            // Movimiento horizontal durante el salto
            if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                posPlayer.x -= 2;
                if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
                lookingRight = false;
                if (map->collisionMoveLeft(posPlayer, glm::ivec2(22, 32))) posPlayer.x += 2;
            }
            else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                posPlayer.x += 2;
                if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
                lookingRight = true;
                if (map->collisionMoveRight(posPlayer, glm::ivec2(22, 32))) posPlayer.x -= 2;
            }
        }
    }
    else {  // El jugador no está saltando (puede estar cayendo, agachado o en el suelo)
        posPlayer.y += FALL_STEP;
        if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
            // El jugador está en el suelo
            if (Game::instance().getKey(GLFW_KEY_UP)) {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
                if (lookingRight) sprite->changeAnimation(JUMP_RIGHT);
                else sprite->changeAnimation(JUMP_LEFT);
            }
            else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                bCrouching = true;
                if (lookingRight && sprite->animation() != CROUCH_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);
                else if (!lookingRight && sprite->animation() != CROUCH_LEFT) sprite->changeAnimation(CROUCH_LEFT);
            }
            else {
                bCrouching = false;
                // Asegurarse de que la animación sea STAND al tocar el suelo si no se mueve ni agacha
                if (sprite->animation() != STAND_RIGHT && sprite->animation() != STAND_LEFT &&
                    sprite->animation() != MOVE_RIGHT && sprite->animation() != MOVE_LEFT) {
                    if (lookingRight) sprite->changeAnimation(STAND_RIGHT);
                    else sprite->changeAnimation(STAND_LEFT);
                }
            }
        }
        else {  // El jugador está cayendo
            if (lookingRight && sprite->animation() != FALL_RIGHT) sprite->changeAnimation(FALL_RIGHT);
            else if (!lookingRight && sprite->animation() != FALL_LEFT) sprite->changeAnimation(FALL_LEFT);

            // Movimiento horizontal durante la caída
            if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                posPlayer.x -= 2;
                if (sprite->animation() != FALL_LEFT) sprite->changeAnimation(FALL_LEFT);
                lookingRight = false;
                if (map->collisionMoveLeft(posPlayer, glm::ivec2(22, 32))) posPlayer.x += 2;
            }
            else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                posPlayer.x += 2;
                if (sprite->animation() != FALL_RIGHT) sprite->changeAnimation(FALL_RIGHT);
                lookingRight = true;
                if (map->collisionMoveRight(posPlayer, glm::ivec2(22, 32))) posPlayer.x -= 2;
            }
        }
    }
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render() {
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}


void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() {
    return posPlayer;
}


glm::vec2 Player::getVelocity() {
    glm::vec2 velocity(0.f, 0.f);

    // Velocidad horizontal
    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
        velocity.x = -2.f; // Velocidad fija hacia la izquierda
    }
    else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
        velocity.x = 2.f; // Velocidad fija hacia la derecha
    }

    // Velocidad vertical
    if (bJumping) {
        // Aproximación de la velocidad vertical durante el salto
        velocity.y = -JUMP_HEIGHT * cos(PI * jumpAngle / 180.f) * JUMP_ANGLE_STEP / 180.f * PI;
    }
    else {
        // Si está cayendo
        velocity.y = FALL_STEP;
    }

    return velocity;
}
