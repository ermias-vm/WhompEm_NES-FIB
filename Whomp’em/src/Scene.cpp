#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 10

Scene::Scene() : map(NULL), player(NULL), playerHub(NULL) {
}

Scene::~Scene() {
    if (map != NULL) delete map;
    if (player != NULL) delete player;
    if (playerHub != NULL) delete playerHub;
    for (auto& platform : platforms) delete platform;
    for (auto& bamboo : bamboos) delete bamboo;
    for (auto& snake : snakes) delete snake;
}

void Scene::init() {
    initShaders();
    map = TileMap::createTileMap("levels/FINAL_MAP.tmx", glm::vec2(0, 0), texProgram);

    part1 = part2 = part3 = part4 = false;
    horitzontal = true;
    cameraPos = glm::vec2(0.f, 0.f);

    player = new Player();
    player->init(glm::ivec2(0, 0), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES*16, INIT_PLAYER_Y_TILES*16));
    player->setTileMap(map);

    playerHub = new PlayerHUB();
    playerHub->init(glm::ivec2(0, 0), texProgram);
    playerHub->setPosition(glm::vec2(0, 0));
    playerHub->setTileMap(map);
    player->setPlayerHUB(playerHub);

    initPlatforms();
    initBamboos();

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    currentTime = 0.0f;
    snakesSpawned = false;
    cameraPos.y = fixedYHorizontal;
    horitzontal = true;

    updateCamera(player->getPosition(), 0);
}

void Scene::initBamboos() {
    int delay = 0;
    for (float x : bambooSpawnXPositions) {
        Bamboo* bamboo = new Bamboo();
        bamboo->init(glm::ivec2(0, 0), texProgram, glm::vec2(x, -delay * 120.f));
        bamboo->setTileMap(map);
        bamboos.push_back(bamboo);
        delay++;
    }
}

void Scene::initPlatforms() {
    Platform* platform1 = new Platform();
    platform1->init(glm::ivec2(0, 0), texProgram, 3172.0f, 780.0f, 20.0f, 1.0f, true);
    platform1->setTileMap(map);
    platforms.push_back(platform1);

    Platform* platform2 = new Platform();
    platform2->init(glm::ivec2(0, 0), texProgram, 3140.0f, 745.0f, 25.0f, 1.0f, false);
    platform2->setTileMap(map);
    platforms.push_back(platform2);

    Platform* platform3 = new Platform();
    platform3->init(glm::ivec2(0, 0), texProgram, 3228.0f, 705.0f, 25.0f, 1.0f, true);
    platform3->setTileMap(map);
    platforms.push_back(platform3);

    Platform* platform4 = new Platform();
    platform4->init(glm::ivec2(0, 0), texProgram, 3165.0f, 655.0f, 20.0f, 1.0f, true);
    platform4->setTileMap(map);
    platforms.push_back(platform4);

    Platform* platform5 = new Platform();
    platform5->init(glm::ivec2(0, 0), texProgram, 3140.0f, 615.0f, 25.0f, 1.0f, false);
    platform5->setTileMap(map);
    platforms.push_back(platform5);

    Platform* platform6 = new Platform();
    platform6->init(glm::ivec2(0, 0), texProgram, 3200.0f, 580.0f, 15.0f, 1.0f, true);
    platform6->setTileMap(map);
    platforms.push_back(platform6);
}

