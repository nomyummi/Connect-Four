#ifndef GAME_STATE_H
#define GAME_STATE_H

//Base class, all virtual functions
class GameState
{
public:
	virtual void handle_events() = 0;
	virtual void render() = 0;
	virtual ~GameState() {};
};
#endif
