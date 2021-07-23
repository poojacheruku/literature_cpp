#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "PlayingMyTurn.hpp"
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
            PlayingMyTurn::GetInstance().PlayTurn();
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
}
