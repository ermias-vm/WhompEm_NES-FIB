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
    GLuint instructionsBackgroundTextureId; // New texture ID for instructions background
    GLuint backgroundVao, backgroundVbo;
    ShaderProgram* texProgram;
    bool showInstructions; // Flag to toggle instructions display
  
   
public:
    Menu(ShaderProgram* program);
    ~Menu();
    void init();
    void render(glm::mat4& projection);
    void loadInstructionsBackground(); // New function to load instructions background
    void setShowInstructions(bool value); // Setter for showInstructions
    bool isShowingInstructions() const;   // Getter for showInstructions

};

#endif // MENU_H