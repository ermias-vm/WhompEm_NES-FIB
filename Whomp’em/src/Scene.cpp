#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


//#define SCREEN_X 16
//#define SCREEN_Y 50

#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 10		// MAPA DE 16x15 TILES



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
	HUB = new PlayerHUB();
	HUB->init(glm::ivec2(0, 0), texProgram);
	HUB->setPosition(glm::vec2(0,0));
	HUB->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{

	//std::cout << player->getPosition().x << " " << player->getPosition().y << std::endl;
	currentTime += deltaTime;
	player->update(deltaTime);
	snake->update(deltaTime);
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

	if (player->getPosition().x == 3008  && !part3) {
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
	

	
	if (player->getPosition().x >= 2042 && player->getPosition().x <= 2288  && part1) {
		if (horitzontal) { // Transición de horizontal a vertical
			cameraPos.x = fixedXVertical; // Fijar X al entrar al pasadizo vertical
		}
		horitzontal = false;
	}
	else if (player->getPosition().x >= 2288 && player->getPosition().x <= 3008  && part2) {
		if (!horitzontal) { // Transición de vertical a horizontal
		  cameraPos.y = fixedYHorizontal2; // Fijar Y al entrar al pasadizo inferior
		}
		horitzontal = true;
	}
	else if(player->getPosition().x >= 3040 && (player->getPosition().x <= 3256)){
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
	Scene::updateCamera(player->getPosition(),deltaTime);
	//std::cout << player->getPosition().x << " " << player->getPosition().y << " " << this->cameraPos.x << "" << this->cameraPos.y << std::endl;
	//std::cout << "Vel: " << player->getVelocity().x << " " << player->getVelocity().y << std::endl;
	//auto offset = Scene::getPlayerOffset(player);
	//Scene::setcameraPos(offset);
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
	snake->render();
	HUB->render();
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



