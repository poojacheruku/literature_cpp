#include "PlayingMyTurn.hpp"
#include "LogIt.hpp"

#include <iostream>
using namespace std;

/* constructor */
PlayingMyTurn::PlayingMyTurn()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
    m_name = "It is your turn";
}

PlayingMyTurn& PlayingMyTurn::GetInstance()
{
    static PlayingMyTurn instance;       // Gets destroyed at the end
    return instance;
}

void PlayingMyTurn::Handle(const DocumentSnapshot& snapshot)
{
    log(logINFO) << GetName();
}