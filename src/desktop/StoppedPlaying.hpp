#ifndef LITERATURE_STOPPED_PLAYING_H
#define LITERATURE_STOPPED_PLAYING_H

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
    static StoppedPlaying& GetInstance();
	void Start();
	void Handle(const DocumentSnapshot& snapshot);
};

#endif  // LITERATURE_STOPPED_PLAYING_H  // NOLINT