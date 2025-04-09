#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Proyectil.h"

Proyectil::Proyectil() : proyectilSprite(nullptr), map(nullptr), direction(LEFT),
moveSpeed(1.f), maxDistance(64.f), currentDistance(0.f) {
}

Proyectil::~Proyectil() {
    if (proyectilSprite) delete proyectilSprite;
}

void Proyectil::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& pos, ProyectilDirection dir) {
    tileMapDispl = tileMapPos;
    posProyectil = pos;
    direction = dir;

    // Cargar el spritesheet (64x16 píxeles, 4 frames de 16x16 píxeles)
    proyectilSpritesheet.loadFromFile("images/sprites/proyectilFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    proyectilSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 1.0), &proyectilSpritesheet, &shaderProgram);
    proyectilSprite->setNumberAnimations(4);  // Ahora hay 4 animaciones

    // Animación MOVE_LEFT (frame 1: proyectil hacia la izquierda)
    proyectilSprite->setAnimationSpeed(MOVE_LEFT, 1);
    proyectilSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0, 0.0));  // Frame 1

    // Animación MOVE_RIGHT (frame 2: proyectil hacia la derecha)
    proyectilSprite->setAnimationSpeed(MOVE_RIGHT, 1);
    proyectilSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.0));  // Frame 2

    // Animación MOVE_UP (frame 3: proyectil hacia arriba)
    proyectilSprite->setAnimationSpeed(MOVE_UP, 1);
    proyectilSprite->addKeyframe(MOVE_UP, glm::vec2(0.5, 0.0));  // Frame 3

    // Animación MOVE_DOWN (frame 4: proyectil hacia abajo)
    proyectilSprite->setAnimationSpeed(MOVE_DOWN, 1);
    proyectilSprite->addKeyframe(MOVE_DOWN, glm::vec2(0.75, 0.0));  // Frame 4

    // Establecer la animación según la dirección
    switch (direction) {
    case LEFT:
        proyectilSprite->changeAnimation(MOVE_LEFT);
        break;
    case RIGHT:
        proyectilSprite->changeAnimation(MOVE_RIGHT);
        break;
    case UP:
        proyectilSprite->changeAnimation(MOVE_UP);
        break;
    case DOWN:
        proyectilSprite->changeAnimation(MOVE_DOWN);
        break;
    }
    proyectilSprite->setPosition(glm::vec2(float(tileMapDispl.x + posProyectil.x), float(tileMapDispl.y + posProyectil.y)));
}

void Proyectil::update(int deltaTime) {
    proyectilSprite->update(deltaTime);

    // Mover el proyectil en la dirección especificada
    switch (direction) {
    case LEFT:
        posProyectil.x -= moveSpeed;
        break;
    case RIGHT:
        posProyectil.x += moveSpeed;
        break;
    case UP:
        posProyectil.y -= moveSpeed;
        break;
    case DOWN:
        posProyectil.y += moveSpeed;
        break;
    }
    currentDistance += moveSpeed;

    proyectilSprite->setPosition(glm::vec2(float(tileMapDispl.x + posProyectil.x), float(tileMapDispl.y + posProyectil.y)));
}

void Proyectil::render() {
    proyectilSprite->render();
}

void Proyectil::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

glm::vec2 Proyectil::getPosition() const {
    return posProyectil;
}

bool Proyectil::hasReachedMaxDistance() const {
    return currentDistance >= maxDistance;
}