#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include <vector> // Add this to include std::vector
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	struct CollisionRect {
		int x, y, width, height;
	};
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	std::vector<CollisionRect> collisionObjects; // Declare collisionObjects here
	Texture imageLayerTexture; // Textura para la capa de imagen
	bool hasImageLayer;        // Indicador de si hay una capa de imagen
	glm::vec2 imageLayerPos;   // Posici�n de la capa de imagen
	glm::vec2 imageLayerSize;  // Tama�o de la capa de imagen
	GLuint imageVao, imageVbo; // VAO y VBO para renderizar la imagen
	GLint imagePosLocation, imageTexCoordLocation; // Localizaciones de atributos para la imagen
};


#endif // _TILE_MAP_INCLUDE


