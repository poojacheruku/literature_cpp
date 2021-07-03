#include "WaitingForPlayers.hpp"

/* constructor */
WaitingForPlayers::WaitingForPlayers()
{
	state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForPlayers& WaitingForPlayers::getInstance()
{
    static WaitingForPlayers instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForPlayers::Handle(Player * player)
{
	// player->SetState(Player::ST_PAUSED);
}