void Scene::update(int deltaTime) {
    currentTime += deltaTime;

    for (auto& platform : platforms) {
        platform->update(deltaTime);
        bool onPlatform = checkPlatformCollision(player, platform);
        if (onPlatform) {
            float deltaY = platform->getDeltaY();
            glm::vec2 playerPos = player->getPosition();
            player->setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
        }
    }

    player->update(deltaTime);

    glm::vec2 playerPos = player->getPosition();
    if (!snakesSpawned && playerPos.x >= 2560) {
        Snake* snake1 = new Snake();
        snake1->init(glm::ivec2(0, 0), texProgram, 1, glm::vec2(2480.f, 1616.f));
        snake1->setTileMap(map);
        snakes.push_back(snake1);
        std::cout << "Spawned snake at 2480, 1616 moving right" << std::endl;

        Snake* snake2 = new Snake();
        snake2->init(glm::ivec2(0, 0), texProgram, -1, glm::vec2(2656.f, 1616.f));
        snake2->setTileMap(map);
        snakes.push_back(snake2);
        std::cout << "Spawned snake at 2656, 1616 moving left" << std::endl;

        snakesSpawned = true;
    }

    for (auto it = snakes.begin(); it != snakes.end();) {
        Snake* snake = *it;
        snake->update(deltaTime);

        if (readyToJump(snake) && !snake->isjumping() && !snake->hasJumped()) { // Añadimos !snake->hasJumped()
            snake->snakeJump();
            std::cout << "Snake at " << snake->getPosition().x << " jumped!" << std::endl;
        }

        if (CheckEnemyCollission(snake) && damagecooldown == 0) {
            damagecooldown = 100;
			player->takeDamage(1);
            std::cout << "PLAYER: Damaged by snake at " << snake->getPosition().x << std::endl;
        }

        if (player->isAttacking() && checkSpearCollisionWithSnake(snake)) {
            delete snake;
            it = snakes.erase(it);
            std::cout << "SNAKE: Killed by spear attack!" << std::endl;
            continue;
        }

        if (snake->shouldDisappear()) {
            delete snake;
            it = snakes.erase(it);
            std::cout << "Snake removed (disappeared)" << std::endl;
            continue;
        }

        ++it;
    }

    if (snakesSpawned && snakes.empty()) {
        snakesSpawned = false;
    }

    if (damagecooldown > 0) --damagecooldown;

    for (auto& bamboo : bamboos) {
        bamboo->update(deltaTime);
        if (bamboo->checkCollisionWithPlayer(player->getPosition(), glm::ivec2(25, 32))) {
            if (damagecooldown == 0) {
				player->takeDamage(1);
                damagecooldown = 100;
                std::cout << "PLAYER: Damaged by bamboo at " << bamboo->getPosition().x << std::endl;
            }
            bamboo->reset();
        }
        else if (!bamboo->isActive()) {
            bamboo->reset();
        }
    }

    playerHub->update(deltaTime);
    handleSceneTransitions();
    updateCamera(player->getPosition(), deltaTime);
    playerHub->setPosition(glm::vec2(cameraPos.x, cameraPos.y));
}

bool Scene::checkSpearCollisionWithSnake(Snake* snake) {
    if (!snake || !player) return false;

    glm::vec2 spearPos = player->getPosition();
    if (player->Crouching()) spearPos.y += 16.0f;
    glm::ivec2 spearSize(32, 16);

    glm::vec2 snakePos = snake->getPosition();
    glm::ivec2 snakeSize(16, 10);

    if (!player->lookingleft()) {
        spearPos.x += 32.0f;
        if (spearPos.x + spearSize.x > snakePos.x &&
            spearPos.x < snakePos.x + snakeSize.x &&
            spearPos.y + spearSize.y > snakePos.y &&
            spearPos.y < snakePos.y + snakeSize.y) {
            return true;
        }
    }
    else {
        spearPos.x -= 32.0f;
        if (spearPos.x + spearSize.x > snakePos.x &&
            spearPos.x < snakePos.x + snakeSize.x &&
            spearPos.y + spearSize.y > snakePos.y &&
            spearPos.y < snakePos.y + snakeSize.y) {
            return true;
        }
    }
    return false;
}

