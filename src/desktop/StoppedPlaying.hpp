#ifndef STOPPED_PLAYING_H
#define STOPPED_PLAYING_H

#include "PlayerState.hpp"
#include "Player.hpp"

class StoppedPlaying : public PlayerState
{
private:
    /* data */

    /* constructor */
    StoppedPlaying();
    StoppedPlaying(StoppedPlaying const&);              // Don't implement
    void operator=(StoppedPlaying const&);    // Don't implement

public:
    static StoppedPlaying& getInstance();
	virtual void Handle(Player * player);
};

#endif  // STOPPED_PLAYING_H  // NOLINT