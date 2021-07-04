#include "WaitingForPlayers.hpp"

#include <iostream>
using namespace std;

/* constructor */
WaitingForPlayers::WaitingForPlayers()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForPlayers& WaitingForPlayers::getInstance()
{
    static WaitingForPlayers instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForPlayers::Handle()
{
    cout << GetName() << endl;
}
