#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "PlayingMyTurn.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "auth/LiteratureAuth.hpp"

using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::Firestore;

#include <algorithm>
#include <vector>
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
        string gameCode = Game::GetInstance().GetGameCode(); 
    
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
        cout << endl; 

        switch (choice)
        {
        case 1:
        {
            MapFieldValue playerMap; 
            vector<FieldValue> hand; 
            vector<string> hand_string;

            for(int i =0; i < playerList.size(); i++)
            {
                playerMap = playerList[i].map_value();

                if(playerMap["playerId"].string_value() == snapshot.Get("playerBeingAsked").string_value())
                {
                    hand = playerMap["hand"].array_value(); 
                }
            }

            cout << "HAND SIZE BEFORE: " << hand.size() << endl; 

            for(int i=0; i < hand.size(); i++)
            {
                string card = hand[i].string_value(); 
                hand_string.push_back(card); 
            }

            cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

            hand_string.erase(remove(hand_string.begin(), hand_string.end(), card), hand_string.end());

            cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

            for(int i = 0; i < hand_string.size(); i++)
            {
                cout << FieldValue::String(hand_string[i]) << endl; 
                hand.push_back(FieldValue::String(hand_string[i]));
            }
            
            cout << "HAND SIZE AFTER: " << hand.size() << endl; 

            cout << "hand: " << endl; 

            for(int i = 0; i < hand.size(); i++)
            {
                cout << hand[i] << endl; 
            }

            playerMap["hand"] = FieldValue::Array(hand);

            break; 
        }
        case 2:
        {
            string nextTurnPlayerId = Player::GetInstance().GetPlayerId();
            Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
            DocumentReference doc_ref = db->Collection("games").Document(gameCode); 
            
            doc_ref.Update({
                {"turn", FieldValue::String(nextTurnPlayerId)},
                {"changeReason", FieldValue::String("NOCARD")}
                });

            cout << "It's your turn to play!" << endl;
            Player::GetInstance().SetState(&PlayingMyTurn::GetInstance());
            PlayingMyTurn::GetInstance().PlayTurn(snapshot);
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

    if(changeReason == "NOCARD")
    {
        string card = snapshot.Get("card").string_value();

        for(int i = 0; i < playerList.size(); i++)
        {
            MapFieldValue playerMap = playerList[i].map_value();
            if(playerMap["playerId"].string_value() == snapshot.Get("turn").string_value())
            {
                    cout << playerMap["displayName"].string_value() << " does not have " << card << endl; 
                    cout << endl; 
                    cout << "It's " << playerMap["displayName"].string_value() << "'s turn to play!"; 
            }
        }

    }

    if(changeReason == "HASCARD")
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
                    cout << askPlayerMap["displayName"].string_value() << " has " << card << endl; 
                    cout << endl; 
                    cout << askPlayerMap["displayName"].string_value() << " is giving " << turnPlayerMap["displayName"].string_value() << card << endl; 
                    cout << endl; 
                    cout << "It's " << turnPlayerMap["displayName"].string_value() << "'s turn to play again!" << endl; 
                }
            }

    }
}
