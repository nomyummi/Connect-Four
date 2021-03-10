#ifndef CHIP_H
#define CHIP_H

class Chip
{
public:
	Chip(int color, int player);
	Chip();
	SDL_Rect spriteSheetQuadrant() const;
	int player() const;
private:
	SDL_Rect m_spriteSheetQuadrant;
	int m_player;
};

#endif
