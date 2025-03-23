#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 4
#define PI 3.14159265
#define ANIMATION_SPEED 20
#define SPEAR_ANIMATION_SPEED 30


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
    SPEAR_PASSIVE_R, SPEAR_PASSIVE_L, SPEAR_PASSIVE_LOW_R, SPEAR_PASSIVE_LOW_L, NONE, NONE2
};



void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bLookingRight = bAttackCharged = true;
    bJumping = bCrouching = bAttacking = bDamaged = bFalling = false;
    is_Z_pressed = is_Right_pressed = is_Left_pressed = false;
    spearDist = 32;


    // ANIMACIONES PLAYER

    playerSpritesheet.loadFromFile("images/playerFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //(pixels frame, %espaci� ocupado(ancho,alto), playerSpritesheet, shaderProgram)
    playerSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.05), &playerSpritesheet, &shaderProgram);
    playerSprite->setNumberAnimations(25);

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

    // ANIMACIONES DE LA LANZA

    spearSpritesheet.loadFromFile("images/spearFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spearSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.20f, 0.10f), &spearSpritesheet, &shaderProgram);  // Tama�o ajustado para ataque
    spearSprite->setNumberAnimations(10);

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

    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_LOW_R, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_LOW_R, glm::vec2(0.0, 0.2f));

    spearSprite->setAnimationSpeed(SPEAR_PASSIVE_LOW_L, SPEAR_ANIMATION_SPEED);
    spearSprite->addKeyframe(SPEAR_PASSIVE_LOW_L, glm::vec2(0.0, 0.3f));

    spearSprite->setAnimationSpeed(NONE, SPEAR_ANIMATION_SPEED);  //TODO: MODIFICAR A NO RENDERIZAR
    spearSprite->addKeyframe(NONE, glm::vec2(0.8, 0.0f));

    spearSprite->setAnimationSpeed(NONE2, SPEAR_ANIMATION_SPEED);  //TODO: MODIFICAR A NO RENDERIZAR
    spearSprite->addKeyframe(NONE2, glm::vec2(0.8, 0.0f));

    playerSprite->changeAnimation(STAND_RIGHT); 	// Animaci�n inicial
    spearSprite->changeAnimation(NONE);

    tileMapDispl = tileMapPos;				// Desplazamiento del tile map (0,0)
    playerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    spearSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + spearDist), float(tileMapDispl.y + posPlayer.y)));
}


void Player::changeAnimToRightLeft(Sprite& sprite, int animation) {
    if (bLookingRight) {
        spearDist = 32;
    }
    else {
        animation++;
        spearDist = -32;
    }
    printAnimName(&sprite, animation);
    sprite.changeAnimation(animation);
}

