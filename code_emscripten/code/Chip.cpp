#include <SDL.h>
#include <stdio.h>
#include "constants.h"
#include "Chip.h"


Chip::Chip(int color, int player)
{
	switch (color)
	{
	case REDCHIP:
		m_spriteSheetQuadrant.x = 0;
		m_spriteSheetQuadrant.y = 0;
		m_spriteSheetQuadrant.w = 150;
		m_spriteSheetQuadrant.h = 150;
		break;
	case BLUECHIP:
		m_spriteSheetQuadrant.x = 150;
		m_spriteSheetQuadrant.y = 0;
		m_spriteSheetQuadrant.w = 150;
		m_spriteSheetQuadrant.h = 150;
		break;
	case YELLOWCHIP:
		m_spriteSheetQuadrant.x = 0;
		m_spriteSheetQuadrant.y = 150;
		m_spriteSheetQuadrant.w = 150;
		m_spriteSheetQuadrant.h = 150;
		break;
	case GREENCHIP:
		m_spriteSheetQuadrant.x = 150;
		m_spriteSheetQuadrant.y = 150;
		m_spriteSheetQuadrant.w = 150;
		m_spriteSheetQuadrant.h = 150;
		break;
	default:
		printf("Error - Not a Chip Color");
	}
	m_player = player;
}

Chip::Chip()
{
	m_player;
	m_spriteSheetQuadrant;
}
SDL_Rect Chip::spriteSheetQuadrant() const
{
	return m_spriteSheetQuadrant;
}

int Chip::player() const
{
	return m_player;
}