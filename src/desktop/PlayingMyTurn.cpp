#include "PlayingMyTurn.hpp"

/* constructor */
PlayingMyTurn::PlayingMyTurn()
{
	state = Player::ST_WAITING_FOR_PLAYERS;
}

PlayingMyTurn& PlayingMyTurn::getInstance()
{
    static PlayingMyTurn instance;       // Gets destroyed at the end
    return instance;
}

void PlayingMyTurn::Handle(Player * player)
{
	// player->SetState(Player::ST_PAUSED);
}
