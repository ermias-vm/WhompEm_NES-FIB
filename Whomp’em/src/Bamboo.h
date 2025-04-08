#ifndef BAMBOO_H
#define BAMBOO_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"

class Bamboo {
public:
    Bamboo();
    ~Bamboo();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& startPos);
    void update(int deltaTime);
    void render();
    void reset(); 
    void setTileMap(TileMap* tileMap);
    glm::vec2 getPosition() const;
    bool isActive() const;
    bool checkCollisionWithPlayer(const glm::vec2& playerPos, const glm::ivec2& playerSize);

private:
    glm::vec2 position;
    glm::vec2 velocity;
    Sprite* sprite;
    TileMap* map;
    glm::vec2 initialPosition; 
    ShaderProgram* texProgram;
    bool active;
    Texture spritesheet;
};

#endif // BAMBOO_H