#include "Menu.h"
#include "Game.h"

Menu::Menu(ShaderProgram* program) : texProgram(program),
backgroundTextureId(0), instructionsBackgroundTextureId(0), creditsBackgroundTextureId(0),
backgroundVao(0), backgroundVbo(0), showInstructions(false), showCredits(false) {
}

Menu::~Menu() {
    if (backgroundTextureId != 0) glDeleteTextures(1, &backgroundTextureId);
    if (instructionsBackgroundTextureId != 0) glDeleteTextures(1, &instructionsBackgroundTextureId); // Cleanup new texture
    if (backgroundVbo != 0) glDeleteBuffers(1, &backgroundVbo);
    if (backgroundVao != 0) glDeleteVertexArrays(1, &backgroundVao);
}

void Menu::init() {
    int width, height;

    // Cargar la imagen de fondo
    unsigned char* image = SOIL_load_image("images/menu/MenuFons.png", &width, &height, 0, SOIL_LOAD_RGBA);
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
}

void Menu::loadInstructionsBackground() {
    int width, height;

    // Cargar la imagen de fondo de instrucciones
    unsigned char* image = SOIL_load_image("images/menu/FONS_INSTRUCCIONS.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading FONS_INSTRUCCIONS.png" << std::endl;
        return;
    }
    if (instructionsBackgroundTextureId == 0) {
        glGenTextures(1, &instructionsBackgroundTextureId);
    }
    glBindTexture(GL_TEXTURE_2D, instructionsBackgroundTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SOIL_free_image_data(image);

    showInstructions = true; // Set flag to show instructions
}

void Menu::loadCreditsBackground() {
    int width, height;

    // Cargar la imagen de fondo de instrucciones
    unsigned char* image = SOIL_load_image("images/menu/CREDITS.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cerr << "Error loading CREDITS.png" << std::endl;
        return;
    }
    if (creditsBackgroundTextureId == 0) {
        glGenTextures(1, &creditsBackgroundTextureId);
    }
    glBindTexture(GL_TEXTURE_2D, creditsBackgroundTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SOIL_free_image_data(image);

    showCredits = true; // Set flag to show instructions
}

void Menu::render(glm::mat4& projection) {
    texProgram->use();
    texProgram->setUniformMatrix4f("projection", projection);
    texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram->setUniformMatrix4f("modelview", modelview);
    texProgram->setUniformMatrix4f("cameraview", glm::mat4(1.0f));
    texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
    if (showInstructions && instructionsBackgroundTextureId != 0) {
        // Render instructions background instead of main menu
        glBindTexture(GL_TEXTURE_2D, instructionsBackgroundTextureId);
        glBindVertexArray(backgroundVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    else if (showCredits && creditsBackgroundTextureId != 0) {
        // Render instructions background instead of main menu
        glBindTexture(GL_TEXTURE_2D, creditsBackgroundTextureId);
        glBindVertexArray(backgroundVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
        glBindVertexArray(backgroundVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void Menu::setShowInstructions(bool value) {
    showInstructions = value;
}

void Menu::setShowCredits(bool value) {
    showCredits = value;
}

bool Menu::isShowingInstructions() const {
    return showInstructions;
}

bool Menu::isShowingCredits() const {
    return showCredits;
}
