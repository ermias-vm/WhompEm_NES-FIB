#ifndef CYCLOPE_H
#define CYCLOPE_H


#include "Sprite.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64.0f
#define PI 3.14159265

class Cyclope {
public:

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setPosition(const glm::vec2& pos);
    void setTileMap(TileMap* tileMap);
    glm::vec2 getPosition() const;

    void startJump();
    //void changeAnimToRightLeft(Sprite& sprite, int animation);

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posCyclope;
    Sprite* cyclopeSprite;
    Texture cyclopeSpritesheet;
    TileMap* map;


    bool isJumping, isLookingLeft;
    int jumpAngle;
    float startY;           
    glm::vec2 targetPos;    
    glm::vec2 jumpStartPos;
};

#endif // CYCLOPE_H