#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
//Texture wrapper class
class Texture
{
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFile(SDL_Renderer* tRenderer, std::string path, Uint8 r, Uint8 g, Uint8 b);

	bool loadFile(SDL_Renderer* tRenderer, std::string path);
	//Deallocates texture
	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Renders texture at given point
	void render(SDL_Renderer* tRenderer, int x, int y);
	void render(SDL_Renderer* tRenderer, int x, int y, int w, int h, SDL_Rect* clip);
	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	//Image dimensions
	int mWidth;
	int mHeight;
};


#endif
