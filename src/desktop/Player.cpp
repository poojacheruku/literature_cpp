#include "Player.hpp"
#include "PlayerState.hpp"
#include "StoppedPlaying.hpp"
#include "PlayerSettings.hpp"
#include "Hand.hpp"
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

string Player::GetPlayerName(const DocumentSnapshot& snapshot, string playerTurnId)
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

int Player::GetPlayerIndex(const DocumentSnapshot& snapshot, string playerTurnId)
{
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    int playerIndex;

    for(int i=0; i < playerList.size(); i++)
    {
        MapFieldValue playerMap = playerList[i].map_value();
        string playerId = playerMap["playerId"].string_value();
        if(playerTurnId == playerId)
        {
            playerIndex = i;
            break;
        }
    
    }
    return playerIndex;
}

string Player::GetPlayerNameAndIndex(const DocumentSnapshot& snapshot, string playerTurnId, int& playerIndex)
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
            playerIndex = i;
            break;
        }
    
    }
    return playerName;
}

void Player::PrintHand(const DocumentSnapshot& snapshot)
{
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    vector<string> hand_string;
    vector<FieldValue> hand;  
    MapFieldValue playerMap;
    int i = 0;

    for(i=0; i < playerList.size(); i++)
    {
        playerMap = playerList[i].map_value();
        string playerId = playerMap["playerId"].string_value();
        if(Player::GetInstance().GetPlayerId() == playerId)
        {
            break; 
        }
    }

    if(i == playerList.size())
    {
        logIt(logERROR) << "Error. Unable to print hand."; 
        return;
    }
    else
    {
        logIt(logINFO) << "broke out of loop"; 
        hand = playerMap["hand"].array_value(); 
    }

    for(int i=0; i < hand.size(); i++)
    {
        string card = hand[i].string_value(); 
        hand_string.push_back(card); 
    }
    
    Hand::GetInstance().Initialize(hand_string);
    Hand::GetInstance().Print();
}
