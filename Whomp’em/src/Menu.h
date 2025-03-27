#ifndef MENU_H
#define MENU_H

#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <iostream>
#include "Game.h"

class Menu {
private:
    GLuint textureId;
    GLuint vao, vbo;
    ShaderProgram* texProgram;

public:
    Menu(ShaderProgram* program);
    ~Menu();
    void init();
    void render(glm::mat4& projection);
};

#endif // MENU_H