bool Scene::readyToJump(Snake* snake) {
    glm::vec2 playerPos = player->getPosition();
    glm::vec2 snakePos = snake->getPosition();
    float distance = glm::length(playerPos - snakePos);
    int snakeDirection = snake->getMovementDirection();
    const float jumpThreshold = 16.0f * 3;
    bool isApproaching = (snakeDirection == 1 && snakePos.x < playerPos.x) ||
        (snakeDirection == -1 && snakePos.x > playerPos.x);
    return distance < jumpThreshold && isApproaching;
}

bool Scene::playerColisionPlatform() {
    for (auto& platform : platforms) {
        if (checkPlatformCollision(player, platform)) {
            return true;
        }
    }
    return false;
}

bool Scene::checkPlatformCollision(Player* player, Platform* platform) {
    glm::vec2 playerPos = player->getPosition();
    glm::ivec2 playerSize(25, 32);
    glm::vec2 platformPos = platform->getPosition();
    glm::ivec2 platformSize(25, 16);

    platformPos.y += 16;
    if (playerPos.x + playerSize.x > platformPos.x &&
        playerPos.x < platformPos.x + platformSize.x &&
        playerPos.y + playerSize.y >= platformPos.y &&
        playerPos.y < platformPos.y + platformSize.y) {
        int deltaY = platformPos.y - (playerPos.y + playerSize.y);
        if (deltaY <= 4 && deltaY >= -4) {
            return true;
        }
    }
    return false;
}

void Scene::handleSceneTransitions() {
    glm::vec2 playerPos = player->getPosition();

    if (playerPos.x >= 2036 && playerPos.x <= 2040 && !part1) {
        player->setPosition(glm::vec2(2054, 64));
        part1 = true;
    }
    else if (part1) {
        if (playerPos.x <= 2042) player->setPosition(glm::vec2(2042, playerPos.y));
    }

    if (playerPos.x >= 2278 && playerPos.y < 1600 && !part2) {
        player->setPosition(glm::vec2(2278, playerPos.y));
    }

    if (playerPos.x >= 2288 && playerPos.x <= 2292 && !part2) {
        player->setPosition(glm::vec2(2330, 1600));
        part2 = true;
    }
    else if (part2) {
        if (playerPos.x <= 2304) player->setPosition(glm::vec2(2304, playerPos.y));
    }

    if (playerPos.x >= 3008 && playerPos.x <= 3012 && !part3) {
        player->setPosition(glm::vec2(3056, 1600));
        part3 = true;
    }
    else if (part3) {
        if (playerPos.x <= 3056) {
            player->setPosition(glm::vec2(3056, playerPos.y));
        }
    }

    if (playerPos.x >= 3296 && playerPos.x <= 3300 && !part4) {
        player->setPosition(glm::vec2(3328, 576));
        part4 = true;
    }
    else if (part4) {
        if (playerPos.x <= 3328) {
            player->setPosition(glm::vec2(3328, playerPos.y));
        }
    }

    if (playerPos.x >= 2042 && playerPos.x <= 2288 && part1) {
        if (horitzontal) {
            cameraPos.x = fixedXVertical;
        }
        horitzontal = false;
    }
    else if (playerPos.x >= 2288 && playerPos.x <= 3008 && part2) {
        if (!horitzontal) {
            cameraPos.y = fixedYHorizontal2;
        }
        horitzontal = true;
    }
    else if (playerPos.x >= 3040 && playerPos.x <= 3256) {
        if (horitzontal) {
            cameraPos.x = fixedXVertical2;
        }
        horitzontal = false;
    }
    else if (playerPos.x >= 3296 && playerPos.x <= 4096) {
        if (!horitzontal) {
            cameraPos.y = fixedYHorizontal3;
        }
        horitzontal = true;
    }
}