void Player::righLeftKeyPressed(int isLeftAnim) {
    int playerAnim = playerSprite->animation();
    int spearAnim = spearSprite->animation();
    if (!bFalling && !bJumping) {
        if (bAttacking) {
            if (playerAnim != (CHARGE_RIGHT + isLeftAnim)) changeAnimToRightLeft(*playerSprite, CHARGE_RIGHT);
            if (bAttackCharged) {
                cout << "-----------------------------CHARGED-----------------" << endl;
                if (spearAnim != (SPEAR_ATTACK_RIGHT + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_RIGHT);
            }
            else {
                cout << "-----------------------------DESCARREGAT-----------------" << endl;
                if (spearAnim != (SPEAR_PASSIVE_R + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_R);
            }
        }
        else {
            if (playerAnim != (MOVE_RIGHT + isLeftAnim)) changeAnimToRightLeft(*playerSprite, MOVE_RIGHT);

        }
    }
    else {
        if (bAttacking) {

            if (playerAnim != (HOLD_SPEAR_CROUCH_R + isLeftAnim)) changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
            if (bAttackCharged) {
                if (spearAnim != (SPEAR_ATTACK_CROUCH_R + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_CROUCH_R);
            }
            else {
                if (spearAnim != (SPEAR_PASSIVE_LOW_R + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_LOW_R);
            }
        }
        else {
            if (playerAnim != (CROUCH_RIGHT + isLeftAnim)) changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);

        }

    }

}

void Player::righLeftKeyReleased(int isLeftAnim) {
    int playerAnim = playerSprite->animation();
    int spearAnim = spearSprite->animation();
    if (!bFalling && !bJumping) {
        if (bAttacking) {
            //if (!Game::instance().getKey(GLFW_KEY_X)) bAttackCharged = false;
            /*
            if (playerAnim != (HOLD_SPEAR_R + isLeftAnim)) changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_R);
            if (spearAnim != (SPEAR_PASSIVE_R + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_R);
            */
        }
        else {
            if (playerAnim != (STAND_RIGHT + isLeftAnim)) changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
        }
    }
    else {
        if (bAttacking) {
            if (playerAnim != (HOLD_SPEAR_CROUCH_R + isLeftAnim)) changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
            if (spearAnim != (SPEAR_PASSIVE_LOW_R + isLeftAnim)) changeAnimToRightLeft(*spearSprite, SPEAR_PASSIVE_LOW_R);
            //if (!Game::instance().getKey(GLFW_KEY_X)) bAttackCharged = false;

        }
        else {
            if (playerAnim != (CROUCH_RIGHT + isLeftAnim)) changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
        }
    }
}

void Player::update(int deltaTime)
{

    spearSprite->update(deltaTime);
    playerSprite->update(deltaTime);

    if (Game::instance().getKey(GLFW_KEY_X)) {
        if (!bAttacking) {             // Iniciar el ataque al presionar 'X'
            cout << "X" << endl;
            if (is_Right_pressed || is_Left_pressed) bAttackCharged = true;
            else bAttackCharged = false;

            bAttacking = true;
            if (!bJumping && !bFalling) {
                changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_R);
                changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_RIGHT);
            }
            else {
                changeAnimToRightLeft(*playerSprite, HOLD_SPEAR_CROUCH_R);
                changeAnimToRightLeft(*spearSprite, SPEAR_ATTACK_CROUCH_R);

            }

        }

    }
    else if (bAttacking) {  // Si se suelta 'X, dejar de atacar
        cout << "release X" << endl;
        bAttacking = false;
        bAttackCharged = true;
        if (!bJumping && !bFalling) {
            changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
        }
        else {
            changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
        }
    }

    if (Game::instance().getKey(GLFW_KEY_Z)) {
        if (!is_Z_pressed) {
            cout << "Z" << endl;
            is_Z_pressed = true;
            if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
                changeAnimToRightLeft(*playerSprite, JUMP_RIGHT);

            }
        }
    }
    else if (is_Z_pressed) {
        cout << "release Z" << endl;
        is_Z_pressed = !is_Z_pressed;
    }


    if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
        is_Right_pressed = true;
        if (!Game::instance().getKey(GLFW_KEY_LEFT)) {
            bLookingRight = true;
            posPlayer.x += 2;
            if (map->collisionMoveRight(posPlayer, glm::ivec2(22, 32))) posPlayer.x -= 2;
            righLeftKeyPressed(0);
        }
    }
    else if (is_Right_pressed) {
        is_Right_pressed = false;
        righLeftKeyReleased(0);

    }

    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
        is_Left_pressed = true;
        if (!Game::instance().getKey(GLFW_KEY_RIGHT)) {
            bLookingRight = false;
            posPlayer.x -= 2;
            if (map->collisionMoveLeft(posPlayer, glm::ivec2(22, 32))) posPlayer.x += 2;
            righLeftKeyPressed(1);
        }
    }
    else if (is_Left_pressed) {
        is_Left_pressed = false;
        righLeftKeyReleased(1);
    }



    if (Game::instance().getKey(GLFW_KEY_DOWN)) {

    }

    if (Game::instance().getKey(GLFW_KEY_UP)) {

    }

    if (bJumping) {            // GESTION SALTO
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180) {     // El salto ha terminado
            bJumping = false;
            posPlayer.y = startY;
            if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
        }
        else {                      // El jugador est� en el aire (subiendo o bajando)
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(PI * jumpAngle / 180.f));

            if (jumpAngle > 90) {   // Bajando
                if (map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
                    bJumping = false;
                    changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
                }
            }
        }
    }

    if (!bJumping) {
        if (!map->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y)) {
            bFalling = true;
            posPlayer.y += FALL_STEP;
            if (!bAttacking) {
                if (playerSprite->animation() != (CROUCH_RIGHT + !bLookingRight))
                    changeAnimToRightLeft(*playerSprite, CROUCH_RIGHT);
            }

        }
        else {
            if (bFalling && !bAttacking) {
                changeAnimToRightLeft(*playerSprite, STAND_RIGHT);
            }
            bFalling = false;
        }
    }


    ///////////////////////////////////////////////////////////////////////
    playerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    spearSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + spearDist), float(tileMapDispl.y + posPlayer.y)));

    // Imprimir la posici�n del jugador cada 500ms
    static int timeSinceLastPrint = 0;
    timeSinceLastPrint += deltaTime;
    if (timeSinceLastPrint >= 500) {
        //std::cout << "Posicion: (" << posPlayer.x << ", " << posPlayer.y << ")" << std::endl;
        timeSinceLastPrint = 0;
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


void Player::printAnimName(Sprite* sprite, int animation) {
    // Determine if the sprite is the player's or the spear's
    if (sprite == playerSprite) {
        // Print the animation name for the player sprite:
        switch (animation) {
        case STAND_RIGHT:      cout << "Stand Right" << endl; break;
        case STAND_LEFT:       cout << "Stand Left" << endl; break;
        case MOVE_RIGHT:       cout << "Move Right" << endl; break;
        case MOVE_LEFT:        cout << "Move Left" << endl; break;
        case JUMP_RIGHT:       cout << "Jump Right" << endl; break;
        case JUMP_LEFT:        cout << "Jump Left" << endl; break;
        case CROUCH_RIGHT:     cout << "Crouch Right" << endl; break;
        case CROUCH_LEFT:      cout << "Crouch Left" << endl; break;
        case DAMAGED_RIGHT:    cout << "Damaged Right" << endl; break;
        case DAMAGED_LEFT:     cout << "Damaged Left" << endl; break;
        case COVER_RIGHT:      cout << "Cover Right" << endl; break;
        case COVER_LEFT:       cout << "Cover Left" << endl; break;
        case HOLD_SPEAR_R:     cout << "Hold Spear Right" << endl; break;
        case HOLD_SPEAR_L:     cout << "Hold Spear Left" << endl; break;
        case CHARGE_RIGHT:     cout << "Charge Right" << endl; break;
        case CHARGE_LEFT:      cout << "Charge Left" << endl; break;
        case HOLD_SPEAR_CROUCH_R: cout << "Hold Spear Crouch Right" << endl; break;
        case HOLD_SPEAR_CROUCH_L: cout << "Hold Spear Crouch Left" << endl; break;
        case ATTACK_DOWN_R:    cout << "Attack Down Right" << endl; break;
        case ATTACK_DOWN_L:    cout << "Attack Down Left" << endl; break;
        case ATTACK_UP_R:      cout << "Attack Up Right" << endl; break;
        case ATTACK_UP_L:      cout << "Attack Up Left" << endl; break;
        case DIE_RIGHT:        cout << "Die Right" << endl; break;
        case DIE_LEFT:         cout << "Die Left" << endl; break;
        default:               cout << "Unknown Player Animation (" << animation << ")" << endl; break;
        }
    }
    else if (sprite == spearSprite) {
        // Print the animation name for the spear sprite. These animations come from PlayerLargeAnims.
        switch (animation) {
        case SPEAR_ATTACK_RIGHT:     cout << "Attack Right" << endl; break;
        case SPEAR_ATTACK_LEFT:      cout << "Attack Left" << endl; break;
        case SPEAR_ATTACK_CROUCH_R:  cout << "Attack Crouch Right" << endl; break;
        case SPEAR_ATTACK_CROUCH_L:  cout << "Attack Crouch Left" << endl; break;
        case SPEAR_PASSIVE_R:  cout << "Passive Spear Right" << endl; break;
        case SPEAR_PASSIVE_L:  cout << "Passive Spear Left" << endl; break;
        case SPEAR_PASSIVE_LOW_R: cout << "Low Passive Spear Right" << endl; break;
        case SPEAR_PASSIVE_LOW_L: cout << "Low Passive Spear Left" << endl; break;
        case NONE:             cout << "None" << endl; break;
        case NONE2:            cout << "None2" << endl; break;
        default:               cout << "Unknown Spear Animation (" << animation << ")" << endl; break;
        }
    }
    else {
        cout << "Unknown sprite" << endl;
    }
}


