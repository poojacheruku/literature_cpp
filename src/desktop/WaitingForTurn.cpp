#include "WaitingForTurn.hpp"

#include <iostream>
using namespace std;

/* constructor */
WaitingForTurn::WaitingForTurn()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForTurn& WaitingForTurn::getInstance()
{
    static WaitingForTurn instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForTurn::Handle()
{
    cout << GetName() << endl;
}
