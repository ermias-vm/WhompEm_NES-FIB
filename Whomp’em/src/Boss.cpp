#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Boss.h"
#include "Game.h"

Boss::Boss() : bossSprite(nullptr), map(nullptr), state(WAITING), timeAccumulator(0.f), isJumping(false), jumpTime(0.f), airTime(0.f), airFrameToggle(0), airFrameTimer(0.f), triangleTime(0.f), triangleCount(0), bambooThrowTimer(0.f), fallTime(0.f), groundWaitTime(0.f), isdown(false) {
}

Boss::~Boss() {
    if (bossSprite) delete bossSprite;
}

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::vec2 pos) {
    tileMapDispl = tileMapPos;
    posBoss = pos;
    initialPos = pos;
    bossSpritesheet.loadFromFile("images/sprites/bossFrames.png", TEXTURE_PIXEL_FORMAT_RGBA);
    bossSprite = Sprite::createSprite(glm::ivec2(32, 48), glm::vec2(0.2, 1.0), &bossSpritesheet, &shaderProgram);
    bossSprite->setNumberAnimations(5);

    // Frame 1: Reposo inicial
    bossSprite->setAnimationSpeed(FRAME_1, 1);
    bossSprite->addKeyframe(FRAME_1, glm::vec2(0.0, 0.0));

    // Frame 2: En el aire (1)
    bossSprite->setAnimationSpeed(FRAME_2, 1);
    bossSprite->addKeyframe(FRAME_2, glm::vec2(0.2, 0.0));

    // Frame 3: En el aire (2)
    bossSprite->setAnimationSpeed(FRAME_3, 1);
    bossSprite->addKeyframe(FRAME_3, glm::vec2(0.4, 0.0));

    // Frame 4: En el suelo después de caer
    bossSprite->setAnimationSpeed(FRAME_4, 1);
    bossSprite->addKeyframe(FRAME_4, glm::vec2(0.6, 0.0));

    // Frame 5: Cayendo con semicírculos
    bossSprite->setAnimationSpeed(FRAME_5, 1);
    bossSprite->addKeyframe(FRAME_5, glm::vec2(0.8, 0.0));

    // Iniciar con Frame 1
    bossSprite->changeAnimation(FRAME_1);
    bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::startFight() {
    state = (GROUND_INITIAL);
}

bool Boss::lounchBamboo() {
    return isdown;
}

void Boss::update(int deltaTime) {
    timeAccumulator += deltaTime;
    bossSprite->update(deltaTime);

    switch (state) {
    case WAITING:
        break;

    case GROUND_INITIAL:
        // Cambiar a estado de salto después de un breve momento (inmediato para simplificar)
        state = JUMPING_UP;
        isJumping = true;
        jumpTime = 0.f;
        jumpStartPos = posBoss;
        bossSprite->changeAnimation(FRAME_2);
        break;

    case JUMPING_UP: {
        jumpTime += deltaTime;
        float t = jumpTime / jumpDuration;
        if (t >= 1.0f) {
            posBoss.y = jumpStartPos.y - jumpHeight;  // Altura máxima
            isJumping = false;
            state = IN_AIR;
            airTime = 0.f;
            airFrameToggle = 0;
            airFrameTimer = 0.f;
            triangleTime = 0.f;
            triangleCount = 0;
            bambooThrowTimer = 0.f;  // Reiniciar el temporizador de bambúe
            triangleStartPos = posBoss;  // Centro de la base del triángulo
            bossSprite->changeAnimation(FRAME_2);
        }
        else {
            posBoss.y = jumpStartPos.y - (jumpHeight * t);  // Subida lineal
        }
        bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
        break;
    }

    case IN_AIR: {
        airTime += deltaTime;
        airFrameTimer += deltaTime;
        triangleTime += deltaTime;
        bambooThrowTimer += deltaTime;  // Asegurarse de que el temporizador se actualice siempre

        // Alternar entre Frame 2 y Frame 3 cada 0.5s
        if (airFrameTimer >= airFrameInterval) {
            airFrameToggle = (airFrameToggle + 1) % 2;
            bossSprite->changeAnimation(airFrameToggle == 0 ? FRAME_2 : FRAME_3);
            airFrameTimer = 0.f;
        }

        // Movimiento en triángulo simétrico
        float t = fmod(triangleTime, triangleDuration) / triangleDuration;  // Normalizar t entre 0 y 1 para cada triángulo
        if (triangleTime >= triangleDuration) {
            triangleTime -= triangleDuration;  // Reiniciar el tiempo del triángulo sin teletransportar
            triangleCount++;
        }

        if (triangleCount < 2) {  // Hacer 2 triángulos
            // Definir los vértices del triángulo
            glm::vec2 vertexLeft = glm::vec2(triangleStartPos.x - triangleWidth / 2.0f, triangleStartPos.y);  // Vértice izquierdo
            glm::vec2 vertexTop = glm::vec2(triangleStartPos.x, triangleStartPos.y - triangleHeight);        // Vértice superior
            glm::vec2 vertexRight = glm::vec2(triangleStartPos.x + triangleWidth / 2.0f, triangleStartPos.y); // Vértice derecho

            if (t < 0.25f) {  // Primer segmento: Centro -> Vértice izquierdo
                float segmentT = t / 0.25f;
                posBoss.x = triangleStartPos.x + (vertexLeft.x - triangleStartPos.x) * segmentT;
                posBoss.y = triangleStartPos.y + (vertexLeft.y - triangleStartPos.y) * segmentT;
            }
            else if (t < 0.5f) {  // Segundo segmento: Vértice izquierdo -> Vértice superior
                float segmentT = (t - 0.25f) / 0.25f;
                posBoss.x = vertexLeft.x + (vertexTop.x - vertexLeft.x) * segmentT;
                posBoss.y = vertexLeft.y + (vertexTop.y - vertexLeft.y) * segmentT;
            }
            else if (t < 0.75f) {  // Tercer segmento: Vértice superior -> Vértice derecho
                float segmentT = (t - 0.5f) / 0.25f;
                posBoss.x = vertexTop.x + (vertexRight.x - vertexTop.x) * segmentT;
                posBoss.y = vertexTop.y + (vertexRight.y - vertexTop.y) * segmentT;
            }
            else {  // Cuarto segmento: Vértice derecho -> Centro
                float segmentT = (t - 0.75f) / 0.25f;
                posBoss.x = vertexRight.x + (triangleStartPos.x - vertexRight.x) * segmentT;
                posBoss.y = vertexRight.y + (triangleStartPos.y - vertexRight.y) * segmentT;
            }
        }

        // Después de 2 triángulos (3 segundos), pasar a la caída
        if (airTime >= airDuration) {
            state = FALLING;
            fallTime = 0.f;
            bossSprite->changeAnimation(FRAME_5);
        }
        bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
        break;
    }

    case FALLING: {
        fallTime += deltaTime;
        float ft = fallTime / fallDuration;
        if (ft >= 1.0f) {
            posBoss.y = initialPos.y;  // Volver al suelo
            posBoss.x = initialPos.x;  // Asegurar posición inicial en X
            state = GROUND_WAIT;
            isdown = true;
            groundWaitTime = 0.f;
            bossSprite->changeAnimation(FRAME_4);
        }
        else {
            // Movimiento oscilante (semicírculos) mientras cae
            posBoss.y = jumpStartPos.y - jumpHeight + (jumpHeight * ft);  // Caída lineal
            posBoss.x = initialPos.x + fallOscillationAmplitude * sin(ft * fallOscillationFrequency * 2 * 3.14159f);  // Oscilación en X
        }
        bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
        break;
    }

    case GROUND_WAIT: {
        groundWaitTime += deltaTime;
        if (groundWaitTime >= groundWaitDuration) {
            state = JUMPING_UP;
            isJumping = true;
            jumpTime = 0.f;
            jumpStartPos = posBoss;
            bossSprite->changeAnimation(FRAME_2);
            isdown = false;
        }
        bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
        break;
    }
    }
}

bool Boss::shouldThrowBamboo() const {
    if (state == IN_AIR && bambooThrowTimer >= bambooThrowInterval) {
        return true;
    }
    return false;
}

void Boss::resetBambooThrow() {
    bambooThrowTimer = 0.f;
}

void Boss::render() {
    bossSprite->render();
}

void Boss::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Boss::setPosition(const glm::vec2& pos) {
    posBoss = pos;
    initialPos = pos;
    bossSprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

glm::vec2 Boss::getPosition() {
    return posBoss;
}