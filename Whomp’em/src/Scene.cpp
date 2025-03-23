#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


//#define SCREEN_X 16
//#define SCREEN_Y 50

#define INIT_PLAYER_X_TILES 4
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
	map = TileMap::createTileMap("levels/MAPA_FINAL.tmx", glm::vec2(0, 0), texProgram);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	std::cout << map->getTileSize() << std::endl;
	player->setTileMap(map);
	horitzontal = true;
	cameraPos = glm::vec2(0.f, 0.f);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	// Determinar la sección actual
	if (player->getPosition().x >= 2082 && player->getPosition().x <= 2280) {
		if (horitzontal) { // Transición de horizontal a vertical
			cameraPos.x = fixedXVertical; // Fijar X al entrar al pasadizo vertical
		}
		horitzontal = false;
	}
	else if (player->getPosition().x >= 2280) {
		if (!horitzontal) { // Transición de vertical a horizontal
		  cameraPos.y = fixedYHorizontal2; // Fijar Y al entrar al pasadizo inferior
		}
		horitzontal = true;
	}
	else {
		if (!horitzontal) { // Transición de vertical a horizontal
			cameraPos.y = fixedYHorizontal;
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
			cameraX = std::max(0.0f, cameraX);

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



