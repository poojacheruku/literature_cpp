#ifndef LITERATURE_WAITING_FOR_PLAYERS_H
#define LITERATURE_WAITING_FOR_PLAYERS_H

#include "PlayerState.hpp"
#include "Player.hpp"

class WaitingForPlayers : public PlayerState
{
private:
    /* data */

    /* constructor */
    WaitingForPlayers();
    WaitingForPlayers(WaitingForPlayers const&);              // Don't implement
    void operator=(WaitingForPlayers const&);    // Don't implement

public:
    static WaitingForPlayers& getInstance();
	void Handle();
};

#endif  // LITERATURE_WAITING_FOR_PLAYERS_H  // NOLINT