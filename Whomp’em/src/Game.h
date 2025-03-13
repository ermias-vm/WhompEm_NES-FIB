#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"


#define ORIGINAL_WIDTH 640
#define ORIGINAL_HEIGHT 480

#define ZOOM_FACTOR 1.0f  // Cambia a 4.0f para x4
// Tama�o de la ventana ajustado al zoom
#define SCREEN_WIDTH (ORIGINAL_WIDTH * ZOOM_FACTOR)
#define SCREEN_HEIGHT (ORIGINAL_HEIGHT * ZOOM_FACTOR)

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene scene;

};


#endif // _GAME_INCLUDE


