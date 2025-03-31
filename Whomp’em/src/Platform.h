#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Platform
{

public:
	void init(const glm::ivec2& pos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);
	void setTileMap(TileMap* tileMap);

	


private:
	glm::ivec2 tileMapDispl, posPlatform;
	Texture platformSpriteSheet;
	Sprite* platformSprite;
	TileMap* map;
};


#endif // _Platform_INCLUDE

