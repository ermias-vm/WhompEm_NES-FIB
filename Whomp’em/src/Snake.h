#ifndef _SNAKE_INCLUDE
#define _SNAKE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Snake is basically a Sprite that represents the Snake. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Snake
{

public:

	/**
	* @brief Inicializa el jugador configurando su sprite y animaciones.
	* @param tileMapPos Posici�n del tile map para calcular el desplazamiento del sprite.
	* @param shaderProgram Programa de shader empleado para renderizar el sprite.
	*/
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram,int direction,glm::vec2 pos);
	void update(int deltaTime);

	void render();
	void setTileMap(TileMap* tileMap);

	/**
	* @brief Establece la posici�n inicial del sprite.
	* @param tileMapDispl Desplazamiento del mapa.
	* @param posSnake Posici�n del jugador.
	*/
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	void update(int deltatime, int count);
	int max = 2;
	int count;
	int movementCooldown;

	void changeAnimToRightLeft(Sprite& sprite, int animation);
	void righLeftKeyPressed();
	void snakeJump();
	void righLeftKeyReleased();
	void printAnimName(Sprite* sprite, int animation);
	bool shouldDisappear() const;
	int getMovementDirection();
	

private:
	glm::vec2 spawnPos;
	bool bLookingLeft, bCrouching;
	bool isJumping = false;
	float jumpTime = 0.0f;
	glm::vec2 jumpStartPos;
	float jumpDuration = 500.0f; // 0.5 seconds
	float jumpHeight = 16.0f;    // 1 tile
	float jumpWidth = 48.0f;     // 3 tiles
	float timeAccumulator; // Acumulador de tiempo
	bool bDamaged, b_X_Attacking;
	int movementDirection;
	int startY;
	int maxpos;
	int minpos;
	float SnakeSpeed;
	glm::ivec2 tileMapDispl;
	glm::vec2 posSnake;
	Texture SnakeSpritesheet, spearSpritesheet;
	Sprite* SnakeSprite, * spearSprite;
	TileMap* map;
};

#endif // _Snake_INCLUDE
