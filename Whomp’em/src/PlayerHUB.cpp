#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "PlayerHUB.h"
#include "Game.h"
#include "Scene.h"



#define ANIM_SPEED 20

enum HeartAnims
{
};

enum LifeAnims
{
};

enum TotemAnims
{

};


void PlayerHUB::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

    playerLifes = 2;
    playerHP = 4;

    hubSpriteSheet.loadFromFile("images/playerHUBframes.png", TEXTURE_PIXEL_FORMAT_RGBA);
    heartSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.1), &hubSpriteSheet, &shaderProgram);
    heartSprite->setNumberAnimations(30);

	heartSprite->setAnimationSpeed(0, ANIM_SPEED);
    heartSprite->setAnimationAcyclic(0);
    heartSprite->addKeyframe(0, glm::vec2(0.0, 0.75f));

    heartSprite->setAnimationSpeed(1, ANIM_SPEED);
    heartSprite->setAnimationAcyclic(1);
    heartSprite->addKeyframe(1, glm::vec2(0.0, 0.5f));

    heartSprite->setAnimationSpeed(2, ANIM_SPEED);
    heartSprite->setAnimationAcyclic(2);
    heartSprite->addKeyframe(2, glm::vec2(0.0, 0.25f));

    heartSprite->setAnimationSpeed(3, ANIM_SPEED);
    heartSprite->setAnimationAcyclic(3);
    heartSprite->addKeyframe(3, glm::vec2(0.0, 0.0f));


    tileMapDispl = tileMapPos;
    heartSprite->changeAnimation(0);				            
    heartSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y)));

}


void PlayerHUB::update(int deltaTime)
{

    heartSprite->update(deltaTime);
    heartSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y)));

}

void PlayerHUB::render() {

    heartSprite->render();
}

void PlayerHUB::setPosition(const glm::vec2& pos) {
    posHearts = pos;
    heartSprite->setPosition(glm::vec2(float(tileMapDispl.x + posHearts.x), float(tileMapDispl.y + posHearts.y)));

}

void PlayerHUB::setTileMap(TileMap* tileMap) {
    map = tileMap;
}







