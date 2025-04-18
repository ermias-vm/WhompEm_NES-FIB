﻿#include <cmath>
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
    godMode = is_H_pressed = is_G_pressed = is_B_pressed = showBossHP = false;
    playerLifes = 2;
    playerHP = 12;
    bossHP = 24;

    // HP Player
    hubSpriteSheet.loadFromFile("images/sprites/playerHUBframes.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

    // HP Boss
    bossHpSpriteSheet.loadFromFile("images/sprites/bossHPframes.png", TEXTURE_PIXEL_FORMAT_RGBA);
    bossHpSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.04, 1.0), &bossHpSpriteSheet, &shaderProgram);
    bossHpSprite->setNumberAnimations(25);

    bossHpSprite->setAnimationSpeed(0, ANIM_SPEED);
    float x = 0.0f;

    for (int i = 0; i <= 24; ++i) {
        bossHpSprite->setAnimationSpeed(i, ANIM_SPEED);
        bossHpSprite->setAnimationAcyclic(i);
        bossHpSprite->addKeyframe(i, glm::vec2(x, 0.0f));
        x += 0.04f;
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

    // GODMODE

    godModeSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &hubSpriteSheet, &shaderProgram);
    godModeSprite->setNumberAnimations(5);

    godModeSprite->setAnimationSpeed(0, ANIM_SPEED);
    godModeSprite->setAnimationAcyclic(0);
    godModeSprite->addKeyframe(0, glm::vec2(0.5, 0.9f));



    /////////
    tileMapDispl = tileMapPos;
    heartSprite->changeAnimation(playerHP);
    lifeSprite->changeAnimation(playerLifes);
    totemSprite->changeAnimation(NORMAL_SPEAR);
    godModeSprite->changeAnimation(0);
    bossHpSprite->changeAnimation(bossHP);
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
void PlayerHUB::restoreLivesHp() {
    playerHP = 12;
    playerLifes = 2;
    heartSprite->changeAnimation(playerHP);
    lifeSprite->changeAnimation(playerLifes);
    cout << "PLAYER_HUB: Restored Lives and HP" << endl;
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

void PlayerHUB::bossTakeDamage(int amount) {
    bossHP -= amount;
    if (bossHP <= 0) {
        bossHP = 0;
    }
    bossHpSprite->changeAnimation(bossHP);
}

void PlayerHUB::setTotemAnimation(bool bUsingTotem) {
    totemSprite->changeAnimation(bUsingTotem);
}

void PlayerHUB::update(int deltaTime)
{
    checkCheats();
    heartSprite->update(deltaTime);
    lifeSprite->update(deltaTime);
    totemSprite->update(deltaTime);
    godModeSprite->update(deltaTime);
    bossHpSprite->update(deltaTime);
}

void PlayerHUB::render() {

    heartSprite->render();
    lifeSprite->render();
    totemSprite->render();
    if (godMode) godModeSprite->render();
    // TO DO:  renderitzar nomes a partir de la x on esta el boss
    if (showBossHP) bossHpSprite->render();
}
void PlayerHUB::checkCheats() {
    // Gestionar la tecla H  para curar completamente al jugador
    if (Game::instance().getKey(GLFW_KEY_H)) {
        if (!is_H_pressed) {
            is_H_pressed = true;
            restoreLivesHp();;
        }
    }
    else {
        is_H_pressed = false;
    }
    // Gestionar la tecla G  activar godMode
    if (Game::instance().getKey(GLFW_KEY_G)) {
        if (!is_G_pressed) {
            is_G_pressed = true;
            godMode = !godMode;
            if (godMode) cout << "PLAYER: God Mode Activated" << endl;
            else cout << "PLAYER: God Mode Deactivated" << endl;
        }
    }
    else {
        is_G_pressed = false;
    }

    if (Game::instance().getKey(GLFW_KEY_B)) {
        if (!is_B_pressed) {
            is_B_pressed = true;
            bossTakeDamage(1);
            cout << "PLAYER: Boss HP -1, Total HP : " << bossHP << endl;

        }
    }
    else {
        is_B_pressed = false;
    }
}

void PlayerHUB::setPosition(const glm::vec2& pos) {
    posHearts = pos;
    heartSprite->setPosition(glm::vec2(float(pos.x + 8), float(pos.y + 24)));
    totemSprite->setPosition(glm::vec2(float(pos.x + 8), float(pos.y - 10)));
    lifeSprite->setPosition(glm::vec2(float(pos.x + 28), float(pos.y - 10)));
    godModeSprite->setPosition(glm::vec2(float(pos.x + 48), float(pos.y - 10)));
    bossHpSprite->setPosition(glm::vec2(float(pos.x + 28), float(pos.y + 24)));
}

void PlayerHUB::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

bool PlayerHUB::isBossHPDead() const {
    return bossHP <= 0;
}



