#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 3.5
#define PI 3.14159265
#define ANIMATION_SPEED 20
#define SPEAR_ANIMATION_SPEED 30
#define SIZE_X 25


enum PlayerAnims
{
    STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, JUMP_RIGHT, JUMP_LEFT,
    CROUCH_RIGHT, CROUCH_LEFT, DAMAGED_RIGHT, DAMAGED_LEFT, COVER_RIGHT, COVER_LEFT,
    HOLD_SPEAR_R, HOLD_SPEAR_L, CHARGE_RIGHT, CHARGE_LEFT, HOLD_SPEAR_CROUCH_R, HOLD_SPEAR_CROUCH_L,
    ATTACK_DOWN_R, ATTACK_DOWN_L, ATTACK_UP_R, ATTACK_UP_L, DIE_RIGHT, DIE_LEFT
};

enum PlayerLargeAnims
{
    SPEAR_ATTACK_RIGHT, SPEAR_ATTACK_LEFT, SPEAR_ATTACK_CROUCH_R, SPEAR_ATTACK_CROUCH_L,
    SPEAR_PASSIVE_R, SPEAR_PASSIVE_L, SPEAR_PASSIVE_CROUCH_R, SPEAR_PASSIVE_CROUCH_L, NONE, NONE2
};



void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

    bJumping = bFalling = b_X_Attacking = bDamaged = bFalling = false;
    bLookingLeft = bCrouching = bUsingTotem = false;
    is_Z_pressed = is_Right_pressed = is_Left_pressed = is_UP_pressed = is_DOWN_pressed = is_V_pressed = false;
    is_T_pressed = is_O_pressed = is_P_pressed = is_I_pressed = false;
    playerSpeed = 2.f;

    // ANIMACIONES PLAYER //

    playerSpritesheet.loadFromFile("images/sprites/playerFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //(pixels frame, %espaci� ocupado(ancho,alto), playerSpritesheet, shaderProgram)
    playerSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.05), &playerSpritesheet, &shaderProgram);
    playerSprite->setNumberAnimations(30);

    playerSprite->setAnimationSpeed(STAND_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

    playerSprite->setAnimationSpeed(STAND_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));

    playerSprite->setAnimationSpeed(MOVE_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.05f));
    playerSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.05f));
    playerSprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.05f));

    playerSprite->setAnimationSpeed(MOVE_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0, 0.10f));
    playerSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.10f));
    playerSprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50, 0.10f));

    playerSprite->setAnimationSpeed(JUMP_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.15f));

    playerSprite->setAnimationSpeed(JUMP_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.15f));

    playerSprite->setAnimationSpeed(CROUCH_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.0f, 0.15f));

    playerSprite->setAnimationSpeed(CROUCH_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.25f, 0.15f));

    playerSprite->setAnimationSpeed(DAMAGED_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(DAMAGED_RIGHT, glm::vec2(0.0f, 0.20f));

    playerSprite->setAnimationSpeed(DAMAGED_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(DAMAGED_LEFT, glm::vec2(0.25f, 0.20f));

    playerSprite->setAnimationSpeed(COVER_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(COVER_RIGHT, glm::vec2(0.0f, 0.25f));

    playerSprite->setAnimationSpeed(COVER_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(COVER_LEFT, glm::vec2(0.25f, 0.25f));

    playerSprite->setAnimationSpeed(HOLD_SPEAR_R, ANIMATION_SPEED);
    playerSprite->addKeyframe(HOLD_SPEAR_R, glm::vec2(0.0f, 0.30f));

    playerSprite->setAnimationSpeed(HOLD_SPEAR_L, ANIMATION_SPEED);
    playerSprite->addKeyframe(HOLD_SPEAR_L, glm::vec2(0.25f, 0.30f));

    playerSprite->setAnimationSpeed(CHARGE_RIGHT, ANIMATION_SPEED);
    playerSprite->addKeyframe(CHARGE_RIGHT, glm::vec2(0.0, 0.35f));
    playerSprite->addKeyframe(CHARGE_RIGHT, glm::vec2(0.25, 0.35f));
    playerSprite->addKeyframe(CHARGE_RIGHT, glm::vec2(0.50, 0.35f));

    playerSprite->setAnimationSpeed(CHARGE_LEFT, ANIMATION_SPEED);
    playerSprite->addKeyframe(CHARGE_LEFT, glm::vec2(0.0, 0.40f));
    playerSprite->addKeyframe(CHARGE_LEFT, glm::vec2(0.25, 0.40f));
    playerSprite->addKeyframe(CHARGE_LEFT, glm::vec2(0.50, 0.40f));

    playerSprite->setAnimationSpeed(HOLD_SPEAR_CROUCH_R, ANIMATION_SPEED);
    playerSprite->addKeyframe(HOLD_SPEAR_CROUCH_R, glm::vec2(0.0f, 0.45f));

    playerSprite->setAnimationSpeed(HOLD_SPEAR_CROUCH_L, ANIMATION_SPEED);
    playerSprite->addKeyframe(HOLD_SPEAR_CROUCH_L, glm::vec2(0.25f, 0.45f));

    playerSprite->setAnimationSpeed(ATTACK_DOWN_R, ANIMATION_SPEED);
    playerSprite->addKeyframe(ATTACK_DOWN_R, glm::vec2(0.0f, 0.50f));

    playerSprite->setAnimationSpeed(ATTACK_DOWN_L, ANIMATION_SPEED);
    playerSprite->addKeyframe(ATTACK_DOWN_L, glm::vec2(0.25f, 0.50f));

    playerSprite->setAnimationSpeed(ATTACK_UP_R, ANIMATION_SPEED);
    playerSprite->addKeyframe(ATTACK_UP_R, glm::vec2(0.0f, 0.55f));

    playerSprite->setAnimationSpeed(ATTACK_UP_L, ANIMATION_SPEED);
    playerSprite->addKeyframe(ATTACK_UP_L, glm::vec2(0.25f, 0.55f));


    // ANIMACIONES DE LA LANZA //

    spearSpritesheet.loadFromFile("images/sprites/spearFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spearSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.20f, 0.10f), &spearSpritesheet, &shaderProgram);  // Tama�o ajustado para ataque
    spearSprite->setNumberAnimations(15);

    spearSprite->setAnimationSpeed(SPEAR_ATTACK_RIGHT, SPEAR_ANIMATION_SPEED);
    spearSprite->setAnimationAcyclic(SPEAR_ATTACK_RIGHT);
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.0, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.2, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.4, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.6, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.4, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.2, 0.0f));
    spearSprite->addKeyframe(SPEAR_ATTACK_RIGHT, glm::vec2(0.0, 0.0f));

    spearSprite->setAnimationSpeed(SPEAR_ATTACK_LEFT, SPEAR_ANIMATION_SPEED);
    spearSprite->setAnimationAcyclic(SPEAR_ATTACK_LEFT);
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.0, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.2, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.4, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.6, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.4, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.2, 0.1f));
    spearSprite->addKeyframe(SPEAR_ATTACK_LEFT, glm::vec2(0.0, 0.1f));

    spearSprite->setAnimationSpeed(SPEAR_ATTACK_CROUCH_R, SPEAR_ANIMATION_SPEED);
    spearSprite->setAnimationAcyclic(SPEAR_ATTACK_CROUCH_R);
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.0, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.2, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.4, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.6, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.4, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.2, 0.2f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_R, glm::vec2(0.0, 0.2f));

    spearSprite->setAnimationSpeed(SPEAR_ATTACK_CROUCH_L, SPEAR_ANIMATION_SPEED);
    spearSprite->setAnimationAcyclic(SPEAR_ATTACK_CROUCH_L);
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.0, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.2, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.4, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.6, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.4, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.2, 0.3f));
    spearSprite->addKeyframe(SPEAR_ATTACK_CROUCH_L, glm::vec2(0.0, 0.3f));


    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_R, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_R, glm::vec2(0.0, 0.0f));

    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_L, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_L, glm::vec2(0.0, 0.1f));

    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_CROUCH_R, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_CROUCH_R, glm::vec2(0.0, 0.2f));

    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_CROUCH_L, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_CROUCH_L, glm::vec2(0.0, 0.3f));

    spearSprite->setAnimationSpeed(NONE, SPEAR_ANIMATION_SPEED);  //TODO: MODIFICAR A NO RENDERIZAR
    spearSprite->addKeyframe(NONE, glm::vec2(0.8, 0.0f));

    spearSprite->setAnimationSpeed(NONE2, SPEAR_ANIMATION_SPEED);  //TODO: MODIFICAR A NO RENDERIZAR
    spearSprite->addKeyframe(NONE2, glm::vec2(0.8, 0.0f));

    changeAnimToRightLeft(*playerSprite, STAND_RIGHT);  // Animaci�n inicial
    tileMapDispl = tileMapPos;				            // Desplazamiento del tile map (0,0)
    playerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    spearSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + spearDist), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::colisionPlatform() {
    Scene* scene = Game::instance().getScene();
	return scene->playerColisionPlatform();
}
void Player::changeAnimToRightLeft(Sprite& sprite, int animation) {
    if (bLookingLeft) {
        animation++;
        spearDist = -32;
    }
    else {
        spearDist = 32;
    }
    printAnimName(&sprite, animation);
    sprite.changeAnimation(animation);
}

