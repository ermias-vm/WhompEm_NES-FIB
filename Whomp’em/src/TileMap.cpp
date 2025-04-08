#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tinyxml2.h>
#include "TileMap.h"

using namespace std;
using namespace tinyxml2;

TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
    TileMap* map = new TileMap(levelFile, minCoords, program);
    return map;
}

TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
    FRONT = nullptr;
    map = nullptr; // Inicializar map como nullptr
    hasImageLayer = true; // Por defecto, no hay capa de imagen
    imageLayerPos = glm::vec2(0.0f, 0.0f);
    imageLayerSize = glm::vec2(0.0f, 0.0f);
    if (!loadLevel(levelFile)) {
        std::cout << "Fallo al cargar el nivel: " << levelFile << endl;
        return; // Salir del constructor si falla
    }
    prepareArrays(minCoords, program);
}



TileMap::~TileMap()
{
    if (map != NULL)
        delete[] map; // Use delete[] for array deallocation
    if (FRONT != NULL)
        delete[] FRONT; // Use delete[] for array deallocation

}

int TileMap::getTileId(int x, int y) const {
    if (x >= 0 && x < mapSize.x && y >= 0 && y < mapSize.y) {
        return map[y * mapSize.x + x];
    }
    return -1; // Valor inválido si está fuera del mapa
}

void TileMap::render() const
{
    glEnable(GL_TEXTURE_2D);

    // Renderizar la capa de imagen primero (fondo)
    if (hasImageLayer) {
        imageLayerTexture.use();
        glBindVertexArray(imageVao);
        glEnableVertexAttribArray(imagePosLocation);
        glEnableVertexAttribArray(imageTexCoordLocation);
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 v�rtices para un quad
    }
    tilesheet.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
    glDisable(GL_TEXTURE_2D);

}

