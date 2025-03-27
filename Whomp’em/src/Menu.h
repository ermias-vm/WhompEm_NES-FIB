#ifndef MENU_H
#define MENU_H

#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <iostream>

class Menu {
private:
    GLuint backgroundTextureId;
    GLuint playTextureId;
    GLuint instructionsTextureId;
    GLuint creditsTextureId;
    GLuint backgroundVao, backgroundVbo;
    GLuint buttonsVao, buttonsVbo;
    ShaderProgram* texProgram;
    float playXMin, playXMax, playYMin, playYMax; // Coordenadas del quad de "Play"

public:
    Menu(ShaderProgram* program);
    ~Menu();
    void init();
    void render(glm::mat4& projection);
    bool isPlayClicked(float x, float y); // Método para verificar clic en "Play"
};

#endif // MENU_H