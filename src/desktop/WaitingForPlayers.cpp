#include "WaitingForPlayers.hpp"
#include "LogIt.hpp"

#include <iostream>
using namespace std;

/* constructor */
WaitingForPlayers::WaitingForPlayers()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForPlayers& WaitingForPlayers::GetInstance()
{
    static WaitingForPlayers instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForPlayers::WaitForPlayers()
{
    cout << "Waiting for other players!" << endl;
}

void WaitingForPlayers::Handle(const DocumentSnapshot& snapshot)
{
    if (snapshot.Get("changeReason").string_value() == "JOIN")
    {
        cout << "New player joined."; 
    }
}
