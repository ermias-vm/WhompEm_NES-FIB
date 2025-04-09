#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Menu.h"
#include <glm/gtc/matrix_transform.hpp>

#define TARGET_FRAMERATE 60.0f

struct WindowData {
    bool jugar;
    Menu* menu; // Pointer to Menu instance
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        Game::instance().keyPressed(key);
        WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (data) {
            if (key == GLFW_KEY_P) {
                data->jugar = true; // Start game
            }
            else if (key == GLFW_KEY_I && !data->jugar) {
                data->menu->loadInstructionsBackground(); // Load instructions background
            }
            else if (key == GLFW_KEY_C && !data->jugar) {
                data->menu->loadCreditsBackground(); // Load instructions background
            }
            else if (key == GLFW_KEY_ESCAPE && !data->jugar) {
                Menu* menu = data->menu;
                if (menu && (menu->isShowingInstructions() || menu->isShowingCredits())) {
                    menu->setShowInstructions(false); // Return to main menu
                    menu->setShowCredits(false);
                }
            }
        }
    }
    else if (action == GLFW_RELEASE) {
        Game::instance().keyReleased(key);
    }
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
    bool gameInitialized = false;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(SCREEN_WIDTH * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR, "Whomp'em", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 10, 40);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    ShaderProgram texProgram;
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked()) {
        std::cerr << "Shader linking failed: " << texProgram.log() << std::endl;
        glfwTerminate();
        return -1;
    }
    texProgram.bindFragmentOutput("outColor");

    glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

    Menu menu(&texProgram);
    menu.init(); // Initialize the menu here to load textures and set up VAOs/VBOs
    WindowData windowData = { false, &menu };
    glfwSetWindowUserPointer(window, &windowData);

    timePreviousFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        if (currentTime - timePreviousFrame >= timePerFrame) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (!windowData.jugar) {
                menu.render(projection); // Render menu or instructions based on state
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