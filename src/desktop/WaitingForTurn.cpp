#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "PlayingMyTurn.hpp"
#include "Player.hpp"
#include "auth/LiteratureAuth.hpp"

using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::Firestore;

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
        string card = snapshot.Get("card").string_value(); 
        string gameCode = snapshot.Get("gameCode").string_value(); 
    
        for(int i=0; i < playerList.size(); i++)
            {
                MapFieldValue playerMap = playerList[i].map_value();
                if(playerMap["playerId"].string_value() == playerId)
                {
                    cout << playerMap["displayName"].string_value() << " is asking you for " << card << endl; 
                }
            }
        int choice; 
        cout << "Do you have this card?" << endl;
        cout << "1. Yes" << endl; 
        cout << "2. No" << endl; 
        cout << "Please select an option (1 or 2): "; 
        cin >> choice; 

        switch (choice)
        {
        case 1:
        {
            cout << "you have the card!" << endl; 
            break; 
        }
        case 2: 
        {
            Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
            DocumentReference doc_ref = db->Collection("games").Document(gameCode); 
            for(int i = 0; i < playerList.size(); i++)
            {
                MapFieldValue playerMap = playerList[i].map_value();
                string playerId = playerMap["playerId"].string_value();
                if(playerId == snapshot.Get("playerBeingAsked").string_value())
                {
                    doc_ref.Update({
                        {"turn", FieldValue::String(playerMap["displayName"].string_value())},
                        {"changeReason", FieldValue::String("TURN")}
                        });
                }
            }
            
            break; 
        }
        }
    }
    else if(changeReason == "ASK")
    {
        string beingAskedId = snapshot.Get("playerBeingAsked").string_value();
        string turnId = snapshot.Get("turn").string_value(); 
        string card = snapshot.Get("card").string_value(); 

        for(int i=0; i < playerList.size(); i++)
            {
                MapFieldValue askPlayerMap = playerList[i].map_value();
                MapFieldValue turnPlayerMap = playerList[i].map_value();
                if(turnPlayerMap["playerId"].string_value() == turnId && askPlayerMap["playerId"].string_value() == beingAskedId)
                {
                    cout << turnPlayerMap["displayName"].string_value() << " is asking " << askPlayerMap["displayName"].string_value() << " for " << card << endl; 
                }
            }
    }
}
