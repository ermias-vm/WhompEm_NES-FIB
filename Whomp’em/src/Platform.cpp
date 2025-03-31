#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"
#include "Scene.h"
#include "Platform.h"

#define ANIM_SPEED 20



void Platform::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {


    platformSpriteSheet.loadFromFile("images/sprites/plataformFrame.png", TEXTURE_PIXEL_FORMAT_RGBA);
    platformSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &platformSpriteSheet, &shaderProgram);
    platformSprite->setNumberAnimations(1);

    platformSprite->setAnimationSpeed(0, ANIM_SPEED);
    platformSprite->setAnimationAcyclic(0);
    platformSprite->addKeyframe(0, glm::vec2(0.0, 0.0f));


    tileMapDispl = tileMapPos;
    platformSprite->changeAnimation(0);
}


void Platform::update(int deltaTime)
{
    platformSprite->update(deltaTime);

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
