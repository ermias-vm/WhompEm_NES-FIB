#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:

	/**
	* @brief Inicializa el jugador configurando su sprite y animaciones.
	* @param tileMapPos Posición del tile map para calcular el desplazamiento del sprite.
	* @param shaderProgram Programa de shader empleado para renderizar el sprite.
	*/
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);

	void update(int deltaTime);

	void render();
	void setTileMap(TileMap *tileMap);

	/**
	* @brief Establece la posición inicial del sprite.
	* @param tileMapDispl Desplazamiento del mapa.
	* @param posPlayer Posición del jugador.
	*/
	void setPosition(const glm::vec2 &pos);
	glm::vec2 Player::getPosition();
	glm::vec2 Player::getVelocity();

	void changeAnimToRightLeft(Sprite &sprite, int animation);
	void Player::righLeftKeyPressed(int isLeftAnim);
	void Player::righLeftKeyReleased(int isLeftAnim);
	void Player::printAnimName(Sprite* sprite, int animation);
	
private:
	bool bJumping, bCrouching, bLookingRight, bAttacking, bDamaged, bFalling;
	bool is_Z_pressed, is_Right_pressed, is_Left_pressed;
	bool bRenderingSpear;
	bool bAttackCharged;
	int jumpAngle, startY, spearDist;
	glm::ivec2 tileMapDispl, posPlayer;
	Texture playerSpritesheet, spearSpritesheet;
	Sprite *playerSprite, *spearSprite;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