void Player::righLeftKeyPressed() {
    if (b_X_Attacking) return;
    int playerAnim = playerSprite->animation();
    if (!bFalling && !bJumping) {
        if (bCrouching) {
            if (playerAnim != (CROUCH_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
            }
        }
        else if (Game::instance().getKey(GLFW_KEY_UP)) {
            if (playerAnim != (COVER_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, COVER_RIGHT);
            }
        }
        else {
            if (playerAnim != (MOVE_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, MOVE_RIGHT);
            }
        }
    }
    else {
        if (playerAnim != (CROUCH_RIGHT + bLookingLeft)) {
            if (playerAnim != (ATTACK_UP_R + bLookingLeft) && playerAnim != (ATTACK_DOWN_R + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
            }
        }
    }
}

void Player::righLeftKeyReleased() {
    if (b_X_Attacking) return;
    int playerAnim = playerSprite->animation();
    // Si a�n se mantiene UP presionada, mantener COVER
    if (Game::instance().getKey(GLFW_KEY_UP)) {
        if (playerAnim != (COVER_RIGHT + bLookingLeft)) {
            changeAnimToRightLeft(*playerSprite, COVER_RIGHT);
        }
    }
    else {
        if (!bFalling && !bJumping) {
            if (bCrouching) {
                if (playerAnim != (CROUCH_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
                }
            }
            else {
                if (playerAnim != (STAND_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
                }
            }
        }
        else {
            if (playerAnim != (CROUCH_RIGHT + bLookingLeft)) {
                if (playerAnim != (ATTACK_UP_R + bLookingLeft) && playerAnim != (ATTACK_DOWN_R + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
                }
            }
        }
    }
}

void Player::update(int deltaTime)
{

    spearSprite->update(deltaTime);
    playerSprite->update(deltaTime);

    if (Game::instance().getKey(GLFW_KEY_X)) {
        int playerAnim = playerSprite->animation();
        int spearAnim = spearSprite->animation();
        if (!b_X_Attacking) {             // Iniciar el ataque al presionar 'X' si no se estava ejecutando
            cout << "--------------------  X  (Attacking)" << endl;

            b_X_Attacking = true;
            if (!bJumping && !bFalling) {
                if (is_Right_pressed || is_Left_pressed) {
                    if (playerAnim != (CHARGE_RIGHT + bLookingLeft)) {
                        changeAnimToRightLeft(*playerSprite, CHARGE_RIGHT);
                        changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_RIGHT);
                    }
                }
                else if (is_DOWN_pressed) {
                    if (playerAnim != (HOLD_SPEAR_CROUCH_R + bLookingLeft)) {
                        changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
                        changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_CROUCH_R);
                    }
                }
                else {
                    changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_R);
                    changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_RIGHT);
                }

            }
            else {
                changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
                changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_CROUCH_R);
            }
        }
        else if (!bJumping && !bFalling) { // ANIMACIONES cuando ya se ha atacado y player (EN TIERRA) pero se mantiene X presionada)
            if (is_Right_pressed || is_Left_pressed) {
                if (playerAnim != (CHARGE_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, CHARGE_RIGHT);
                    changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_R);
                }
            }
            else if (is_DOWN_pressed) {
                if (playerAnim != (HOLD_SPEAR_CROUCH_R + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
                    changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_CROUCH_R);
                }
            }
            else {
                if (playerAnim != (HOLD_SPEAR_R + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_R);
                    changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_R);
                }

            }
        }
        else {      // ANIMACIONES cuando ya se ha atacado y player (EN AIRE) pero se mantiene X presionada)
            if (playerAnim != (HOLD_SPEAR_CROUCH_R + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
                changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_CROUCH_R);
            }
        }

    }
    else if (b_X_Attacking) {  // Si se suelta 'X, dejar de atacar
        cout << "--------------------  X  (RELEASE)" << endl;
        b_X_Attacking = false;
        if (!bJumping && !bFalling && !is_DOWN_pressed) {
            changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
        }
        else {

            changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
        }
    }

    if (Game::instance().getKey(GLFW_KEY_Z)) {
        if (!is_Z_pressed) {
            cout << "-------------------- Z (Jumping)" << endl;
            is_Z_pressed = true;
            if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y) || colisionPlatform()) {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
                changeAnimToRightLeft(*playerSprite, JUMP_RIGHT);

            }
        }
    }
    else if (is_Z_pressed) {
        cout << "-------------------- Z (RELEASE)" << endl;
        is_Z_pressed = !is_Z_pressed;
    }

    ////  GESTION MOVIMEINTO DERECHA/IZQUIERDA
    bool rightKey = Game::instance().getKey(GLFW_KEY_RIGHT);
    bool leftKey = Game::instance().getKey(GLFW_KEY_LEFT);
    int anim = playerSprite->animation();

    // Movimiento a la derecha
    if (rightKey && !is_UP_pressed) {
        if (anim == MOVE_LEFT || anim == CHARGE_LEFT) changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
        if (!leftKey) {
            is_Right_pressed = true;
            bLookingLeft = false;
            if (!bCrouching)
                posPlayer.x += playerSpeed;
            if (map->collisionMoveRight(posPlayer, glm::ivec2(28, 32)))
                posPlayer.x -= playerSpeed;
            righLeftKeyPressed();
        }
    }
    else if (is_Right_pressed) {
        is_Right_pressed = false;
        righLeftKeyReleased();
    }

    // Movimiento a la izquierda
    if (leftKey && !is_UP_pressed) {
        if (anim == MOVE_RIGHT || anim == CHARGE_RIGHT) changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
        if (!rightKey) {
            is_Left_pressed = true;
            bLookingLeft = true;
            if (!bCrouching)
                posPlayer.x -= playerSpeed;
            if (map->collisionMoveLeft(posPlayer, glm::ivec2(25, 32)))
                posPlayer.x += playerSpeed;
            righLeftKeyPressed();
        }
    }
    else if (is_Left_pressed) {
        is_Left_pressed = false;
        righLeftKeyReleased();
    }
    ////

    if (bJumping || bFalling) {         // EN AIRE SIN ESTAR PRESIONANDO X
        if (!b_X_Attacking) {
            if (Game::instance().getKey(GLFW_KEY_UP)) {
                if (playerSprite->animation() != (ATTACK_UP_R + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, ATTACK_UP_R);
                }
            }
            else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                if (playerSprite->animation() != (ATTACK_DOWN_R + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, ATTACK_DOWN_R);
                }
            }
            else {
                if (playerSprite->animation() != (CROUCH_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
                }
            }
        }
    }
    else if (!b_X_Attacking) {                  // EN TIERRA
        if (Game::instance().getKey(GLFW_KEY_UP)) {
            if (!is_UP_pressed) {
                is_UP_pressed = true;
                if (playerSprite->animation() != (COVER_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, COVER_RIGHT);
                }
            }
        }
        else if (is_UP_pressed) {
            is_UP_pressed = false;
            if (playerSprite->animation() != (STAND_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
        }

        if (Game::instance().getKey(GLFW_KEY_DOWN)) {
            if (!is_DOWN_pressed) {
                is_DOWN_pressed = bCrouching = true;
                if (playerSprite->animation() != (CROUCH_RIGHT + bLookingLeft)) {
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
                }
            }
        }
        else if (is_DOWN_pressed) {
            is_DOWN_pressed = bCrouching = false;
            if (playerSprite->animation() != (STAND_RIGHT + bLookingLeft)) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
        }
    }

    if (bJumping) {            // GESTION DEL SALTO
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180) {     // El salto ha terminado
            bJumping = false;
            posPlayer.y = startY;
            if (map->collisionMoveDown(posPlayer, glm::ivec2(25, 32), &posPlayer.y) || colisionPlatform()) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
        }
        else {                      // El jugador esta en el aire (subiendo o bajando)
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(PI * jumpAngle / 180.f));

            if (jumpAngle > 90) {   // Bajando
                if (map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_X, 32), &posPlayer.y) || colisionPlatform()) {
                    bJumping = false;
                    changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
                }
            }
        }
    }

    if (!bJumping) {
        if (!map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_X, 32), &posPlayer.y) && !colisionPlatform()) {
            bFalling = true;
            posPlayer.y += FALL_STEP;
            int anim = playerSprite->animation();
            if (!b_X_Attacking && anim != (ATTACK_DOWN_R + bLookingLeft) && anim != (ATTACK_UP_R + bLookingLeft)) {
                if (playerSprite->animation() != (CROUCH_RIGHT + bLookingLeft))
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
            }

        }
        else {
            if (bFalling && !b_X_Attacking) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
            bFalling = false;
        }
    }
    playerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    spearSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + spearDist), float(tileMapDispl.y + posPlayer.y)));


    if (Game::instance().getKey(GLFW_KEY_T)) {
        if (!is_T_pressed) {
            bUsingTotem = !bUsingTotem;
            if (playerHub) {
                playerHub->setTotemAnimation(bUsingTotem);
                cout << "PLAYER: Totem " << (bUsingTotem ? "Activated" : "Deactivated") << endl;
            }
            is_T_pressed = true;
        }
    }
    else {
        is_T_pressed = false; // Reiniciar el estado cuando se suelta la tecla
    }

    ///////////////////////////////////////////////////////////////////////

    if (Game::instance().getKey(GLFW_KEY_V)) {
        if (!is_V_pressed) {
            playerSpeed = (playerSpeed == 2) ? 4 : 2;
            is_V_pressed = true;
        }
    }
    else {
        is_V_pressed = false;
    }

    // Imprimir la posici�n del jugador cada 500ms
    static int timeSinceLastPrint = 0;
    timeSinceLastPrint += deltaTime;
    if (timeSinceLastPrint >= 2000) {
        std::cout << "Posicion: (" << posPlayer.x << ", " << posPlayer.y << ")" << std::endl;
        timeSinceLastPrint = 0;
    }

    // Gestionar la tecla O para aumentar la vida
    if (Game::instance().getKey(GLFW_KEY_O)) {
        if (!is_O_pressed) {
            is_O_pressed = true;
            if (playerHub) {
                heal(1);
            }
        }
    }
    else {
        is_O_pressed = false;
    }

    // Gestionar la tecla P para aplicar da�o
    if (Game::instance().getKey(GLFW_KEY_P)) {
        if (!is_P_pressed) {
            is_P_pressed = true;
            takeDamage(1);
        }
    }
    else {
        is_P_pressed = false;
    }

    // Gestionar la tecla I para a�adir un coraz�n (si HP >= 12)
    if (Game::instance().getKey(GLFW_KEY_I)) {
        if (!is_I_pressed) {
            is_I_pressed = true;
            addHeart();
        }
    }
    else {
        is_I_pressed = false;
    }

}

