#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "PlayerHUB.h"
#include "Game.h"
#include "Scene.h"

#define ANIM_SPEED 20

enum TotemAnims {
    NORMAL_SPEAR, FIRE_SPEAR
};


void PlayerHUB::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

    playerLifes = 2;
    playerHP = 12;
    // CORAZONES
    hubSpriteSheet.loadFromFile("images/playerHUBframes.png", TEXTURE_PIXEL_FORMAT_RGBA);
    heartSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &hubSpriteSheet, &shaderProgram);
    heartSprite->setNumberAnimations(25);

    heartSprite->setAnimationSpeed(0, ANIM_SPEED);
    heartSprite->setAnimationAcyclic(0);
    heartSprite->addKeyframe(0, glm::vec2(0.75, 0.0f));

    for (int i = 1; i <= 24; ++i) {
        heartSprite->setAnimationSpeed(i, ANIM_SPEED);
        heartSprite->setAnimationAcyclic(i);
        float x = (i % 3 == 1) ? 0.50f : (i % 3 == 2) ? 0.25f : 0.0f;
        float y = ((i - 1) / 3) * 0.1f;
        heartSprite->addKeyframe(i, glm::vec2(x, y));
    }

    // VIDAS
    lifeSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &hubSpriteSheet, &shaderProgram);
    lifeSprite->setNumberAnimations(5);

    lifeSprite->setAnimationSpeed(0, ANIM_SPEED);
    lifeSprite->setAnimationAcyclic(0);
    lifeSprite->addKeyframe(0, glm::vec2(0.5, 0.8f));

    lifeSprite->setAnimationSpeed(1, ANIM_SPEED);
    lifeSprite->setAnimationAcyclic(1);
    lifeSprite->addKeyframe(1, glm::vec2(0.25, 0.8f));

    lifeSprite->setAnimationSpeed(2, ANIM_SPEED);
    lifeSprite->setAnimationAcyclic(2);
    lifeSprite->addKeyframe(2, glm::vec2(0.0, 0.8f));

    // TOTEMS

    totemSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &hubSpriteSheet, &shaderProgram);
    totemSprite->setNumberAnimations(5);

    totemSprite->setAnimationSpeed(NORMAL_SPEAR, ANIM_SPEED);
    totemSprite->setAnimationAcyclic(NORMAL_SPEAR);
    totemSprite->addKeyframe(NORMAL_SPEAR, glm::vec2(0.0, 0.9f));

    totemSprite->setAnimationSpeed(FIRE_SPEAR, ANIM_SPEED);
    totemSprite->setAnimationAcyclic(FIRE_SPEAR);
    totemSprite->addKeyframe(FIRE_SPEAR, glm::vec2(0.25, 0.9f));

    /////////
    tileMapDispl = tileMapPos;
    heartSprite->changeAnimation(playerHP);
    lifeSprite->changeAnimation(playerLifes);
    totemSprite->changeAnimation(NORMAL_SPEAR);

    // Posición base para heartSprite
    posHearts = glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y));
    heartSprite->setPosition(posHearts);

    // Posición de totemSprite (encima de heartSprite)
    glm::vec2 posTotem = glm::vec2(posHearts.x + 16, posHearts.y - 5);
    totemSprite->setPosition(posTotem);

    // Posición de lifeSprite (al lado derecho de totemSprite y encima de heartSprite)
    glm::vec2 posLife = glm::vec2(posTotem.x + 20, posTotem.y);
    lifeSprite->setPosition(posLife);

}

bool PlayerHUB::isPlayerDead() const {
    return playerHP <= 0 && playerLifes <= 0;
}

int PlayerHUB::getPlayerHP() const {
    return playerHP;
}

int PlayerHUB::getPlayerLifes() const {
    return playerLifes;
}

void PlayerHUB::modifyPlayerHP(int amount, bool addHeart) {
    if (addHeart) playerHP += 3;
    else if (amount > 0) {
        int previousHP = playerHP;
        playerHP += amount;
        if (previousHP <= 12) {
            if (playerHP > 12) playerHP = 12;
        }
        else {
            if (previousHP % 3 != 0) {
                playerHP = previousHP + (3 - (previousHP % 3));
            }
            else playerHP = previousHP;
        }
    }
    else {
        playerHP += amount;
        if (playerHP <= 0) {
            playerLifes--;
            if (playerLifes >= 0) {
                playerHP = 12; // Restaurar HP al máximo si quedan vidas
            }
            else {
                playerHP = playerLifes = 0;
            }
        }
    }
    if (playerHP > 24) playerHP = 24;

    // Actualizar la animación del HUD
    heartSprite->changeAnimation(playerHP);
    lifeSprite->changeAnimation(playerLifes);
}

void PlayerHUB::setTotemAnimation(bool bUsingTotem) {
    totemSprite->changeAnimation(bUsingTotem);
}

void PlayerHUB::update(int deltaTime)
{

    heartSprite->update(deltaTime);
    heartSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y)));
}

void PlayerHUB::render() {

    heartSprite->render();
    lifeSprite->render();
    totemSprite->render();
}

void PlayerHUB::setPosition(const glm::vec2& pos) {
    posHearts = pos;
    heartSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y)));

}

void PlayerHUB::setTileMap(TileMap* tileMap) {
    map = tileMap;
}
