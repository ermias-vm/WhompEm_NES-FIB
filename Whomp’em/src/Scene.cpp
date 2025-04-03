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

#define INIT_PLAYER_X_TILES 38
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
}


void Scene::init() {
	initShaders();
	map = TileMap::createTileMap("levels/FINAL_MAP.tmx", glm::vec2(0, 0), texProgram);
	
	// Inicializar el jugador
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	snake = new Snake();
	snake->init(glm::ivec2(0, 0), texProgram);
	snake->setPosition(glm::vec2((INIT_PLAYER_X_TILES+2) * map->getTileSize(), (INIT_PLAYER_Y_TILES+1) * map->getTileSize()));
	snake->setTileMap(map);



	// Inicializar cámara
	horitzontal = true;
	cameraPos = glm::vec2(0.f, 0.f);
	cameraPos.x = fixedXVertical2;

	// Inicializar HUB
	playerHub = new PlayerHUB();
	playerHub->init(glm::ivec2(0, 0), texProgram);
	playerHub->setPosition(glm::vec2(0,0));
	playerHub->setTileMap(map);
	player->setPlayerHUB(playerHub);

	// Inicializar Plataformas
	platform = new Platform();
	platform->init(glm::ivec2(0, 0), texProgram, 140.0f, 30.0f, 1.0f);
	platform->setPosition(glm::vec2(40 * map->getTileSize(), 140.0f));
	platform->setTileMap(map);




	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{

	//std::cout << player->getPosition().x << " " << player->getPosition().y << std::endl;
	currentTime += deltaTime;
	
	// Actualizar la plataforma primero
	platform->update(deltaTime);

	// Verificar colisión entre el jugador y la plataforma
	bool onPlatform = checkPlatformCollision(player, platform);
	if (onPlatform) {
		//cout << "----------------PLATFORM COLISION" << endl;
		float deltaY = platform->getDeltaY(); // Obtener el movimiento vertical de la plataforma
		//cout << "DeltaY: " << deltaY << endl;
		glm::vec2 playerPos = player->getPosition();
		glm::vec2 platformPos = platform->getPosition();
		player->setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
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
	return checkPlatformCollision(player, platform);
}

bool Scene::checkPlatformCollision(Player* player, Platform* platform) {
	glm::vec2 playerPos = player->getPosition();
	glm::ivec2 playerSize(25, 32); // Tamaño del jugador (ajusta según tu código)
	glm::vec2 platformPos = platform->getPosition();
	glm::ivec2 platformSize(32,16); // Tamaño de la plataforma
	
	platformPos.y += 16;
	// Verificar si hay colisión
	if (playerPos.x + playerSize.x > platformPos.x &&
		playerPos.x < platformPos.x + platformSize.x &&
		playerPos.y + playerSize.y >= platformPos.y &&
		playerPos.y < platformPos.y + platformSize.y) {
		// Ajustar la posición del jugador solo si está cerca de la parte superior de la plataforma
		int deltaY = platformPos.y - (playerPos.y + playerSize.y);
		if (deltaY <= 4 && deltaY >= -4) { // Umbral ajustable
			//player->setPosition(glm::vec2(playerPos.x, platformPos.y - playerSize.y));
			return true; // Colisión detectada
		}
	}
	return false; // Sin colisión
}

void Scene::handleSceneTransitions() {
	// Determinar la sección actual
	if (player->getPosition().x == 2036 && !part1) {
		player->setPosition(glm::vec2(2054, 64));
		part1 = true;
	}
	else if (part1) {
		if (player->getPosition().x <= 2042) player->setPosition(glm::vec2(2042, player->getPosition().y));
	}

	if (player->getPosition().x >= 2278 && player->getPosition().y < 1600 && !part2) player->setPosition(glm::vec2(2278, player->getPosition().y));

	if (player->getPosition().x == 2288 && !part2) {
		player->setPosition(glm::vec2(2330, 1600));
		part2 = true;
	}
	else if (part2) {
		if (player->getPosition().x <= 2304) player->setPosition(glm::vec2(2304, player->getPosition().y));
	}

	if (player->getPosition().x == 3008 && !part3) {
		player->setPosition(glm::vec2(3056, 1600));
		part3 = true;
	}
	else if (part3) {
		if (player->getPosition().x <= 3056) {
			player->setPosition(glm::vec2(3056, player->getPosition().y));
		}
	}
	if (player->getPosition().x == 3296 && !part4) {
		player->setPosition(glm::vec2(3328, 576));
		part4 = true;
	}
	else if (part4) {
		if (player->getPosition().x <= 3328) {
			player->setPosition(glm::vec2(3328, player->getPosition().y));
		}
	}



	if (player->getPosition().x >= 2042 && player->getPosition().x <= 2288 && part1) {
		if (horitzontal) { // Transición de horizontal a vertical
			cameraPos.x = fixedXVertical; // Fijar X al entrar al pasadizo vertical
		}
		horitzontal = false;
	}
	else if (player->getPosition().x >= 2288 && player->getPosition().x <= 3008 && part2) {
		if (!horitzontal) { // Transición de vertical a horizontal
			cameraPos.y = fixedYHorizontal2; // Fijar Y al entrar al pasadizo inferior
		}
		horitzontal = true;
	}
	else if (player->getPosition().x >= 3040 && (player->getPosition().x <= 3256)) {
		if (horitzontal) { // Transición de vertical a horizontal
			cameraPos.x = fixedXVertical2;
		}
		horitzontal = false;
	}
	else if (player->getPosition().x >= 3296 && (player->getPosition().x <= 4096)) {
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
	platform->render();
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



