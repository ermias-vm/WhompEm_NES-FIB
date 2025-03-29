#ifndef _PLAYERHUB_INCLUDE
#define _PLAYERHUB_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class PlayerHUB
{

public:
	void init(const glm::ivec2& pos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);
	void setTileMap(TileMap* tileMap);


private:
	int playerHP, playerLifes;
	glm::ivec2 tileMapDispl, posHearts, posLifes, posTotem;
	Texture hubSpriteSheet;
	Sprite* heartSprite;
	TileMap* map;
};


#endif // _PLAYERHUB_INCLUDE

