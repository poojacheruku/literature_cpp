#include "StoppedPlaying.hpp"

/* constructor */
StoppedPlaying::StoppedPlaying()
{
	state = Player::ST_WAITING_FOR_PLAYERS;
}

StoppedPlaying& StoppedPlaying::getInstance()
{
    static StoppedPlaying instance;       // Gets destroyed at the end
    return instance;
}

void StoppedPlaying::Handle(Player * player)
{
	// player->SetState(Player::ST_PAUSED);
}
