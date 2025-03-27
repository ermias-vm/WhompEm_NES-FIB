#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Menu.h"
#include <glm/gtc/matrix_transform.hpp>

#define TARGET_FRAMERATE 60.0f

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        Game::instance().keyPressed(key);
    else if (action == GLFW_RELEASE)
        Game::instance().keyReleased(key);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Game::instance().mouseMove(int(xpos), int(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS)
        Game::instance().mousePress(button);
    else if (action == GLFW_RELEASE)
        Game::instance().mouseRelease(button);
}

int main(void) {
    GLFWwindow* window;
    double timePerFrame = 1.f / TARGET_FRAMERATE, timePreviousFrame, currentTime;
    bool jugar = false;
    bool menuInitialized = false;
    bool gameInitialized = false;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR, "Whomp'em", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Set window initial position */
    glfwSetWindowPos(window, 10, 40);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Init glew to have access to GL extensions */
    glewExperimental = GL_TRUE;
    glewInit();

    // Configuración inicial para el menú
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    ShaderProgram texProgram;
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
    glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

    Menu menu(&texProgram);

    // Variable para detectar el clic
    bool mousePressed = false;
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            void* userData = glfwGetWindowUserPointer(window);
            if (userData) {
                bool* pressed = static_cast<bool*>(userData);
                *pressed = true;
            }
        }
        });
    glfwSetWindowUserPointer(window, &mousePressed);

    timePreviousFrame = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        if (currentTime - timePreviousFrame >= timePerFrame) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (!jugar) {
                if (!menuInitialized) {
                    menu.init();
                    menuInitialized = true;
                }
                menu.render(projection);

                if (mousePressed) {
                    jugar = true;
                    mousePressed = false; // Resetear para evitar múltiples transiciones
                }
            }
            else {
                if (!gameInitialized) {
                    Game::instance().init();
                    gameInitialized = true;
                }
                if (!Game::instance().update(int(1000.0f * (currentTime - timePreviousFrame))))
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                Game::instance().render();
            }

            timePreviousFrame = currentTime;
            glfwSwapBuffers(window);
        }

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}