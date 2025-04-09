#ifndef _SPIDER_INCLUDE
#define _SPIDER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <glm/glm.hpp>

class Spider {
public:
    enum SpiderState {
        MOVING_UP,
        MOVING_DOWN
    };

    enum SpiderAnimation {
        RUN_DOWN_1,
        RUN_UP_1,
        STOP_LEFT_DOWN,
        STOP_RIGHT_DOWN,
        STOP_LEFT_UP,
        STOP_RIGHT_UP
    };

    Spider();
    ~Spider();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float posX, float posY);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    SpiderState getState() const;
    Sprite* spiderSprite;
    void setStop(bool stop1);
    bool hasShot() const;  // Verificar si ya ha disparado
    void setShot(bool shot);  // Establecer si ha disparado
    bool isPlayerToLeft(float playerX) const;  // Determinar si el jugador está a la izquierda
    bool isPlayerInVerticalRange(float playerX, float playerY, float maxDistance) const;  // Nuevo método

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posSpider, initialPos;
    
    Texture spiderSpritesheet;
    TileMap* map;
    SpiderState state;

    float moveSpeed;
    float moveDistance;
    float currentDistance;
    bool stop;
    bool hasShotProyectil;  // Bandera para controlar si ya ha disparado

    const float TILE_SIZE = 16.f;
};

#endif // _SPIDER_INCLUDE