void TileMap::renderFRONT() const
{
    glEnable(GL_TEXTURE_2D);
    frontsheet.use();
    glBindVertexArray(foregroundVao);
    glEnableVertexAttribArray(foregroundPosLocation);
    glEnableVertexAttribArray(foregroundTexCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * frontTiles);
    glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
    glDeleteBuffers(1, &vbo);
    if (hasImageLayer) {
        glDeleteBuffers(1, &imageVbo);
        glDeleteVertexArrays(1, &imageVao);
    }
    glDeleteBuffers(1, &foregroundVbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
    
    XMLDocument doc;
    if (doc.LoadFile(levelFile.c_str()) != XML_SUCCESS) {
        std::cout << "Error loading TMX file: " << levelFile << endl;
        return false;
    }

    // Get the root <map> element
    XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cout << "No <map> element found in TMX file" << endl;
        return false;
    }

    // Parse map attributes
    mapElement->QueryIntAttribute("width", &mapSize.x);
    mapElement->QueryIntAttribute("height", &mapSize.y);
    mapElement->QueryIntAttribute("tilewidth", &tileSize);
    mapElement->QueryIntAttribute("tileheight", &blockSize);

    // Load external tileset (.tsx file)
    XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (!tilesetElement) {
        std::cout << "No <tileset> element found in TMX file" << endl;
        return false;
    }

    const char* tsxFile = tilesetElement->Attribute("source");
    if (!tsxFile) {
        std::cout << "No 'source' attribute in <tileset>" << endl;
        return false;
    }

    // Construct path to .tsx file (assumes same directory as .tmx)
    string tsxPath = levelFile.substr(0, levelFile.find_last_of("/\\") + 1) + tsxFile;
    XMLDocument tsxDoc;
    if (tsxDoc.LoadFile(tsxPath.c_str()) != XML_SUCCESS) {
        std::cout << "Error loading TSX file: " << tsxPath << endl;
        return false;
    }

    // Cargar capa de imagen (imagelayer)
    XMLElement* imageLayerElement = mapElement->FirstChildElement("imagelayer");
    // Dentro de loadLevel, en la secci�n de la capa de imagen:
    if (imageLayerElement) {
        XMLElement* imageElement = imageLayerElement->FirstChildElement("image");
        if (imageElement) {
            string imageFile = imageElement->Attribute("source");
            string imagePath = levelFile.substr(0, levelFile.find_last_of("/\\") + 1) + imageFile;
            if (!imageLayerTexture.loadFromFile(imagePath, TEXTURE_PIXEL_FORMAT_RGBA)) {
                std::cout << "Error al cargar imagen de capa: " << imagePath << endl;
                return false;
            }
            imageLayerTexture.setWrapS(GL_CLAMP_TO_EDGE);
            imageLayerTexture.setWrapT(GL_CLAMP_TO_EDGE);
            imageLayerTexture.setMinFilter(GL_NEAREST);
            imageLayerTexture.setMagFilter(GL_NEAREST);

            // Obtener posici�n y tama�o de la imagen
            imageLayerElement->QueryFloatAttribute("offsetx", &imageLayerPos.x);
            imageLayerElement->QueryFloatAttribute("offsety", &imageLayerPos.y);

            // Usar variables temporales int para el ancho y alto
            int width, height;
            imageElement->QueryIntAttribute("width", &width);
            imageElement->QueryIntAttribute("height", &height);
            imageLayerSize.x = static_cast<float>(width);  // Convertir int a float
            imageLayerSize.y = static_cast<float>(height); // Convertir int a float

            hasImageLayer = true;
            std::cout << "Capa de imagen cargada: " << imagePath << endl;
        }
    }
    XMLElement* tilesetRoot = tsxDoc.FirstChildElement("tileset");
    XMLElement* imageElement = tilesetRoot->FirstChildElement("image");
    if (!imageElement) {
        std::cout << "No <image> element found in TSX file" << endl;
        return false;
    }

    string tilesheetFile = imageElement->Attribute("source");
    // Construct full path to tilesheet (assumes same directory as .tmx)
    string tilesheetPath = levelFile.substr(0, levelFile.find_last_of("/\\") + 1) + tilesheetFile;
    std::cout << "Intentando cargar tilesheet desde: " << tilesheetPath << endl;
    if (!tilesheet.loadFromFile(tilesheetPath, TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cout << "Error al cargar tilesheet: " << tilesheetPath << endl;
        ifstream file(tilesheetPath);
        if (!file.good()) {
            std::cout << "El archivo no existe o no se puede abrir." << endl;
        }
        return false;
    }
    tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
    tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
    tilesheet.setMinFilter(GL_NEAREST);
    tilesheet.setMagFilter(GL_NEAREST);

    string frontsheetFile = imageElement->Attribute("source");
    // Construct full path to tilesheet (assumes same directory as .tmx)
    string frontsheetPath = levelFile.substr(0, levelFile.find_last_of("/\\") + 1) + frontsheetFile;
    std::cout << "Intentando cargar tilesheet desde: " << frontsheetPath << endl;
    if (!tilesheet.loadFromFile(frontsheetPath, TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cout << "Error al cargar tilesheet: " << frontsheetPath << endl;
        ifstream file(frontsheetPath);
        if (!file.good()) {
            std::cout << "El archivo no existe o no se puede abrir." << endl;
        }
        return false;
    }
    frontsheet.setWrapS(GL_CLAMP_TO_EDGE);
    frontsheet.setWrapT(GL_CLAMP_TO_EDGE);
    frontsheet.setMinFilter(GL_NEAREST);
    frontsheet.setMagFilter(GL_NEAREST);

    int tilesheetWidth, tilesheetHeight;
    imageElement->QueryIntAttribute("width", &tilesheetWidth);
    imageElement->QueryIntAttribute("height", &tilesheetHeight);
    tilesheetSize = glm::ivec2(tilesheetWidth / tileSize, tilesheetHeight / tileSize);
    tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

    int frontsheetWidth, frontsheetHeight;
    imageElement->QueryIntAttribute("width", &frontsheetWidth);
    imageElement->QueryIntAttribute("height", &frontsheetHeight);
    frontsheetSize = glm::ivec2(frontsheetWidth / tileSize, frontsheetHeight / tileSize);
    frontTexSize = glm::vec2(1.f / frontsheetSize.x, 1.f / frontsheetSize.y);



    XMLElement* layerElement = mapElement->FirstChildElement("layer");
    while (layerElement) {
        const char* layerName = layerElement->Attribute("name");
        cout << "Capa encontrada: " << layerName << endl; // Debug

        if (string(layerName) == "ID's") {
            XMLElement* dataElement = layerElement->FirstChildElement("data");
            if (!dataElement || strcmp(dataElement->Attribute("encoding"), "csv") != 0) {
                std::cout << "Only CSV-encoded data is supported" << endl;
                return false;
            }

            // Parse CSV tile data
            string csvData = dataElement->GetText();
            vector<int> tiles;
            stringstream ss(csvData);
            string tileValue;
            while (getline(ss, tileValue, ',')) {
                tiles.push_back(stoi(tileValue));
            }

            if (tiles.size() != mapSize.x * mapSize.y) {
                std::cout << "Tile data size (" << tiles.size() << ") does not match map dimensions (" << mapSize.x * mapSize.y << ")" << endl;
                return false;
            }

            // Allocate and fill the map array
            if (map != nullptr) delete[] map; // Use delete[] for array
            map = new int[mapSize.x * mapSize.y];
            for (int i = 0; i < tiles.size(); ++i) {
                map[i] = tiles[i]; // TMX uses 0 for empty tiles
            }
        }
        else if (string(layerName) == "Foreground") {
            XMLElement* dataElementFons = layerElement->FirstChildElement("data");
            if (!dataElementFons || strcmp(dataElementFons->Attribute("encoding"), "csv") != 0) {
                std::cout << "Only CSV-encoded data is supported" << endl;
                return false;
            }

            // Parse CSV tile data
            string csvDataFons = dataElementFons->GetText();
            vector<int> tilesFons;
            stringstream ss(csvDataFons);
            string tileValueFons;
            while (getline(ss, tileValueFons, ',')) {
                tilesFons.push_back(stoi(tileValueFons));
            }

            if (tilesFons.size() != mapSize.x * mapSize.y) {
                std::cout << "Tile data size (" << tilesFons.size() << ") does not match map dimensions (" << mapSize.x * mapSize.y << ")" << endl;
                return false;
            }

            // Allocate and fill the map array
            if (FRONT != nullptr) delete[] FRONT; // Use delete[] for array
            FRONT = new int[mapSize.x * mapSize.y];
            for (int i = 0; i < tilesFons.size(); ++i) {
                FRONT[i] = tilesFons[i]; // TMX uses 0 for empty tiles
            }
        }

        layerElement = layerElement->NextSiblingElement("layer");
    }
    

    // Load collision objects (COLISIONS layer)
    XMLElement* objectGroup = mapElement->FirstChildElement("objectgroup");
    if (objectGroup && string(objectGroup->Attribute("name")) == "COLISIONS") {
        XMLElement* object = objectGroup->FirstChildElement("object");
        while (object) {
            CollisionRect collisionObj;
            object->QueryIntAttribute("x", &collisionObj.x);
            object->QueryIntAttribute("y", &collisionObj.y);
            object->QueryIntAttribute("width", &collisionObj.width);
            object->QueryIntAttribute("height", &collisionObj.height);

            // Inicializar valores predeterminados para platform y damage
            collisionObj.platform = false; // Por defecto, no es una plataforma
            collisionObj.damage = false;   // Por defecto, no hace daño

            // Buscar las propiedades dentro de <properties>
            XMLElement* properties = object->FirstChildElement("properties");
            if (properties) {
                XMLElement* property = properties->FirstChildElement("property");
                while (property) {
                    const char* name = property->Attribute("name");
                    if (name) {
                        if (string(name) == "Platform") {
                            property->QueryBoolAttribute("value", &collisionObj.platform);
                        }
                        else if (string(name) == "Damage") {
                            property->QueryBoolAttribute("value", &collisionObj.damage);
                        }
                    }
                    property = property->NextSiblingElement("property");
                }
            }

            // Agregar el objeto de colisión al vector
            collisionObjects.push_back(collisionObj);
            object = object->NextSiblingElement("object");
        }
    }


    return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
    int tile;
    glm::vec2 posTile, texCoordTile[2], halfTexel;
    vector<float> vertices;

    nTiles = 0;
    halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
    for (int j = 0; j < mapSize.y; j++) {
        for (int i = 0; i < mapSize.x; i++) {
            tile = map[j * mapSize.x + i];
            if (tile != 0) {
                // Verificar que el tile est� dentro del rango del tileset
                if (tile - 1 >= tilesheetSize.x * tilesheetSize.y) {
                    std::cout << "Warning: Tile value " << tile << " at (" << i << ", " << j << ") exceeds tilesheet size. Clamping to 1." << endl;
                    tile = 1; // O manejar de otra forma (por ejemplo, ignorar el tile)
                }
                nTiles++;
                posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
                texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
                texCoordTile[1] = texCoordTile[0] + tileTexSize;
                texCoordTile[1] -= halfTexel;

                // First triangle
                vertices.push_back(posTile.x); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);

                // Second triangle
                vertices.push_back(posTile.x); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
                vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
            }
        }
    }
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    int tileFons;
    glm::vec2 posTileFons, texCoordTileFons[2], halfTexelFons;
    vector<float> verticesFons;

    frontTiles = 0;
    halfTexelFons = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
    for (int j = 0; j < mapSize.y; j++) {
        for (int i = 0; i < mapSize.x; i++) {
            tileFons = map[j * mapSize.x + i];
            if (tileFons != 0) {
                // Verificar que el tile est� dentro del rango del tileset
                if (tileFons - 1 >= tilesheetSize.x * tilesheetSize.y) {
                    std::cout << "Warning: Tile value " << tileFons << " at (" << i << ", " << j << ") exceeds tilesheet size. Clamping to 1." << endl;
                    tileFons = 1; // O manejar de otra forma (por ejemplo, ignorar el tile)
                }
                frontTiles++;
                posTileFons = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
                texCoordTileFons[0] = glm::vec2(float((tileFons - 1) % tilesheetSize.x) / tilesheetSize.x, float((tileFons - 1) / tilesheetSize.x) / tilesheetSize.y);
                texCoordTileFons[1] = texCoordTileFons[0] + tileTexSize;
                texCoordTileFons[1] -= halfTexelFons;

                // First triangle
                verticesFons.push_back(posTileFons.x); verticesFons.push_back(posTileFons.y);
                verticesFons.push_back(texCoordTileFons[0].x); verticesFons.push_back(texCoordTileFons[0].y);
                verticesFons.push_back(posTileFons.x + blockSize); verticesFons.push_back(posTileFons.y);
                verticesFons.push_back(texCoordTileFons[1].x); verticesFons.push_back(texCoordTileFons[0].y);
                verticesFons.push_back(posTileFons.x + blockSize); verticesFons.push_back(posTileFons.y + blockSize);
                verticesFons.push_back(texCoordTileFons[1].x); verticesFons.push_back(texCoordTileFons[1].y);

                // Second triangle
                verticesFons.push_back(posTileFons.x); verticesFons.push_back(posTileFons.y);
                verticesFons.push_back(texCoordTileFons[0].x); verticesFons.push_back(texCoordTileFons[0].y);
                verticesFons.push_back(posTileFons.x + blockSize); verticesFons.push_back(posTileFons.y + blockSize);
                vertices.push_back(texCoordTileFons[1].x); verticesFons.push_back(texCoordTileFons[1].y);
                verticesFons.push_back(posTileFons.x); verticesFons.push_back(posTileFons.y + blockSize);
                verticesFons.push_back(texCoordTileFons[0].x); verticesFons.push_back(texCoordTileFons[1].y);
            }
        }
    }

    glGenVertexArrays(1, &foregroundVao);
    glBindVertexArray(foregroundVao);
    glGenBuffers(1, &foregroundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, foregroundVbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * frontTiles * sizeof(float), &verticesFons[0], GL_STATIC_DRAW);
    foregroundPosLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    foregroundTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    if (hasImageLayer) {
        vector<float> imageVertices = {
            // Posiciones           // Coordenadas de textura
            minCoords.x + imageLayerPos.x, minCoords.y + imageLayerPos.y, 0.0f, 0.0f,
            minCoords.x + imageLayerPos.x + imageLayerSize.x, minCoords.y + imageLayerPos.y, 1.0f, 0.0f,
            minCoords.x + imageLayerPos.x + imageLayerSize.x, minCoords.y + imageLayerPos.y + imageLayerSize.y, 1.0f, 1.0f,
            minCoords.x + imageLayerPos.x, minCoords.y + imageLayerPos.y, 0.0f, 0.0f,
            minCoords.x + imageLayerPos.x + imageLayerSize.x, minCoords.y + imageLayerPos.y + imageLayerSize.y, 1.0f, 1.0f,
            minCoords.x + imageLayerPos.x, minCoords.y + imageLayerPos.y + imageLayerSize.y, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &imageVao);
        glBindVertexArray(imageVao);
        glGenBuffers(1, &imageVbo);
        glBindBuffer(GL_ARRAY_BUFFER, imageVbo);
        glBufferData(GL_ARRAY_BUFFER, imageVertices.size() * sizeof(float), &imageVertices[0], GL_STATIC_DRAW);
        imagePosLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
        imageTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
}


bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
    for (const auto& rect : collisionObjects) {
        if (pos.x + size.x >= rect.x && pos.x < rect.x + rect.width &&
            pos.y + size.y > rect.y && pos.y < rect.y + rect.height) {
            if (rect.platform) return false;
            else return true;
        }
    }
    return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
    for (const auto& rect : collisionObjects) {
        if (pos.x  <= rect.x + rect.width - 4 && pos.x + size.x > rect.x &&
            pos.y + size.y > rect.y && pos.y < rect.y + rect.height) {
            if (rect.platform) return false;
            else return true;
        }
    }
    return false;
}




bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
    for (const auto& rect : collisionObjects) {
        if (pos.x + size.x > rect.x && pos.x < rect.x + rect.width &&
            pos.y + size.y >= rect.y && pos.y < rect.y + rect.height) {
            if (*posY - rect.y + size.y <= 4) { // Adjust threshold as needed
                *posY = rect.y - size.y;
                return true;
            }
        }
    }
    return false;
}

bool TileMap::collisionDownDoesDamage(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
    for (const auto& rect : collisionObjects) {
        if (pos.x + size.x > rect.x && pos.x < rect.x + rect.width &&
            pos.y + size.y >= rect.y && pos.y < rect.y + rect.height) {
            if (*posY - rect.y + size.y <= 4) { 
                *posY = rect.y - size.y;
                return  rect.damage;
            }
        }
    }
    return false;
}

bool TileMap::collisionMoveHoritz(const glm::ivec2& pos, const glm::ivec2& size, bool movingLeft) const {
    glm::ivec2 posAjustada = pos;
    if (movingLeft)
        posAjustada.x += 0; // Ajustar solo para el movimiento a la izquierda

    for (const auto& rect : collisionObjects) {
        if (posAjustada.x <= rect.x + rect.width && posAjustada.x + size.x > rect.x &&
            posAjustada.y + size.y > rect.y && posAjustada.y < rect.y + rect.height) {
            return !rect.platform;
        }
    }
    return false;
}