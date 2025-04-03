#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


//#define SCREEN_X 16
//#define SCREEN_Y 50

// MAPA DE 16x15 TILES
//#define INIT_PLAYER_X_TILES 6
//#define INIT_PLAYER_Y_TILES 10	

#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 10	


Scene::Scene()
{

	map = NULL;
	player = NULL;
}

Scene::~Scene() 
{
	if(map != NULL) delete map;
	if(player != NULL) delete player;
	for (auto& platform : platforms) {
		delete platform;
	}
}


void Scene::init() {
	initShaders();
	map = TileMap::createTileMap("levels/FINAL_MAP.tmx", glm::vec2(0, 0), texProgram);
	
	// Inicializar cámara
	part1 = part2 = part3 = part4 = false;
	horitzontal = true;
	cameraPos = glm::vec2(0.f, 0.f);
	cameraPos.x = fixedXVertical2;

	// Inicializar el jugador
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	//player->setPosition(glm::vec2(3224,816));
	player->setTileMap(map);

	snake = new Snake();
	snake->init(glm::ivec2(0, 0), texProgram);
	snake->setPosition(glm::vec2((INIT_PLAYER_X_TILES+2) * map->getTileSize(), (INIT_PLAYER_Y_TILES+1) * map->getTileSize()));
	snake->setTileMap(map);

	// Inicializar HUB
	playerHub = new PlayerHUB();
	playerHub->init(glm::ivec2(0, 0), texProgram);
	playerHub->setPosition(glm::vec2(0,0));
	playerHub->setTileMap(map);
	player->setPlayerHUB(playerHub);

	// Inicializar Plataformas
	initPlatforms();


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);	
	currentTime = 0.0f;

		// -> TP parte 3 (plataformas) (andriu fix the game)
	//cameraPos.x = fixedXVertical2;  // part 3
	//horitzontal = false;			  // part 3
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
void Scene::update(int deltaTime)
{

	//std::cout << player->getPosition().x << " " << player->getPosition().y << std::endl;
	currentTime += deltaTime;
	
	// Actualizar todas las plataformas
	for (auto& platform : platforms) {
		platform->update(deltaTime);

		// Verificar colisión entre el jugador y la plataforma actual
		bool onPlatform = checkPlatformCollision(player, platform);
		if (onPlatform) {
			float deltaY = platform->getDeltaY(); // Obtener el movimiento vertical de la plataforma
			glm::vec2 playerPos = player->getPosition();
			player->setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
		}
	}
	
	
	///
	player->update(deltaTime);
	snake->update(deltaTime);
	playerHub->update(deltaTime);
	handleSceneTransitions();

	updateCamera(player->getPosition(),deltaTime);
	playerHub->setPosition(glm::vec2(cameraPos.x, cameraPos.y));
	//std::cout << player->getPosition().x << " " << player->getPosition().y << " " << this->cameraPos.x << "" << this->cameraPos.y << std::endl;
	//std::cout << "Vel: " << player->getVelocity().x << " " << player->getVelocity().y << std::endl;
	//auto offset = Scene::getPlayerOffset(player);
	//Scene::setcameraPos(offset);
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
	glm::ivec2 playerSize(25, 32); // Tamaño del jugador (ajusta según tu código)
	glm::vec2 platformPos = platform->getPosition();
	glm::ivec2 platformSize(25,16); // Tamaño de la plataforma
	
	platformPos.y += 16;
	// Verificar si hay colisión
	if (playerPos.x + playerSize.x > platformPos.x &&
		playerPos.x < platformPos.x + platformSize.x &&
		playerPos.y + playerSize.y >= platformPos.y &&
		playerPos.y < platformPos.y + platformSize.y) {
		// Ajustar la posición del jugador solo si está cerca de la parte superior de la plataforma
		int deltaY = platformPos.y - (playerPos.y + playerSize.y);
		if (deltaY <= 4 && deltaY >= -4) { // Umbral ajustable
			return true; // Colisión detectada
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

	// Transiciones de cámara
	if (playerPos.x >= 2042 && playerPos.x <= 2288 && part1) {
		if (horitzontal) { // Transición de horizontal a vertical
			cameraPos.x = fixedXVertical; // Fijar X al entrar al pasadizo vertical
		}
		horitzontal = false;
	}
	else if (playerPos.x >= 2288 && playerPos.x <= 3008 && part2) {
		if (!horitzontal) { // Transición de vertical a horizontal
			cameraPos.y = fixedYHorizontal2; // Fijar Y al entrar al pasadizo inferior
		}
		horitzontal = true;
	}
	else if (playerPos.x >= 3040 && playerPos.x <= 3256) {
		if (horitzontal) { // Transición de vertical a horizontal
			cameraPos.x = fixedXVertical2;
		}
		horitzontal = false;
	}
	else if (playerPos.x >= 3296 && playerPos.x <= 4096) {
		if (!horitzontal) { // Transición de vertical a horizontal
			cameraPos.y = fixedYHorizontal3;
		}
		horitzontal = true;
	}
}

void Scene::updateCamera(glm::vec2 &posJugador,int deltaTime) {
		float jugadorX = posJugador.x; // Cambiar a float
		float jugadorY = posJugador.y; // Cambiar a float
		float cameraX = this->cameraPos.x; // Usar float
		float cameraY = this->cameraPos.y; // Usar float

		// Obtener la velocidad del jugador
		glm::vec2 jugadorVel = player->getVelocity();
		float jugadorVx = jugadorVel.x;
		float jugadorVy = jugadorVel.y;

		// Constante de suavizado
		const float FACTOR = 1.5f;

		// Convertir deltaTime a segundos
		float deltaTimeSec = deltaTime / 1000.f;

		// Calcular la velocidad de la cámara basada en la posición del jugador
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
			if(part2) cameraX = std::max(cameraX, 2304.0f);
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
			cameraY = std::min(cameraY,jugadorY - SCREEN_HEIGHT / 3);
			cameraY = std::max(0.0f, cameraY);
			if (part2) cameraY = std::max(480.0f, cameraY); //posar a part3
		}

		// Actualizar la posición de la cámara
		this->cameraPos = glm::vec2(cameraX, cameraY);
}


bool Scene::isOffScreen(Player* player) {
	glm::vec2 posP = player->getPosition();

	auto deltaOffset = glm::abs(player->getPosition() - this->cameraPos);
	int tileSize = this->map->getTileSize();
	return deltaOffset.x > tileSize  || deltaOffset.y > tileSize;
}

glm::vec2 Scene::getPlayerOffset(Player* player) {
	int tileSize = this->map->getTileSize();
	auto cameraTile = glm::vec2(tileSize * 16, tileSize * 15);
	return glm::floor(player->getPosition() / cameraTile) * cameraTile;
}



void Scene::setcameraPos(const glm::vec2& offset) {
	this->cameraPos = offset;
}

void Scene::render()
{
	glm::mat4 modelview;
	glm::mat4 cameraview;
	cameraview = glm::translate(cameraview, glm::vec3(-this->cameraPos,0));

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
	snake->render();

	if (part3) {
		for (auto& platform : platforms) platform->render();
	}
	//map->renderFRONT();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