void Scene::updateCamera(glm::vec2& posJugador, int deltaTime) {
    float jugadorX = posJugador.x;
    float jugadorY = posJugador.y;
    float cameraX = this->cameraPos.x;
    float cameraY = this->cameraPos.y;

    glm::vec2 jugadorVel = player->getVelocity();
    float jugadorVx = jugadorVel.x;
    float jugadorVy = jugadorVel.y;

    const float FACTOR = 1.5f;
    float deltaTimeSec = deltaTime / 1000.f;

    if (horitzontal) {
        if ((jugadorX - cameraX) < (SCREEN_WIDTH / 3)) {
            cameraVx = FACTOR * jugadorVx;
        }
        else if ((jugadorX - cameraX) > (2 * SCREEN_WIDTH / 3)) {
            cameraVx = FACTOR * jugadorVx;
        }
        else {
            cameraVx = 0;
        }
        cameraX += deltaTimeSec * cameraVx;
        cameraX = std::max(cameraX, jugadorX - 2 * SCREEN_WIDTH / 3);
        cameraX = std::min(cameraX, jugadorX - SCREEN_WIDTH / 3);
        if (part2) cameraX = std::max(cameraX, 2304.0f);
        cameraX = std::max(0.0f, cameraX);
        if (part4) cameraX = std::max(cameraX, 3328.0f);
        cameraX = std::min(cameraX, 3840.0f);
    }
    else {
        if ((jugadorY - cameraY) < (SCREEN_HEIGHT / 3)) {
            cameraVy = FACTOR * jugadorVy;
        }
        else if ((jugadorY - cameraY) > (2 * SCREEN_HEIGHT / 3)) {
            cameraVy = FACTOR * jugadorVy;
        }
        else {
            cameraVy = 0;
        }
        cameraY += deltaTimeSec * cameraVy;
        cameraY = std::max(cameraY, jugadorY - 2 * SCREEN_HEIGHT / 3);
        cameraY = std::min(cameraY, jugadorY - SCREEN_HEIGHT / 3);
        cameraY = std::max(0.0f, cameraY);
        if (part2) cameraY = std::max(480.0f, cameraY);
    }

    this->cameraPos = glm::vec2(cameraX, cameraY);
}

bool Scene::isOffScreen(glm::vec2 pos) {
    float leftEdge = cameraPos.x;
    float rightEdge = cameraPos.x + SCREEN_WIDTH;
    float topEdge = cameraPos.y;
    float bottomEdge = cameraPos.y + SCREEN_HEIGHT;
    return (pos.x + 16 < leftEdge || pos.x > rightEdge || pos.y + 16 < topEdge || pos.y > bottomEdge);
}

glm::vec2 Scene::getPlayerOffset(Player* player) {
    int tileSize = this->map->getTileSize();
    auto cameraTile = glm::vec2(tileSize * 16, tileSize * 15);
    return glm::floor(player->getPosition() / cameraTile) * cameraTile;
}

void Scene::setcameraPos(const glm::vec2& offset) {
    this->cameraPos = offset;
}

void Scene::render() {
    glm::mat4 modelview;
    glm::mat4 cameraview;
    cameraview = glm::translate(cameraview, glm::vec3(-this->cameraPos, 0));

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniformMatrix4f("cameraview", cameraview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    map->render();
    player->render();
    playerHub->render();
    for (auto& snake : snakes) {
        snake->render();
    }

    if (part3) {
        for (auto& platform : platforms) platform->render();
    }
    for (auto& bamboo : bamboos) {
        bamboo->render();
    }
}

bool Scene::CheckEnemyCollission(Snake* snake) {
    glm::vec2 playerPos = player->getPosition();
    glm::ivec2 playerSize(25, 32);
    glm::vec2 snakeformPos = snake->getPosition();
    glm::ivec2 snakeformSize(16, 16);

    snakeformPos.y += 16;
    if (playerPos.x + playerSize.x > snakeformPos.x &&
        playerPos.x < snakeformPos.x + snakeformSize.x &&
        playerPos.y + playerSize.y >= snakeformPos.y &&
        playerPos.y < snakeformPos.y + snakeformSize.y) {
        return true;
    }
    return false;
}

void Scene::initShaders() {
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled()) {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled()) {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked()) {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}