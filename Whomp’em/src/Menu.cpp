#include "Menu.h"
#include "Game.h"

Menu::Menu(ShaderProgram* program) : texProgram(program),
backgroundTextureId(0), playTextureId(0), instructionsTextureId(0), creditsTextureId(0),
backgroundVao(0), backgroundVbo(0), buttonsVao(0), buttonsVbo(0),
playXMin(0), playXMax(0), playYMin(0), playYMax(0) {
}

Menu::~Menu() {
    if (backgroundTextureId != 0) glDeleteTextures(1, &backgroundTextureId);
    if (playTextureId != 0) glDeleteTextures(1, &playTextureId);
    if (instructionsTextureId != 0) glDeleteTextures(1, &instructionsTextureId);
    if (creditsTextureId != 0) glDeleteTextures(1, &creditsTextureId);
    if (backgroundVbo != 0) glDeleteBuffers(1, &backgroundVbo);
    if (backgroundVao != 0) glDeleteVertexArrays(1, &backgroundVao);
    if (buttonsVbo != 0) glDeleteBuffers(1, &buttonsVbo);
    if (buttonsVao != 0) glDeleteVertexArrays(1, &buttonsVao);
}

void Menu::init() {
    int width, height;

    // Cargar la imagen de fondo
    unsigned char* image = SOIL_load_image("images/MenuFons.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading MenuFons.png" << std::endl;
        return;
    }
    glGenTextures(1, &backgroundTextureId);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SOIL_free_image_data(image);

    // Cargar la imagen del botón "Play"
    image = SOIL_load_image("images/play_button.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading play_button.png" << std::endl;
        return;
    }
    glGenTextures(1, &playTextureId);
    glBindTexture(GL_TEXTURE_2D, playTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float playWidth = width * 0.3f;
    float playHeight = height * 0.3f;
    SOIL_free_image_data(image);

    // Cargar la imagen del botón "Instructions"
    image = SOIL_load_image("images/instructions_button.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading instructions_button.png" << std::endl;
        return;
    }
    glGenTextures(1, &instructionsTextureId);
    glBindTexture(GL_TEXTURE_2D, instructionsTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float instructionsWidth = width * 0.3f;
    float instructionsHeight = height * 0.3f;
    SOIL_free_image_data(image);

    // Cargar la imagen del botón "Credits"
    image = SOIL_load_image("images/credits_button.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading credits_button.png" << std::endl;
        return;
    }
    glGenTextures(1, &creditsTextureId);
    glBindTexture(GL_TEXTURE_2D, creditsTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float creditsWidth = width * 0.3f;
    float creditsHeight = height * 0.3f;
    SOIL_free_image_data(image);

    // Configurar VAO/VBO para el fondo
    float backgroundVertices[] = {
        0.0f, 0.0f,         0.0f, 0.0f,
        SCREEN_WIDTH, 0.0f, 1.0f, 0.0f,
        SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f,
        0.0f, 0.0f,         0.0f, 0.0f,
        SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f,
        0.0f, SCREEN_HEIGHT, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &backgroundVao);
    glGenBuffers(1, &backgroundVbo);
    glBindVertexArray(backgroundVao);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);
    GLint posLocation = texProgram->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    GLint texCoordLocation = texProgram->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glBindVertexArray(0);

    // Configurar VAO/VBO para los botones
    float totalWidth = playWidth + instructionsWidth + creditsWidth + 40.0f; // 20px de espacio entre botones
    float startX = (SCREEN_WIDTH - totalWidth) / 2.0f; // Centrar horizontalmente
    float centerY = SCREEN_HEIGHT / 2.0f + 20.0f; // Centrar verticalmente

    // Guardar coordenadas del botón "Play" para detección de clics
    playXMin = startX;
    playXMax = startX + playWidth;
    playYMin = centerY - playHeight / 2.0f;
    playYMax = centerY + playHeight / 2.0f;
    std::cout << "playYMin = " << playYMin << " playYMax = " << playHeight << std::endl;
    std::cout << "minX = " << startX << "maxX = " << playXMax << "minY = " << playYMin << "maxY = " << playYMax << std::endl;
    float buttonVertices[] = {
        // Play
        startX, centerY - playHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth, centerY - playHeight / 2.0f,     1.0f, 0.0f,
        startX + playWidth, centerY + playHeight / 2.0f,     1.0f, 1.0f,
        startX, centerY - playHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth, centerY + playHeight / 2.0f,     1.0f, 1.0f,
        startX, centerY + playHeight / 2.0f,                 0.0f, 1.0f,
        // Instructions
        startX + playWidth + 20.0f, centerY - instructionsHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth + 20.0f + instructionsWidth, centerY - instructionsHeight / 2.0f, 1.0f, 0.0f,
        startX + playWidth + 20.0f + instructionsWidth, centerY + instructionsHeight / 2.0f, 1.0f, 1.0f,
        startX + playWidth + 20.0f, centerY - instructionsHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth + 20.0f + instructionsWidth, centerY + instructionsHeight / 2.0f, 1.0f, 1.0f,
        startX + playWidth + 20.0f, centerY + instructionsHeight / 2.0f,                 0.0f, 1.0f,
        // Credits
        startX + playWidth + instructionsWidth + 40.0f, centerY - creditsHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth + instructionsWidth + 40.0f + creditsWidth, centerY - creditsHeight / 2.0f, 1.0f, 0.0f,
        startX + playWidth + instructionsWidth + 40.0f + creditsWidth, centerY + creditsHeight / 2.0f, 1.0f, 1.0f,
        startX + playWidth + instructionsWidth + 40.0f, centerY - creditsHeight / 2.0f,                 0.0f, 0.0f,
        startX + playWidth + instructionsWidth + 40.0f + creditsWidth, centerY + creditsHeight / 2.0f, 1.0f, 1.0f,
        startX + playWidth + instructionsWidth + 40.0f, centerY + creditsHeight / 2.0f,                 0.0f, 1.0f
    };

    glGenVertexArrays(1, &buttonsVao);
    glGenBuffers(1, &buttonsVbo);
    glBindVertexArray(buttonsVao);
    glBindBuffer(GL_ARRAY_BUFFER, buttonsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_STATIC_DRAW);
    posLocation = texProgram->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = texProgram->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glBindVertexArray(0);
}

void Menu::render(glm::mat4& projection) {
    texProgram->use();
    texProgram->setUniformMatrix4f("projection", projection);
    texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram->setUniformMatrix4f("modelview", modelview);
    texProgram->setUniformMatrix4f("cameraview", glm::mat4(1.0f));
    texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

    glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
    glBindVertexArray(backgroundVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindVertexArray(buttonsVao);
    glBindTexture(GL_TEXTURE_2D, playTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, instructionsTextureId);
    glDrawArrays(GL_TRIANGLES, 6, 6);
    glBindTexture(GL_TEXTURE_2D, creditsTextureId);
    glDrawArrays(GL_TRIANGLES, 12, 6);
    glBindVertexArray(0);
}

bool Menu::isPlayClicked(float x, float y) {
    return (x >= playXMin && x <= playXMax && y >= playYMin && y <= playYMax);
}