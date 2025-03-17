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
	void updateCamera(glm::vec2& posjugador);


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
};


#endif // _SCENE_INCLUDE

