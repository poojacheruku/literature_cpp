#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "PlayerState.h"
#include "Player.h"

class PlayingState : public PlayerState {
public:
	PlayingState();
	virtual ~PlayingState();

	virtual void Pause(Player * player);
	virtual void Stop(Player * player);
};

#endif // PLAYING_STATE_H