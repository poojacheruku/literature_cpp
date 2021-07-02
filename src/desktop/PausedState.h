#ifndef PAUSED_STATE_H
#define PAUSED_STATE_H

#include "PlayerState.h"

class PausedState : public PlayerState {
public:
	PausedState();
	virtual ~PausedState();

	virtual void Play(Player * player);
	virtual void Stop(Player * player);
};

#endif // PAUSED_STATE_H