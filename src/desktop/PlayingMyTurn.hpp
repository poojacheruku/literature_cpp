#ifndef PLAYING_MY_TURN_H
#define PLAYING_MY_TURN_H

#include "PlayerState.hpp"
#include "Player.hpp"

class PlayingMyTurn : public PlayerState
{
private:
    /* data */

    /* constructor */
    PlayingMyTurn();
    PlayingMyTurn(PlayingMyTurn const&);              // Don't implement
    void operator=(PlayingMyTurn const&);    // Don't implement

public:
    static PlayingMyTurn& getInstance();
	virtual void Handle(Player * player);
};

#endif  // PLAYING_MY_TURN_H  // NOLINT