void Player::render() {
    int playerAnim = playerSprite->animation();

    if (playerAnim > 11 && playerAnim < 18) { //ANIMACIONES DE ATAQUE CON LANZA EXTENDIDA
        spearSprite->render();
    }
    playerSprite->render();
}

void Player::setTileMap(TileMap* tileMap) {
    map = tileMap;
}


void Player::setPosition(const glm::vec2& pos) {
    posPlayer = pos;
    playerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    spearSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() {
    return posPlayer;
}


glm::vec2 Player::getVelocity() {
    glm::vec2 velocity(0.f, 0.f);

    // Velocidad horizontal
    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
        velocity.x = -2.f; // Velocidad fija hacia la izquierda
    }
    else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
        velocity.x = 2.f; // Velocidad fija hacia la derecha
    }

    // Velocidad vertical
    if (bJumping) {
        // Aproximaci�n de la velocidad vertical durante el salto
        velocity.y = -JUMP_HEIGHT * cos(PI * jumpAngle / 180.f) * JUMP_ANGLE_STEP / 180.f * PI;
    }
    else {
        // Si est� cayendo
        velocity.y = FALL_STEP;
    }

    return velocity;
}

void Player::takeDamage(int damage) {
    if (playerHub) {
        playerHub->modifyPlayerHP(-damage, false);
        if (playerHub->isPlayerDead()) {
            cout << endl << "PLAYER: Dead -> ----GAME OVER-----" << endl << endl;
            //changeAnimToRightLeft(*playerSprite, DIE_RIGHT + bLookingLeft);
        }
        else {
            cout << "PLAYER: Damaged [-" << damage << "] (" << playerHub->getPlayerHP() << " HP left)" << endl;
            changeAnimToRightLeft(*playerSprite, DAMAGED_RIGHT + bLookingLeft);
        }
    }
}
void Player::heal(int hp) {
    if (playerHub) {
        playerHub->modifyPlayerHP(hp, false);
        cout << "PLAYER: Healed [+" << hp << "] (" << playerHub->getPlayerHP() << " HP left)" << endl;
    }
}
void Player::addHeart() {
    if (playerHub && playerHub->getPlayerHP() >= 12) {
        playerHub->modifyPlayerHP(3, true);
        cout << "PLAYER: ADD HEART [+" << 3 << "] (" << playerHub->getPlayerHP() << " HP left)" << endl;

    }
    else {
        cout << "PLAYER: ADD HEART [FAILED] (curretHP < 12) (" << playerHub->getPlayerHP() << " HP left)" << endl;
    }
}

