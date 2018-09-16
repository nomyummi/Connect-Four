#ifndef TITLE_H
#define TITLE_H

#include "GameState.h"
class Title : public GameState
{
public:
	Title();
	virtual ~Title();
	void handle_events();
	void render();
private:
	//Play button
	Texture *currentButton;
	Texture tPlayButton[2];
	Texture ConnectFourTitle;
};
#endif
