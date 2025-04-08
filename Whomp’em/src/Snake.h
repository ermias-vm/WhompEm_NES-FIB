#ifndef SNAKE_H
#define SNAKE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"

class Snake {
public:
    Snake();
    ~Snake();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int direction, glm::vec2 pos);
    void update(int deltaTime);
    void render();
    void snakeJump();
    bool shouldDisappear() const;
    bool isjumping() const { return isJumping; }
    bool hasJumped() const { return hasJumpedOnce; } // Nuevo getter
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition();
    int getMovementDirection();

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posSnake, spawnPos;
    int movementDirection;
    float minpos, maxpos;
    Sprite* SnakeSprite;
    Texture SnakeSpritesheet;
    TileMap* map;
    float timeAccumulator;

    // Jump variables
    bool isJumping;
    bool hasJumpedOnce; // Nueva variable para rastrear si ya saltó
    float jumpTime;
    float jumpDuration = 500.f;
    float jumpHeight = 32.f;
    float jumpWidth = 48.f;
    glm::vec2 jumpStartPos;
};

#endif // SNAKE_H