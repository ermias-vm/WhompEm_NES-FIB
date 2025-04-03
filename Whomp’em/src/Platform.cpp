#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"
#include "Scene.h"
#include "Platform.h"

#define ANIM_SPEED 20



void Platform::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram,float startY, float range, float speed) {

    this->startY = startY;
    this->range = range;
    this->speed = speed;
    direction = -1; // Inicia subiendo
    deltaY = 0.0f;

    posPlatform = glm::vec2(tileMapPos.x, startY);

    platformSpriteSheet.loadFromFile("images/sprites/plataformFrame.png", TEXTURE_PIXEL_FORMAT_RGBA);
    platformSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &platformSpriteSheet, &shaderProgram);
    platformSprite->setNumberAnimations(1);

    platformSprite->setAnimationSpeed(0, ANIM_SPEED);
    platformSprite->setAnimationAcyclic(0);
    platformSprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));


    tileMapDispl = tileMapPos;
    platformSprite->changeAnimation(0);
}

void Platform::update(int deltaTime) {
    platformSprite->update(deltaTime);
    // Calcular movimiento vertical
    deltaY = speed * direction;
    posPlatform.y += deltaY;

    // Invertir dirección si se excede el rango
    if (posPlatform.y >= startY + range || posPlatform.y <= startY - range) {
        direction *= -1;
    }

    // Actualizar posición del sprite
    platformSprite->setPosition(glm::vec2(float(posPlatform.x), float(posPlatform.y)));

    
}

void Platform::render() {
    platformSprite->render();
}

void Platform::setPosition(const glm::vec2& pos) {
    posPlatform = pos;
    platformSprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void Platform::setTileMap(TileMap* tileMap) {
    map = tileMap;
}
