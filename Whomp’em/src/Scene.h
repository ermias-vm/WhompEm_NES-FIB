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
#include "Cyclope.h"
#include "Spider.h"
#include "Proyectil.h"

class Scene {
public:
    Scene();
    ~Scene();

    enum CollisionType {
        NONE = 0,
        LEFT = 1,
        RIGHT = 2,
        TOP = 3,
        BOTTOM = 4
    };

    void init();
    void initPlatforms();
    void initBossBamboos();
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
    bool checkSpearCollisionWithSnake(Snake* snake);
    glm::vec2 getPlayerPos() const;
    void checkSpiderPlayerInteraction();
    bool checkProyectilCollision(Proyectil* proyectil);
    CollisionType checkCollisionWithEnemy(const glm::ivec2& enemyPos, const glm::ivec2& enemySize);

	void collisionsSpearEnemy();
	void checkAllCollisions(int deltaTime);
	void platformMgmt(int deltaTime);
    void cyclopeMgmt(int deltaTime);
    void bossMgmt(int deltaTime);
    void snakeMgmt(int deltaTime);
	void spiderMgmt(int deltaTime);
    void updateSprites(int deltaTime);

private:
    void initShaders();
    void initBamboos();
    void initSpiders();
    void initProyectiles();

private:
    TileMap* map;
    TileMap* foreground;
    Player* player;
    Boss* boss;
    PlayerHUB* playerHub;

	Cyclope* cyclope;

    std::vector<Snake*> snakes;
    std::vector<Platform*> platforms;
    std::vector<Bamboo*> bamboos;
    std::vector<Bamboo*> bambooslaunch;
    std::vector<Bamboo*> bossBamboos;
    std::vector<Spider*> spiders;
    std::vector<Proyectil*> proyectiles;

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
    bool hasLaunchedLine = false;
    const std::vector<float> bambooSpawnXPositions = { 1040.f, 1104.f, 1184.f, 1280.f };
    const std::vector<float> bambooSpawnLaunchPositions = { 3840.f, 3856, 3872, 3888.f,3904.f,3920.f,3936.f,3952.f,3968.f,3984.f,4000.f,4016.f,4032.f,4048.f,4064,4080 };
    bool snakesSpawned = false;
};

#endif // _SCENE_INCLUDE

