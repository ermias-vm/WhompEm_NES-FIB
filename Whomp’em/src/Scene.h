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
#include "Boss.h"

class Scene {
public:
    Scene();
    ~Scene();

    void init();
    void initPlatforms();
    void initBossBamboos();
    void initBamboosLaunch();
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
    Boss* boss;
    std::vector<Snake*> snakes;
    PlayerHUB* playerHub;
    std::vector<Platform*> platforms;
    std::vector<Bamboo*> bamboos;
    std::vector<Bamboo*> bambooslaunch;
    std::vector<Bamboo*> bossBamboos;

    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    float zoomFactor;
    glm::vec2 cameraPos;
    int cameraWidth = 16 * 16;
    int cameraHeight = 16 * 15;
    float cameraVx = 0.0f;
    float cameraVy = 0.0f;
    bool horitzontal;
    bool hasJumped = false;
    bool part1, part2, part3, part4, part5, part6, start;
    float fixedXVertical2 = 3072;
    float fixedYHorizontal = 0;
    float fixedXVertical = 2048;
    float fixedYHorizontal2 = 1440;
    float fixedYHorizontal3 = 480;
    bool bossready = false;

    const std::vector<float> bambooSpawnXPositions = { 1040.f, 1104.f, 1184.f, 1280.f };
    const std::vector<float> bambooSpawnLaunchPositions = { 3840.f, 3856, 3872, 3888.f,3904.f,3920.f,3936.f,3952.f,3968.f,3984.f };
    bool snakesSpawned = false;
};

#endif // _SCENE_INCLUDE

