#include "WaitingForTurn.hpp"

/* constructor */
WaitingForTurn::WaitingForTurn()
{
	state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForTurn& WaitingForTurn::getInstance()
{
    static WaitingForTurn instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForTurn::Handle(Player * player)
{
	// player->SetState(Player::ST_PAUSED);
}
