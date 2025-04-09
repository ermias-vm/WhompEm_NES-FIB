#ifndef CYCLOPE_H
#define CYCLOPE_H

#include "Sprite.h"
#include "TileMap.h"

class Cyclope {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setPosition(const glm::ivec2& pos);
    void setTileMap(TileMap* tileMap);
    glm::ivec2 getPosition() const;
    bool isAlive() const { return cyclopeHP > 0; }
    void takeDamage(int damage) { cyclopeHP -= damage; }


private:
    void changeAnimToRightLeft(int animation);

    glm::ivec2 tileMapDispl;
    glm::ivec2 posCyclope;
    Sprite* cyclopeSprite;
    Texture cyclopeSpritesheet;
    TileMap* map;

    bool isJumping;
    bool bLookingLeft;  // Para determinar si mira a la izquierda
    float jumpTime;
    glm::ivec2 jumpStartPos;
    int jumpDirection;
    float timeOnGround;
    int jumpWidth, cyclopeHP;
};

#endif // CYCLOPE_H