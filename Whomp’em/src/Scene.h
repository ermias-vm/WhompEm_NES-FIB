#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	bool isOffScreen(Player* player);
	glm::vec2 getPlayerOffset(Player* player);
	void setcameraPos(const glm::vec2& offset);
	void updateCamera(glm::vec2& posjugador,int deltaTime);


private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float zoomFactor; // Factor de zoom
	glm::vec2 cameraPos; 
	int cameraWidth = 16 * 16;
	int cameraHeight = 16 * 15;
	float cameraVx = 0.0f;
	float cameraVy = 0.0f;
	bool horitzontal;
	bool part1 = false;
	bool part2 = false;
	bool part3 = false;
	bool part4 = false;
	float fixedXVertical2 = 3072;
	float fixedYHorizontal = 0; // y = 10 * tileSize (10 * 32 = 320, para el pasadizo superior)
	float fixedXVertical = 2048;  // x = 2082 (inicio del pasadizo vertical)
	float fixedYHorizontal2 = 1440; // y = 140 * tileSize (140 * 32 = 4480, para el pasadizo inferior)
	float fixedYHorizontal3 = 480;
};


#endif // _SCENE_INCLUDE

