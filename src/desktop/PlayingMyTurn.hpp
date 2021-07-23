#ifndef LITERATURE_PLAYING_MY_TURN_H
#define LITERATURE_PLAYING_MY_TURN_H

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
    static PlayingMyTurn& GetInstance();
	void Handle(const DocumentSnapshot& snapshot);
    void PlayTurn(); 
};

#endif  // LITERATURE_PLAYING_MY_TURN_H  // NOLINT