#include "Player.hpp"
#include "PlayerState.hpp"
#include "StoppedPlaying.hpp"
#include "PlayerSettings.hpp"
#include "LogIt.hpp"

using ::firebase::firestore::FieldValue;
using ::firebase::firestore::MapFieldValue;

#include <iostream>
#include <future>
#include <thread>

using namespace std;

/* constructor */
Player::Player()
{
}

Player& Player::GetInstance()
{
    static Player instance;       // Gets destroyed at the end
    return instance;
}

void Player::Handle(const DocumentSnapshot& snapshot) {
    cout << "Player::Handle" << endl;
    logIt(logINFO) << "Passing the handle to State";
	// m_pState->Handle(snapshot);
    std::thread threadObj(&PlayerState::Handle, m_pState, snapshot);
    threadObj.detach();
}

void Player::WaitForPlayers() {
    logIt(logINFO) << "WaitForPlayers called";
    m_pState->WaitForPlayers();
}

void Player::Start() {
    SetState(&StoppedPlaying::GetInstance());
	m_pState->Start();
}

void Player::SetState(PlayerState * state)
{
	m_pState = state;
}

string Player::GetPlayerDisplayName(const DocumentSnapshot& snapshot, string playerTurnId)
{
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    string playerName;

    for(int i=0; i < playerList.size(); i++)
    {
        MapFieldValue playerMap = playerList[i].map_value();
        string playerId = playerMap["playerId"].string_value();
        if(playerTurnId == playerId)
        {
            playerName = playerMap["displayName"].string_value();
            break;
        }
    
    }
    return playerName;
}