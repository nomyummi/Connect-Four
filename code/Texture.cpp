
//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "Texture.h"
#include "Board.h"

Texture::Texture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFile(SDL_Renderer* tRenderer, std::string path) //Doesn't set colorkey
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(tRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool Texture::loadFile(SDL_Renderer* tRenderer, std::string path, Uint8 r, Uint8 g, Uint8 b )
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image //Sets that color as invisible
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, r, g, b) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( tRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render(SDL_Renderer* tRenderer, int x, int y )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( tRenderer, mTexture, NULL, &renderQuad );
}

void Texture::render(SDL_Renderer* tRenderer, int x, int y, int w, int h, SDL_Rect* clip)
{
	//x,y location of the rendered texture. w,h size of the rendered texture. 
	//clip is the part of the sprite sheet rendered

	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL)
	{
		renderQuad.w = w;
		renderQuad.h = h;
	}
	SDL_RenderCopy(tRenderer, mTexture, clip, &renderQuad);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture , red, green, blue);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
