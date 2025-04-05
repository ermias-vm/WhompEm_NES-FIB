#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "PlayerHUB.h"
#include "Snake.h"
#include "Platform.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void initPlatforms();
	void update(int deltaTime);
	void render();
	bool isOffScreen(Player* player);
	bool checkPlatformCollision(Player* player, Platform* platform);
	bool playerColisionPlatform();
	bool readyToJump();
	void handleSceneTransitions();
	glm::vec2 getPlayerOffset(Player* player);
	void setcameraPos(const glm::vec2& offset);
	void updateCamera(glm::vec2& posjugador,int deltaTime);
	glm::vec2 getCameraPos() const { return cameraPos; }
	bool CheckEnemyCollission();
	bool checkSpearCollisionWithSnake();

private:
	void initShaders();

private:
	TileMap *map;
	TileMap *foreground;
	Player *player;
	Snake *snake;
	PlayerHUB *playerHub;
	std::vector<Platform*> platforms;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float zoomFactor; // Factor de zoom
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
	float fixedYHorizontal = 0; // y = 10 * tileSize (10 * 32 = 320, para el pasadizo superior)
	float fixedXVertical = 2048;  // x = 2082 (inicio del pasadizo vertical)
	float fixedYHorizontal2 = 1440; // y = 140 * tileSize (140 * 32 = 4480, para el pasadizo inferior)
	float fixedYHorizontal3 = 480;
};


#endif // _SCENE_INCLUDE

