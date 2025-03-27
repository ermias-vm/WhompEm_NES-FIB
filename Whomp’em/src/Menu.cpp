#include "Menu.h"

Menu::Menu(ShaderProgram* program) : texProgram(program), textureId(0), vao(0), vbo(0) {}

Menu::~Menu() {
    if (textureId != 0) glDeleteTextures(1, &textureId);
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (vao != 0) glDeleteVertexArrays(1, &vao);
}

void Menu::init() {
    // Cargar la imagen con SOIL
    int width, height;
    unsigned char* image = SOIL_load_image("images/MenuFons.png", &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        std::cout << "Error loading MenuFons.png" << std::endl;
        return;
    }

    // Crear y configurar la textura
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SOIL_free_image_data(image);

    // Definir los vértices del quad (posición y coordenadas de textura)
    float vertices[] = {
        // Posiciones        // Coordenadas de textura
        0.0f, 0.0f,         0.0f, 0.0f,  // Inferior izquierdo
        SCREEN_WIDTH, 0.0f, 1.0f, 0.0f,  // Inferior derecho
        SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f,  // Superior derecho
        0.0f, 0.0f,         0.0f, 0.0f,  // Inferior izquierdo
        SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f,  // Superior derecho
        0.0f, SCREEN_HEIGHT, 0.0f, 1.0f   // Superior izquierdo
    };

    // Crear VAO y VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar atributos de vértices
    GLint posLocation = texProgram->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    GLint texCoordLocation = texProgram->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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

    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}