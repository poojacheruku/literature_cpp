#ifndef WAITING_FOR_TURN_H
#define WAITING_FOR_TURN_H

#include "PlayerState.h"
#include "Player.h"

class WaitingForTurn : public PlayerState
{
private:
    /* data */

    /* constructor */
    WaitingForTurn();
    WaitingForTurn(WaitingForTurn const&);              // Don't implement
    void operator=(WaitingForTurn const&);    // Don't implement

public:
    static WaitingForTurn& getInstance();
	virtual void Handle(Player * player);
};

#endif  // WAITING_FOR_TURN_H  // NOLINT