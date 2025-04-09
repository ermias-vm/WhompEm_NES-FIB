#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Spider.h"

Spider::Spider() : spiderSprite(nullptr), map(nullptr), state(MOVING_DOWN),
moveSpeed(1.f), moveDistance(80.f), currentDistance(0.f),
stop(false), hasShotProyectil(false) {
}

Spider::~Spider() {
    if (spiderSprite) delete spiderSprite;
}

void Spider::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float posX, float posY) {
    tileMapDispl = tileMapPos;
    posSpider = glm::vec2(posX, posY);
    initialPos = posSpider;

    // Cargar el spritesheet (128x16 píxeles, 8 frames de 16x16 píxeles)
    spiderSpritesheet.loadFromFile("images/sprites/spiderFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spiderSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1.0), &spiderSpritesheet, &shaderProgram);
    spiderSprite->setNumberAnimations(6);

    // Animación RUN_DOWN_1 (frames 1 y 2: correr hacia abajo)
    spiderSprite->setAnimationSpeed(RUN_DOWN_1, 8);
    spiderSprite->addKeyframe(RUN_DOWN_1, glm::vec2(0.0, 0.0));  // Frame 1
    spiderSprite->addKeyframe(RUN_DOWN_1, glm::vec2(0.125, 0.0));  // Frame 2

    // Animación RUN_UP_1 (frames 3 y 4: correr hacia arriba)
    spiderSprite->setAnimationSpeed(RUN_UP_1, 8);
    spiderSprite->addKeyframe(RUN_UP_1, glm::vec2(0.25, 0.0));  // Frame 3
    spiderSprite->addKeyframe(RUN_UP_1, glm::vec2(0.375, 0.0));  // Frame 4

    // Animación STOP_LEFT_DOWN (frame 5: parada, player a la izquierda, moviéndose hacia abajo)
    spiderSprite->setAnimationSpeed(STOP_LEFT_DOWN, 1);
    spiderSprite->addKeyframe(STOP_LEFT_DOWN, glm::vec2(0.5, 0.0));  // Frame 5

    // Animación STOP_RIGHT_DOWN (frame 6: parada, player a la derecha, moviéndose hacia abajo)
    spiderSprite->setAnimationSpeed(STOP_RIGHT_DOWN, 1);
    spiderSprite->addKeyframe(STOP_RIGHT_DOWN, glm::vec2(0.625, 0.0));  // Frame 6

    // Animación STOP_LEFT_UP (frame 7: parada, player a la izquierda, moviéndose hacia arriba)
    spiderSprite->setAnimationSpeed(STOP_LEFT_UP, 1);
    spiderSprite->addKeyframe(STOP_LEFT_UP, glm::vec2(0.75, 0.0));  // Frame 7

    // Animación STOP_RIGHT_UP (frame 8: parada, player a la derecha, moviéndose hacia arriba)
    spiderSprite->setAnimationSpeed(STOP_RIGHT_UP, 1);
    spiderSprite->addKeyframe(STOP_RIGHT_UP, glm::vec2(0.875, 0.0));  // Frame 8

    // Iniciar con la animación de correr hacia abajo
    spiderSprite->changeAnimation(RUN_DOWN_1);
    spiderSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
}

void Spider::update(int deltaTime) {
    spiderSprite->update(deltaTime);
    if (!stop) {
        // Mover la araña verticalmente
        if (state == MOVING_DOWN) {
            posSpider.y += moveSpeed;
            currentDistance += moveSpeed;
            spiderSprite->changeAnimation(RUN_DOWN_1);
            if (currentDistance >= moveDistance) {
                state = MOVING_UP;
                currentDistance = 0.f;
            }
        }
        else if (state == MOVING_UP) {
            posSpider.y -= moveSpeed;
            currentDistance += moveSpeed;
            spiderSprite->changeAnimation(RUN_UP_1);
            if (currentDistance >= moveDistance) {
                state = MOVING_DOWN;
                currentDistance = 0.f;
            }
        }
    }

    spiderSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
}

bool Spider::isPlayerInVerticalRange(float playerX, float playerY, float maxDistance) const {
    // Verificar si están en la misma X (con una tolerancia de 8 píxeles)
    if (std::abs(playerX - posSpider.x) > 8.f) {
        return false;
    }

    // Verificar la diferencia en Y
    float yDiff = playerY - posSpider.y;
    if (state == MOVING_UP) {
        // Si la araña está moviéndose hacia arriba, el jugador debe estar arriba (Y negativa)
        return yDiff < 0 && std::abs(yDiff) <= maxDistance;
    }
    else if (state == MOVING_DOWN) {
        // Si la araña está moviéndose hacia abajo, el jugador debe estar abajo (Y positiva)
        return yDiff > 0 && std::abs(yDiff) <= maxDistance;
    }
    return false;
}

bool Spider::hasShot() const {
    return hasShotProyectil;
}

void Spider::setShot(bool shot) {
    hasShotProyectil = shot;
}

bool Spider::isPlayerToLeft(float playerX) const {
    return playerX < posSpider.x;
}


void Spider::setStop(bool stop1) {
    this->stop = stop1;
}
void Spider::render() {
    spiderSprite->render();
}

void Spider::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Spider::setPosition(const glm::vec2& pos) {
    posSpider = pos;
    initialPos = pos;
    spiderSprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
}

glm::vec2 Spider::getPosition() const {
    return posSpider;
}

Spider::SpiderState Spider::getState() const {
    return state;
}