bool Player::isPlayerDead() const {
    return playerHub ? playerHub->isPlayerDead() : false;
}

void Player::setPlayerHUB(PlayerHUB* hub) {
    playerHub = hub;
}


///
void Player::printAnimName(Sprite* sprite, int animation) {
    // Determine if the sprite is the player's or the spear's
    if (sprite == playerSprite) {
        // Print the animation name for the player sprite:
        switch (animation) {
        case STAND_RIGHT:           cout << "PLAYER: Stand Right" << endl; break;
        case STAND_LEFT:            cout << "PLAYER: Stand Left" << endl; break;
        case MOVE_RIGHT:            cout << "PLAYER: Move Right" << endl; break;
        case MOVE_LEFT:             cout << "PLAYER: Move Left" << endl; break;
        case JUMP_RIGHT:            cout << "PLAYER: Jump Right" << endl; break;
        case JUMP_LEFT:             cout << "PLAYER: Jump Left" << endl; break;
        case CROUCH_RIGHT:          cout << "PLAYER: Crouch Right" << endl; break;
        case CROUCH_LEFT:           cout << "PLAYER: Crouch Left" << endl; break;
        case DAMAGED_RIGHT:         cout << "PLAYER: Damaged Right" << endl; break;
        case DAMAGED_LEFT:          cout << "PLAYER: Damaged Left" << endl; break;
        case COVER_RIGHT:           cout << "PLAYER: Cover Right" << endl; break;
        case COVER_LEFT:            cout << "PLAYER: Cover Left" << endl; break;
        case HOLD_SPEAR_R:          cout << "PLAYER: Hold Spear Right" << endl; break;
        case HOLD_SPEAR_L:          cout << "PLAYER: Hold Spear Left" << endl; break;
        case CHARGE_RIGHT:          cout << "PLAYER: Charge Right" << endl; break;
        case CHARGE_LEFT:           cout << "PLAYER: Charge Left" << endl; break;
        case HOLD_SPEAR_CROUCH_R:   cout << "PLAYER: Hold Spear Crouch Right" << endl; break;
        case HOLD_SPEAR_CROUCH_L:   cout << "PLAYER: Hold Spear Crouch Left" << endl; break;
        case ATTACK_DOWN_R:         cout << "PLAYER: Attack Down Right" << endl; break;
        case ATTACK_DOWN_L:         cout << "PLAYER: Attack Down Left" << endl; break;
        case ATTACK_UP_R:           cout << "PLAYER: Attack Up Right" << endl; break;
        case ATTACK_UP_L:           cout << "PLAYER: Attack Up Left" << endl; break;
        case DIE_RIGHT:             cout << "PLAYER: Die Right" << endl; break;
        case DIE_LEFT:              cout << "PLAYER: Die Left" << endl; break;
        default:                    cout << "PLAYER: Unknown Player Animation (" << animation << ")" << endl; break;
        }
    }
    else if (sprite == spearSprite) {
        // Print the animation name for the spear sprite. These animations come from PlayerLargeAnims.
        switch (animation) {
        case SPEAR_ATTACK_RIGHT:    cout << "SPEAR: Attack Right" << endl; break;
        case SPEAR_ATTACK_LEFT:     cout << "SPEAR: Attack Left" << endl; break;
        case SPEAR_ATTACK_CROUCH_R: cout << "SPEAR: Attack Crouch Right" << endl; break;
        case SPEAR_ATTACK_CROUCH_L: cout << "SPEAR: Attack Crouch Left" << endl; break;
        case SPEAR_PASSIVE_R:       cout << "SPEAR: Passive Right" << endl; break;
        case SPEAR_PASSIVE_L:       cout << "SPEAR: Passive Left" << endl; break;
        case SPEAR_PASSIVE_CROUCH_R:   cout << "SPEAR: Low Passive  Right" << endl; break;
        case SPEAR_PASSIVE_CROUCH_L:   cout << "SPEAR: Low Passive  Left" << endl; break;
        case NONE:                  cout << "None" << endl; break;
        case NONE2:                 cout << "None2" << endl; break;
        default:                    cout << "Unknown Spear Animation (" << animation << ")" << endl; break;
        }
    }
    else {
        cout << "Unknown sprite" << endl;
    }
}


bool Player::isAttacking() const {
    // Está agachado (is_DOWN_pressed) y atacando (b_X_Attacking)
    return b_X_Attacking;
}

bool Player::Crouching() const {
    return is_DOWN_pressed;
}

bool Player::lookingleft() const {
    return bLookingLeft;
}