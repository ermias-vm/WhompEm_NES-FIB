#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"


#define SCREEN_WIDTH 256		// 16 tiles * 16 píxeles/tile
#define SCREEN_HEIGHT 240	// 15 tiles * 16 píxeles/tile
#define SCALE_FACTOR 2.0f



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
	Scene* getScene() { return &scene; }
private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene scene;

};


#endif // _GAME_INCLUDE


