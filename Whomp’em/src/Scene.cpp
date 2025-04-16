#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 10

Scene::Scene() : map(NULL), player(NULL), playerHub(NULL), boss(NULL) {
}

Scene::~Scene() {
    if (map != NULL) delete map;
    if (player != NULL) delete player;
    if (playerHub != NULL) delete playerHub;
    if (boss != NULL) delete boss;
    for (auto& platform : platforms) delete platform;
    for (auto& bamboo : bamboos) delete bamboo;
    for (auto& snake : snakes) delete snake;
    for (auto& bossBamboo : bossBamboos) delete bossBamboo;
    for (auto& spider : spiders) delete spider;
}

void Scene::init() {
    initShaders();
    map = TileMap::createTileMap("levels/FINAL_MAP.tmx", glm::vec2(0, 0), texProgram);

    part1 = part2 = part3 = part4 = part5 = part6 = false;
    start = true;
    horitzontal = true;
    cameraPos = glm::vec2(0.f, 0.f);

    player = new Player();
    player->init(glm::ivec2(0, 0), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));    //player->setPosition(glm::vec2(3760, 608));
    player->setTileMap(map);

	cyclope = new Cyclope();
    cyclope->init(glm::ivec2(0, 0), texProgram);
    cyclope->setPosition(glm::vec2(35 * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
    cyclope->setTileMap(map);

    playerHub = new PlayerHUB();
    playerHub->init(glm::ivec2(0, 0), texProgram);
    playerHub->setPosition(glm::vec2(0, 0));
    playerHub->setTileMap(map);
    player->setPlayerHUB(playerHub);

    boss = new Boss();
    boss->init(glm::ivec2(0, 0), texProgram, glm::vec2(3968, 592));
    boss->setTileMap(map);

    initPlatforms();
    initBamboos();
    initBossBamboos();
    initSpiders();
    initProyectiles();

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    currentTime = 0.0f;
    snakesSpawned = false;
    cameraPos.y = fixedYHorizontal;
    updateCamera(player->getPosition(), 0);

    // AQUI BOBO
    //cameraPos.y = fixedYHorizontal3;
    //horitzontal = true;
    //part1 = part2 = part3 = part4 = true;
}

void Scene::initBamboos() {
    int delay = 0;
    for (float x : bambooSpawnXPositions) {
        Bamboo* bamboo = new Bamboo();
        bamboo->init(glm::ivec2(0, 0), texProgram, glm::vec2(x, -delay * 120.f), false);
        bamboo->setTileMap(map);
        bamboos.push_back(bamboo);
        delay++;
    }
}


void Scene::initBossBamboos() {
    // Inicializamos el vector de bambúes del jefe vacío
    bossBamboos.clear();
}
void Scene::initProyectiles() {
    proyectiles.clear();
}

void Scene::initSpiders() {
    // Crear algunas arañas en posiciones específicas
    // Por ejemplo, una araña en X=3888 (cerca del jefe) y otra en X=4048
    Spider* spider1 = new Spider();
    spider1->init(glm::ivec2(0, 0), texProgram, 2128.f, 400.f);  // Posición inicial: X=3888, Y=560
    spider1->setTileMap(map);
    spiders.push_back(spider1);

    Spider* spider2 = new Spider();
    spider2->init(glm::ivec2(0, 0), texProgram, 2224.f, 432.f);  // Posición inicial: X=4048, Y=560
    spider2->setTileMap(map);
    spiders.push_back(spider2);
}


void Scene::updateSprites(int deltaTime) {
    if (!playerHub->isBossHPDead())boss->update(deltaTime);
    if (cyclope->isAlive()) cyclope->update(deltaTime);
   
    // Actualizar las arañas
    for (auto& spider : spiders) {
        spider->update(deltaTime);
    }
    player->update(deltaTime);

    // Update de bambús normales
    for (auto& bamboo : bamboos) {
        bamboo->update(deltaTime);
        if (bamboo->checkCollisionWithPlayer(player->getPosition(), glm::ivec2(25, 32))) {
            if (!player->isBlocking()) {
                player->takeDamage(1);
            }
        }
    }


    // Spawnear serpientes
    glm::vec2 playerPos = player->getPosition();

    if (!snakesSpawned && playerPos.x >= 2560) {
        Snake* snake1 = new Snake();
        snake1->init(glm::ivec2(0, 0), texProgram, 1, glm::vec2(2480.f, 1616.f));
        snake1->setTileMap(map);
        snakes.push_back(snake1);

        Snake* snake2 = new Snake();
        snake2->init(glm::ivec2(0, 0), texProgram, -1, glm::vec2(2656.f, 1616.f));
        snake2->setTileMap(map);
        snakes.push_back(snake2);

        snakesSpawned = true;
    }

    // Update de serpientes
    for (auto it = snakes.begin(); it != snakes.end();) {
        Snake* snake = *it;
        snake->update(deltaTime);

        if (readyToJump(snake) && !snake->isjumping() && !snake->hasJumped()) {
            snake->snakeJump();
        }

        if (player->isAttacking() && checkSpearCollisionWithSnake(snake)) {
            delete snake;
            it = snakes.erase(it);
            continue;
        }

        if (snake->shouldDisappear()) {
            delete snake;
            it = snakes.erase(it);
            continue;
        }

        ++it;
    }

    if (snakesSpawned && snakes.empty()) {
        snakesSpawned = false;
    }


    // Actualizar los proyectiles
    for (auto it = proyectiles.begin(); it != proyectiles.end();) {
        Proyectil* proyectil = *it;
        proyectil->update(deltaTime);

        // Verificar colisión con el jugador
        if (checkProyectilCollision(proyectil)) {
            player->takeDamage(1);
            std::cout << "PLAYER: Damaged by proyectil at " << proyectil->getPosition().x << std::endl;
            delete proyectil;
            it = proyectiles.erase(it);
            continue;
        }

        // Eliminar el proyectil si ha alcanzado su distancia máxima
        if (proyectil->hasReachedMaxDistance()) {
            delete proyectil;
            it = proyectiles.erase(it);
            std::cout << "Proyectil removed (reached max distance)" << std::endl;
            continue;
        }

        ++it;
    }

    // Update de bambús del jefe (incluye los lanzados en el aire y los de la línea)
    for (auto it = bossBamboos.begin(); it != bossBamboos.end();) {
        Bamboo* bamboo = *it;
        bamboo->update(deltaTime);

        if (bamboo->checkCollisionWithPlayer(player->getPosition(), glm::ivec2(25, 32)) && !player->isBlocking()) {
            player->takeDamage(1);
        }

        if (!bamboo->isActive()) {
            delete bamboo;
            it = bossBamboos.erase(it);
        }
        else {
            ++it;
        }
    }
    playerHub->update(deltaTime);
    handleSceneTransitions();
    updateCamera(player->getPosition(), deltaTime);
    playerHub->setPosition(glm::vec2(cameraPos.x, cameraPos.y));
}

void Scene::update(int deltaTime) {
    currentTime += deltaTime;
	checkAllCollisions(deltaTime);
    updateSprites(deltaTime);

    checkSpiderPlayerInteraction();

    // Lanzar línea de 10 bambús cuando el boss entra en GROUND_WAIT (una sola vez)
    if (boss->lounchBamboo() && !hasLaunchedLine) {
        for (float x : bambooSpawnLaunchPositions) {
            Bamboo* bamboo = new Bamboo();
            bamboo->init(glm::ivec2(0, 0), texProgram, glm::vec2(x, 480.f), true);
            bamboo->setTileMap(map);
            bossBamboos.push_back(bamboo);
        }
        hasLaunchedLine = true;
        boss->resetBambooThrow();
    }
    else if (!boss->lounchBamboo()) {
        hasLaunchedLine = false;
    }

    // Lanzar un solo bambú cuando el boss está en el aire (cada X tiempo)
    if (boss->shouldThrowBamboo()) {
        Bamboo* bamboo = new Bamboo();
        glm::vec2 bossPos = boss->getPosition();
        float bambooX = bossPos.x + 16.f;
        float bambooY = bossPos.y + 48.f;
        bamboo->init(glm::ivec2(0, 0), texProgram, glm::vec2(bambooX, bambooY), true);
        bamboo->setTileMap(map);
        bossBamboos.push_back(bamboo);
        std::cout << "Boss threw a bamboo at (" << bambooX << ", " << bambooY << ")" << std::endl;
        boss->resetBambooThrow();
    }

}

void Scene::checkAllCollisions(int deltaTime) {
	platformMgmt(deltaTime);
	cyclopeMgmt(deltaTime);
	//bossMgmt(deltaTime);
	//snakeMgmt(deltaTime);
	//spiderMgmt(deltaTime);
}


bool Scene::checkProyectilCollision(Proyectil* proyectil) {
    glm::vec2 playerPos = player->getPosition();
    glm::ivec2 playerSize(25, 32);  // Tamaño del jugador
    glm::vec2 proyectilPos = proyectil->getPosition();
    glm::ivec2 proyectilSize(16, 16);  // Tamaño del proyectil

    if (playerPos.x + playerSize.x > proyectilPos.x &&
        playerPos.x < proyectilPos.x + proyectilSize.x &&
        playerPos.y + playerSize.y > proyectilPos.y &&
        playerPos.y < proyectilPos.y + proyectilSize.y) {
        return true;
    }
    return false;
}

Scene::CollisionType Scene::checkCollisionWithEnemy(const glm::ivec2& enemyPos, const glm::ivec2& enemySize) {
    glm::vec2 playerPos = player->getPosition();
    glm::ivec2 playerSize(24, 32); // Tamaño del jugador (puede ajustarse según necesidad)

    // Calcular los puntos medios de ambos sprites (posición + 16, ya que el sprite es 32x32)
    glm::vec2 enemyCenter(enemyPos.x + 16, enemyPos.y + 16);
    glm::vec2 playerCenter(playerPos.x + 16, playerPos.y + 16);

    // Calcular las mitades de los tamaños efectivos
    float enemyHalfWidth = enemySize.x / 2.0f;
    float enemyHalfHeight = enemySize.y / 2.0f;
    float playerHalfWidth = playerSize.x / 2.0f;
    float playerHalfHeight = playerSize.y / 2.0f;

    // Verificar colisión usando las distancias entre los puntos medios
    float dx = std::abs(enemyCenter.x - playerCenter.x);
    float dy = std::abs(enemyCenter.y - playerCenter.y);

    // Si las distancias son menores que la suma de las mitades de los tamaños, hay colisión
    if (dx < (enemyHalfWidth + playerHalfWidth) && dy < (enemyHalfHeight + playerHalfHeight)) {
        // Determinar la dirección de la colisión según la diferencia relativa
        float xDiff = enemyCenter.x - playerCenter.x;
        float yDiff = enemyCenter.y - playerCenter.y;

        // Calcular cuánto se solapan en cada eje
        float xOverlap = (enemyHalfWidth + playerHalfWidth) - dx;
        float yOverlap = (enemyHalfHeight + playerHalfHeight) - dy;

        // La dirección de colisión es la del eje con menor solapamiento
        if (xOverlap < yOverlap) {
            // Colisión horizontal
            return (xDiff > 0) ? LEFT : RIGHT; // Si el enemigo está a la derecha del jugador, colisión por la izquierda
        }
        else {
            // Colisión vertical
            return (yDiff > 0) ? TOP : BOTTOM; // Si el enemigo está debajo del jugador, colisión por arriba
        }
    }

    return NONE;
   
}



void Scene::collisionsSpearEnemy() {
    if (!player->isAttacking()) return;

    glm::vec2 spearPos = player->getPosition();
    glm::ivec2 spearSize(25, 32);
    int damage = 1;
    if (player->usingFireTotem()) {
        damage = 2;
    }


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

bool Scene::playerCollisionPlatform() {
    for (auto& platform : platforms) {
        if (checkPlatformCollision(player, platform)) {
            return true;
        }
    }
    return false;
}


void Scene::platformMgmt(int deltaTime) {
    for (auto& platform : platforms) {
        platform->update(deltaTime);
        bool onPlatform = checkPlatformCollision(player, platform);
        if (onPlatform) {
            float deltaY = platform->getDeltaY();
            glm::vec2 playerPos = player->getPosition();
            player->setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
        }
    }
}
void Scene::cyclopeMgmt(int deltaTime) {
	if (cyclope->isAlive()) {
		if (checkCollisionWithEnemy(cyclope->getPosition(), glm::ivec2(20, 32)) != NONE) {
			player->takeDamage(1);
		}
	}
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
    if (playerPos.x >= 3808 && !part5) {
        player->setPosition(glm::vec2(3840, 608));
        part5 = true;
        playerHub->setShowBossHP(true);
        bossready = true;
    }
    else if (part5) {
        if (playerPos.x <= 3840) player->setPosition(glm::vec2(3840, playerPos.y));
    }
    if (playerPos.x >= 3904) part6 = true;
    if (part6 && start) {
        boss->startFight();
        start = false;
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
    if (part5) this->cameraPos = glm::vec2(3840, 480);
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
    if (!part1)cyclope->render();

    if (part5)boss->render();
    for (auto& snake : snakes) {
        snake->render();
    }
    for (auto& spider : spiders) { 
        spider->render();
    }
    for (auto& proyectil : proyectiles) {  // Renderizar los proyectiles
        proyectil->render();
    }

    if (part3) {
        for (auto& platform : platforms) platform->render();
    }
    for (auto& bamboo : bamboos) {
        bamboo->render();
    }
    for (auto& bossBamboo : bossBamboos) {  // Renderizar los bambúes del jefe
        bossBamboo->render();
    }
}



void Scene::checkSpiderPlayerInteraction() {
    glm::vec2 playerPos = player->getPosition();
    const float Y_TOLERANCE = 16.f;
    const float MAX_VERTICAL_DISTANCE = 64.f;  // 4 tiles = 64 píxeles

    for (auto& spider : spiders) {
        glm::vec2 spiderPos = spider->getPosition();
        Spider::SpiderState spiderState = spider->getState();

        bool canShoot = false;  // Indica si el jugador está en una posición que activa un disparo

        // Disparo horizontal: misma Y
        if (std::abs(spiderPos.y - playerPos.y) <= Y_TOLERANCE) {
            spider->setStop(true);
            canShoot = true;
            if (!spider->hasShot()) {
                // Crear un proyectil horizontal
                Proyectil* proyectil = new Proyectil();
                bool playerToLeft = spider->isPlayerToLeft(playerPos.x);
                Proyectil::ProyectilDirection direction = playerToLeft ? Proyectil::LEFT : Proyectil::RIGHT;
                glm::vec2 proyectilPos = spiderPos;
                proyectilPos.x += 8.f;
                proyectilPos.y += 8.f;
                proyectil->init(glm::ivec2(0, 0), texProgram, proyectilPos, direction);
                proyectil->setTileMap(map);
                proyectiles.push_back(proyectil);
                spider->setShot(true);
                std::cout << "Spider at X=" << spiderPos.x << " shot a proyectil " << (playerToLeft ? "left" : "right") << std::endl;
            }

            if (spiderState == Spider::MOVING_DOWN) {
                if (playerPos.x < spiderPos.x) {
                    spider->spiderSprite->changeAnimation(Spider::STOP_LEFT_DOWN);
                }
                else {
                    spider->spiderSprite->changeAnimation(Spider::STOP_RIGHT_DOWN);
                }
            }
            else if (spiderState == Spider::MOVING_UP) {
                if (playerPos.x < spiderPos.x) {
                    spider->spiderSprite->changeAnimation(Spider::STOP_LEFT_UP);
                }
                else {
                    spider->spiderSprite->changeAnimation(Spider::STOP_RIGHT_UP);
                }
            }
        }
        // Disparo vertical: misma X, dentro de 4 tiles de altura
        else if (spider->isPlayerInVerticalRange(playerPos.x, playerPos.y, MAX_VERTICAL_DISTANCE)) {
            spider->setStop(true);
            canShoot = true;
            if (!spider->hasShot()) {
                // Crear un proyectil vertical
                Proyectil* proyectil = new Proyectil();
                Proyectil::ProyectilDirection direction = (spiderState == Spider::MOVING_UP) ? Proyectil::UP : Proyectil::DOWN;
                glm::vec2 proyectilPos = spiderPos;
                proyectilPos.x += 4.f;
                proyectilPos.y += 4.f;
                proyectil->init(glm::ivec2(0, 0), texProgram, proyectilPos, direction);
                proyectil->setTileMap(map);
                proyectiles.push_back(proyectil);
                spider->setShot(true);
                std::cout << "Spider at X=" << spiderPos.x << " shot a proyectil " << (direction == Proyectil::UP ? "up" : "down") << std::endl;
            }
        }

        // Si el jugador no está en una posición que active un disparo, reiniciar el estado
        if (!canShoot) {
            spider->setStop(false);
            spider->setShot(false);
        }
    }
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

glm::vec2 Scene::getPlayerPos() const {
    return player->getPosition();
}