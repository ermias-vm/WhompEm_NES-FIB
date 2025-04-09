#ifndef _PROYECTIL_INCLUDE
#define _PROYECTIL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <glm/glm.hpp>

class Proyectil {
public:
    enum ProyectilDirection {
        LEFT,
        RIGHT,
        UP,    // Nueva dirección: hacia arriba
        DOWN   // Nueva dirección: hacia abajo
    };

    enum ProyectilAnimation {
        MOVE_LEFT,   // Frame 1: Proyectil hacia la izquierda
        MOVE_RIGHT,  // Frame 2: Proyectil hacia la derecha
        MOVE_UP,     // Frame 3: Proyectil hacia arriba
        MOVE_DOWN    // Frame 4: Proyectil hacia abajo
    };

    Proyectil();
    ~Proyectil();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& pos, ProyectilDirection dir);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    glm::vec2 getPosition() const;
    bool hasReachedMaxDistance() const;

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posProyectil;
    Sprite* proyectilSprite;
    Texture proyectilSpritesheet;
    TileMap* map;
    ProyectilDirection direction;

    float moveSpeed;
    float maxDistance;
    float currentDistance;
};

#endif // _PROYECTIL_INCLUDE