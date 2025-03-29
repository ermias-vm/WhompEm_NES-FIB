#ifndef _SNAKE_INCLUDE
#define _SNAKE_INCLUDE


#include "src/Sprite.h"
#include "src/TileMap.h"
#include <glm/detail/type_vec.hpp>
#include "src/ShaderProgram.h"
#include "src/TileMap.h"


// Snake is basically a Sprite that represents the Snake. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Snake
{

public:

	/**
	* @brief Inicializa el jugador configurando su sprite y animaciones.
	* @param tileMapPos Posición del tile map para calcular el desplazamiento del sprite.
	* @param shaderProgram Programa de shader empleado para renderizar el sprite.
	*/
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

	void update(int deltaTime);

	void render();
	void setTileMap(TileMap* tileMap);

	/**
	* @brief Establece la posición inicial del sprite.
	* @param tileMapDispl Desplazamiento del mapa.
	* @param posSnake Posición del jugador.
	*/
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	void update(int deltatime, int count);
	bool right;
	int max = 2;
	int count;
	int movementCooldown;

	void changeAnimToRightLeft(Sprite& sprite, int animation);
	void righLeftKeyPressed();
	void righLeftKeyReleased();
	void printAnimName(Sprite* sprite, int animation);

private:
	bool bLookingLeft, bCrouching;
	bool bDamaged, b_X_Attacking;

	int startY;
	float SnakeSpeed;
	glm::ivec2 tileMapDispl, posSnake;
	Texture SnakeSpritesheet, spearSpritesheet;
	Sprite* SnakeSprite, * spearSprite;
	TileMap* map;
};


#endif // _Snake_INCLUDE

