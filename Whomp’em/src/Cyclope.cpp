#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"
#include "Scene.h"
#include "Cyclope.h"

#define JUMP_HEIGHT 60
#define JUMP_WIDTH_DEF 70
#define JUMP_DURATION 0.8f
#define GROUND_DELAY 1.0f
#define TRIGGER_DISTANCE 200
#define CLOSE_DISTANCE 50  // Distancia para salto exacto

enum CyclopeAnims {
    STAND_RIGHT, STAND_LEFT, JUMP_RIGHT, JUMP_LEFT, FLY_RIGHT, FLY_LEFT
};

void Cyclope::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    tileMapDispl = tileMapPos;
    isJumping = false;
    bLookingLeft = false;
    timeOnGround = 0.0f;
	jumpWidth = JUMP_WIDTH_DEF;
    cyclopeSpritesheet.loadFromFile("images/sprites/cyclopeFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    cyclopeSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &cyclopeSpritesheet, &shaderProgram);
    cyclopeSprite->setNumberAnimations(6);

    cyclopeSprite->setAnimationSpeed(STAND_RIGHT, 100);
    cyclopeSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

    cyclopeSprite->setAnimationSpeed(STAND_LEFT, 100);
    cyclopeSprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));

    cyclopeSprite->setAnimationSpeed(JUMP_RIGHT, 50);
    cyclopeSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.25f));

    cyclopeSprite->setAnimationSpeed(JUMP_LEFT, 50);
    cyclopeSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.25f));

    cyclopeSprite->setAnimationSpeed(FLY_RIGHT, 50);
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.0f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.50f, 0.5f));
    cyclopeSprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.5f));

    cyclopeSprite->setAnimationSpeed(FLY_LEFT, 50);
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.0f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.25f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.50f, 0.75f));
    cyclopeSprite->addKeyframe(FLY_LEFT, glm::vec2(0.25f, 0.75f));

    cyclopeSprite->changeAnimation(STAND_RIGHT);
}

void Cyclope::changeAnimToRightLeft(int animation) {
    if (bLookingLeft) {
        animation++;
    }
    cyclopeSprite->changeAnimation(animation);
}

void Cyclope::update(int deltaTime) {
    cyclopeSprite->update(deltaTime);
    float dt = deltaTime / 1000.0f;

    if (bLookingLeft) {
        if (map->collisionMoveLeft(posCyclope, glm::ivec2(25, 32))) {
            posCyclope.x += 1;
        }
    }

	else {
		if (map->collisionMoveRight(posCyclope, glm::ivec2(28, 32))) {
			posCyclope.x -= 1;
		}
	}

    if (isJumping) {
        jumpTime += dt;
        float t = jumpTime / JUMP_DURATION;
        posCyclope.x = jumpStartPos.x + static_cast<int>(jumpWidth * t * jumpDirection);
        posCyclope.y = jumpStartPos.y - static_cast<int>(jumpWidth * sinf(3.14159f * t));

        if (map->collisionMoveLeft(posCyclope, glm::ivec2(25, 32))) {
            posCyclope.x += 1;
        }
        if (map->collisionMoveRight(posCyclope, glm::ivec2(28, 32))) {
            posCyclope.x -= 1;
        }

        if (jumpTime >= JUMP_DURATION || map->collisionMoveDown(posCyclope, glm::ivec2(32, 32), &posCyclope.y)) {
            isJumping = false;
            timeOnGround = 0.0f;
            if (cyclopeSprite->animation() != (STAND_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(STAND_RIGHT);
            }
        }
        else {
            if (cyclopeSprite->animation() != (FLY_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(FLY_RIGHT);
            }
        }
    }
    else {
        if (map->collisionMoveDown(posCyclope, glm::ivec2(32, 32), &posCyclope.y)) {
            timeOnGround += dt;
            if (timeOnGround >= GROUND_DELAY) {
                glm::vec2 playerPosFloat = Game::instance().getScene()->getPlayerPos();
                glm::ivec2 playerPos(static_cast<int>(playerPosFloat.x), static_cast<int>(playerPosFloat.y));
                int distanceX = abs(playerPos.x - posCyclope.x);

                if (distanceX < TRIGGER_DISTANCE) {
                    jumpStartPos = posCyclope;
                    jumpTime = 0.0f;
                    isJumping = true;
                    jumpDirection = (playerPos.x > posCyclope.x) ? 1 : -1;
                    bLookingLeft = (jumpDirection < 0);
                    if  (distanceX < CLOSE_DISTANCE){
                        if (playerPos.y != posCyclope.y) jumpWidth = JUMP_WIDTH_DEF - 20;
						else jumpWidth = distanceX;
					}
					else jumpWidth = JUMP_WIDTH_DEF;
						
                    if (cyclopeSprite->animation() != (JUMP_RIGHT + bLookingLeft)) {
                        changeAnimToRightLeft(JUMP_RIGHT);
                    }
                }
            }
            if (cyclopeSprite->animation() != (STAND_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(STAND_RIGHT);
            }
        }
        else {
            posCyclope.y += 4;

            if (cyclopeSprite->animation() != (FLY_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(FLY_RIGHT);
            }
        }
    }

    cyclopeSprite->setPosition(glm::vec2(
        float(tileMapDispl.x + posCyclope.x),
        float(tileMapDispl.y + posCyclope.y)
    ));
}

void Cyclope::render() {
    cyclopeSprite->render();
}

void Cyclope::setPosition(const glm::ivec2& pos) {
    posCyclope = pos;
    cyclopeSprite->setPosition(glm::vec2(float(tileMapDispl.x + posCyclope.x), float(tileMapDispl.y + posCyclope.y)));
}

glm::ivec2 Cyclope::getPosition() const {
    return posCyclope;
}

void Cyclope::setTileMap(TileMap* tileMap) {
    map = tileMap;
}