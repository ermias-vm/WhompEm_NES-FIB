#ifndef BOSS_H
#define BOSS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"

class Boss {
public:
    Boss();
    ~Boss();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::vec2 pos);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition();
    bool shouldThrowBamboo() const;  // Determinar si debe lanzar un bamb�
    void resetBambooThrow();         // Nuevo m�todo para reiniciar el temporizador y contador
    void startFight();
    bool Boss::lounchBamboo();
    float& getBambooThrowTimer() { return bambooThrowTimer; }

private:
    enum BossState {
        WAITING,
        GROUND_INITIAL,  // Frame 1: En el suelo al inicio
        JUMPING_UP,      // Saltando hacia arriba (Frame 2)
        IN_AIR,          // En el aire haciendo tri�ngulos (Frames 2 y 3)
        FALLING,         // Cayendo con semic�rculos (Frame 5)
        GROUND_WAIT      // En el suelo despu�s de caer (Frame 4)
    };

    enum BossAnims {
        FRAME_1,  // Reposo inicial
        FRAME_2,  // En el aire (1)
        FRAME_3,  // En el aire (2)
        FRAME_4,  // En el suelo despu�s de caer
        FRAME_5   // Cayendo con semic�rculos
    };

    glm::ivec2 tileMapDispl;
    glm::vec2 posBoss, initialPos;
    Sprite* bossSprite;
    Texture bossSpritesheet;
    TileMap* map;
    BossState state;
    float timeAccumulator;
    bool isdown;
    // Variables de salto
    bool isJumping;
    float jumpTime;
    float jumpDuration = 500.f;  // Duraci�n del salto hacia arriba (0.5s)
    float jumpHeight = 96.f;     // Altura m�xima del salto
    glm::vec2 jumpStartPos;

    // Variables para el tiempo en el aire (tri�ngulos)
    float airTime;
    float airDuration = 3000.f;  // 3 segundos en el aire
    int airFrameToggle;          // Para alternar entre Frame 2 y Frame 3
    float airFrameTimer;
    float airFrameInterval = 500.f;  // Cambia de frame cada 0.5s
    float triangleTime;          // Tiempo dentro de un tri�ngulo
    float triangleDuration = 1500.f;  // 1.5 segundos por tri�ngulo
    int triangleCount;           // Contador de tri�ngulos completados
    glm::vec2 triangleStartPos;  // Posici�n inicial del tri�ngulo (centro de la base)
    float triangleHeight = 32.f; // Altura del v�rtice superior del tri�ngulo
    float triangleWidth = 120.f; // Ancho total del tri�ngulo (ajustado a 120.f)
    // Variables para el lanzamiento de bamb�es
    float bambooThrowTimer;      // Temporizador para el lanzamiento de bamb�es
    float bambooThrowInterval = 500.f;  


    // Variables para la ca�da
    float fallTime;
    float fallDuration = 1500.f;  // 1.5 segundos para caer
    float fallOscillationAmplitude = 32.f;  // Amplitud de los semic�rculos
    float fallOscillationFrequency = 2.f;   // Frecuencia de los semic�rculos

    // Variables para esperar en el suelo
    float groundWaitTime;
    float groundWaitDuration = 1000.f;  // 1 segundo de espera en el suelo
};

#endif // BOSS_H