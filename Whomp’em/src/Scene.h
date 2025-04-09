#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "PlayerHUB.h"
#include "Snake.h"
#include "Bamboo.h"
#include "Platform.h"

class Scene {
public:
    Scene();
    ~Scene();

    void init();
    void initPlatforms();
    void update(int deltaTime);
    void render();
    bool isOffScreen(glm::vec2 pos);
    bool checkPlatformCollision(Player* player, Platform* platform);
    bool playerCollisionPlatform();
    bool readyToJump(Snake* snake);
    void handleSceneTransitions();
    glm::vec2 getPlayerOffset(Player* player);
    void setcameraPos(const glm::vec2& offset);
    void updateCamera(glm::vec2& posjugador, int deltaTime);
    glm::vec2 getCameraPos() const { return cameraPos; }
    bool CheckEnemyCollission(Snake* snake);
    bool checkSpearCollisionWithSnake(Snake* snake);

private:
    void initShaders();
    void initBamboos();

private:
    TileMap* map;
    TileMap* foreground;
    Player* player;
    std::vector<Snake*> snakes;
    PlayerHUB* playerHub;
    std::vector<Platform*> platforms;
    std::vector<Bamboo*> bamboos;

    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    float zoomFactor;
    int damagecooldown = 0;
    glm::vec2 cameraPos;
    int cameraWidth = 16 * 16;
    int cameraHeight = 16 * 15;
    float cameraVx = 0.0f;
    float cameraVy = 0.0f;
    bool horitzontal;
    bool hasJumped = false;
    bool part1, part2, part3, part4;
    float fixedXVertical2 = 3072;
    float fixedYHorizontal = 0;
    float fixedXVertical = 2048;
    float fixedYHorizontal2 = 1440;
    float fixedYHorizontal3 = 480;

    const std::vector<float> bambooSpawnXPositions = { 1040.f, 1104.f, 1184.f, 1280.f };
    bool snakesSpawned = false;
};

#endif // _SCENE_INCLUDE