#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Platform
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float startY, float range, float speed);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);
	void setTileMap(TileMap* tileMap);
	//
	float getDeltaY() const { return deltaY; }
	glm::vec2 getPosition() const { return posPlatform; }
	glm::ivec2 getSize() const { return glm::ivec2(32, 32); } // Tamaño de la plataforma

	


private:
	glm::ivec2 tileMapDispl, posPlatform;
	Texture platformSpriteSheet;
	Sprite* platformSprite;
	TileMap* map;

	// Variables para el movimiento vertical
	float startY;       // Posición Y inicial
	float range;        // Rango de movimiento (ej: 100 unidades)
	float speed;        // Velocidad de movimiento
	int direction;      // Dirección (1 = subir, -1 = bajar)
	float deltaY;       // Delta de movimiento en Y (para sincronizar con el jugador)
};


#endif // _Platform_INCLUDE

