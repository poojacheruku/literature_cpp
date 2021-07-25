#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "PlayingMyTurn.hpp"
#include "Player.hpp"
using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::FieldValue;

#include <iostream>
using namespace std;

/* constructor */
WaitingForTurn::WaitingForTurn()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForTurn& WaitingForTurn::GetInstance()
{
    static WaitingForTurn instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForTurn::Handle(const DocumentSnapshot& snapshot)
{
    logIt(logINFO) << GetName();
    string changeReason = snapshot.Get("changeReason").string_value(); 
    string playerId = snapshot.Get("turn").string_value();
    vector<FieldValue> playerList = snapshot.Get("players").array_value();


    if(changeReason == "TURN")
    {
        if(Player::GetInstance().GetPlayerId() == playerId)
        {
            cout << "It's your turn to play!" << endl; 
            Player::GetInstance().SetState(&PlayingMyTurn::GetInstance());
            PlayingMyTurn::GetInstance().PlayTurn(snapshot);
        }
        else 
        {
            for(int i=0; i < playerList.size(); i++)
            {
                MapFieldValue playerMap = playerList[i].map_value();
                string playerId = playerMap["playerId"].string_value();
                if(Player::GetInstance().GetPlayerId() == playerId)
                {
                    cout << "It's " << playerMap["displayName"].string_value() << "'s turn to play!" << endl;  
                    break;
                }
            
            }
            
        }
    }

    if (changeReason == "ASK" && Player::GetInstance().GetPlayerId() == snapshot.Get("playerBeingAsked").string_value())
    {
        string playerId = snapshot.Get("turn").string_value();
        for(int i=0; i < playerList.size(); i++)
            {
                MapFieldValue playerMap = playerList[i].map_value();
                if(playerMap["playerId"].string_value() == playerId)
                {
                    cout << playerMap["displayName"].string_value() << " is asking you for a card..." << endl; 
                }
            }
    }
    else if(changeReason == "ASK")
    {
        string beingAskedId = snapshot.Get("playerBeingAsked").string_value();
        string turnId = snapshot.Get("turn").string_value(); 
        for(int i=0; i < playerList.size(); i++)
            {
                MapFieldValue askPlayerMap = playerList[i].map_value();
                MapFieldValue turnPlayerMap = playerList[i].map_value();
                if(turnPlayerMap["playerId"].string_value() == turnId && askPlayerMap["playerId"].string_value() == beingAskedId)
                {
                    cout << turnPlayerMap["displayName"].string_value() << " is asking " << askPlayerMap["displayName"].string_value() << " for a card..." << endl; 
                }
            }
    }
}
