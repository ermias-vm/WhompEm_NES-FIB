#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "PlayerHUB.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:

	/**
	* @brief Inicializa el jugador configurando su sprite y animaciones.
	* @param tileMapPos Posici�n del tile map para calcular el desplazamiento del sprite.
	* @param shaderProgram Programa de shader empleado para renderizar el sprite.
	*/
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

	void update(int deltaTime);

	void render();
	void setTileMap(TileMap* tileMap);

	/**
	* @brief Establece la posici�n inicial del sprite.
	* @param tileMapDispl Desplazamiento del mapa.
	* @param posPlayer Posici�n del jugador.
	*/
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();
	glm::vec2 getVelocity();

	void changeAnimToRightLeft(Sprite& sprite, int animation);
	void righLeftKeyPressed();
	void righLeftKeyReleased();
	void printAnimName(Sprite* sprite, int animation);
	void setAnimations(ShaderProgram& shaderProgram);

	void setPlayerHUB(PlayerHUB* hub);
	void takeDamage(int damage);
	void heal(int hp);
	void addHeart();
	bool isPlayerDead() const;
	void setFalling(bool falling) { bFalling = falling; }

	bool collisionPlatform();
	bool isAttacking() const;
	bool Crouching() const;
	bool lookingleft() const;
	void checkCheats();
	bool isGodMode() const;
	bool isOnCooldown() const;
	bool isBlocking() const;
	bool usingFireTotem() const { return bUsingTotem; }

private:
	bool bJumping, bFalling, bLookingLeft, bCrouching, bUsingTotem, is_DOWN_pressed;
	bool bDamaged, b_X_Attacking;
	bool is_Z_pressed, is_Right_pressed, is_Left_pressed, is_UP_pressed, is_T_pressed;
	bool is_V_pressed, is_O_pressed, is_L_pressed, is_I_pressed;

	int jumpAngle, startY, spearDist;
	int damageCooldown, timeSinceLastDamage;
	float playerSpeed;
	glm::ivec2 tileMapDispl, posPlayer;
	Texture playerSpritesheet, spearSpritesheet;
	Sprite* playerSprite, * spearSprite;
	TileMap* map;
	PlayerHUB* playerHub;
};


#endif // _PLAYER_INCLUDE


