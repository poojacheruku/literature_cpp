#ifndef STOPPED_STATE_H
#define STOPPED_STATE_H

#include "PlayerState.h"
#include "Player.h"

class StoppedState : public PlayerState {
public:
	StoppedState();
	virtual ~StoppedState();

	virtual void Play(Player * player);
};

#endif // STOPPED_STATE_H