#pragma once
#include "TextureManager.h"
class WindowManager
{
public:
	WindowManager(const char* title, int xPos, int yPos, int width, int height, SDL_Color color);
	~WindowManager();

	void				   FrameEvents();

	
	inline int             GetWindowWidth() { return width; }
	inline int             GetWindowHeight() { return height; }
	inline bool			   GetRunState() { return isRunning;}
	inline SDL_Event	   GetEvent() { return event; }
	inline SDL_Window*     GetWindow() { return window; }
	inline SDL_Renderer*   GetRenderer() { return windowRenderer; }
	inline TextureManager* GetTextureManager() { return textureManager; }
	

private:
	int                    width;
	int                    height;
	bool				   isRunning;
	SDL_Event			   event;
	SDL_Window*            window;
	SDL_Renderer*          windowRenderer;
	TextureManager*        textureManager;
};