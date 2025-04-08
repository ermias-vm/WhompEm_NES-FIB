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
	bool isPlayerDead() const;
	void modifyPlayerHP(int amount, bool addHeart);
	int getPlayerHP() const;
	int getPlayerLifes() const;
	void setTotemAnimation(bool bUsingTotem);
	void restoreLivesHp();
	void checkCheats();
	bool isGodMode() const { return godMode; }

private:
	int playerHP, playerLifes;
	bool godMode, is_H_pressed, is_G_pressed;
	glm::ivec2 tileMapDispl, posHearts, posLifes, posTotem;
	Texture hubSpriteSheet;
	Sprite* heartSprite;
	Sprite* lifeSprite;
	Sprite* totemSprite;
	TileMap* map;
};


#endif // _PLAYERHUB_INCLUDE

