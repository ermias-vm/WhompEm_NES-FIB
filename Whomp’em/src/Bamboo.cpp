#include "Bamboo.h"
#include "Sprite.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

Bamboo::Bamboo() : sprite(nullptr), map(nullptr), texProgram(nullptr), active(false) {
}

Bamboo::~Bamboo() {
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }
}

void Bamboo::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& startPos) {
    texProgram = &shaderProgram;
    position = startPos;
    initialPosition = startPos;
    velocity = glm::vec2(0.f, 16 * 8.f); //8 tile per sec
    active = true;

    if (!spritesheet.loadFromFile("images/sprites/bambooFrame.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "Error loading bambooFrame.png" << std::endl;
        active = false;
        return;
    }
    std::cout << "Bamboo texture loaded successfully" << std::endl;

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &spritesheet, texProgram);
    if (!sprite) {
        std::cerr << "Error creating bamboo sprite" << std::endl;
        active = false;
        return;
    }
    std::cout << "Bamboo sprite created successfully" << std::endl;

    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);
    sprite->setPosition(position);
}

void Bamboo::update(int deltaTime) {
    if (!active) return;

    float deltaTimeSec = deltaTime / 1000.f;
    position += velocity * deltaTimeSec;

    if (position.y > 16 * 11) {
        active = false;
        return;
    }

    sprite->setPosition(position);
}
void Bamboo::reset() {
    position = initialPosition; // Reset to initial position (e.g., (1040, 0))
    active = true;
    sprite->setPosition(position);
}
void Bamboo::render() {
    if (active && sprite) {
        sprite->render();
    }
}

void Bamboo::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

glm::vec2 Bamboo::getPosition() const {
    return position;
}

bool Bamboo::isActive() const {
    return active;
}

bool Bamboo::checkCollisionWithPlayer(const glm::vec2& playerPos, const glm::ivec2& playerSize) {
    if (!active) return false;

    glm::ivec2 bambooSize(16, 16);

    if (position.x + bambooSize.x > playerPos.x &&
        position.x < playerPos.x + playerSize.x &&
        position.y + bambooSize.y > playerPos.y &&
        position.y < playerPos.y + playerSize.y) {
        active = false;
        return true;
    }
    return false;
}