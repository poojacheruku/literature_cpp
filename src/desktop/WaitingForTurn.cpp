#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "PlayingMyTurn.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Actions.hpp"
#include "Hand.hpp"

#include "auth/LiteratureAuth.hpp"

using ::firebase::firestore::FieldValue;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::Firestore;

#include <algorithm>
#include <vector>
#include <iostream>
#include <string> 

using namespace std;

/* constructor */
WaitingForTurn::WaitingForTurn()
{
	m_state = Player::ST_WAITING_FOR_TURN;
}

WaitingForTurn& WaitingForTurn::GetInstance()
{
    static WaitingForTurn instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForTurn::Handle(const DocumentSnapshot& snapshot)
{
    cout << "WaitingForTurn::Handle" << endl;

    Hand::GetInstance().Initialize(snapshot);

    logIt(logINFO) << GetName();
    int gameStatus = snapshot.Get("gameStatus").integer_value();
    int lastAction = snapshot.Get("lastAction").integer_value();
    string playerId = snapshot.Get("turn").string_value();
    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    if(gameStatus == Actions::GAME_STATUS_STARTED)
    {
        if(Player::GetInstance().GetPlayerId() == playerId)
        {
            cout << "It's your turn to play!" << endl; 
            Player::GetInstance().SetState(&PlayingMyTurn::GetInstance());
            PlayingMyTurn::GetInstance().PlayTurn(snapshot);
        }
        else 
        {
            string playerName = Player::GetInstance().GetPlayerName(snapshot, playerId);
            cout << "It's " << playerName << "'s turn to play!" << endl;  

            if(lastAction == Actions::ACTION_REQUEST) {
                HandleRequestAction(snapshot);
            }

            if(lastAction == Actions::ACTION_CALL_SET) {
                HandleRequestCallSet(snapshot); 
            }
        }
    }
}

void WaitingForTurn::HandleRequestCallSet(const DocumentSnapshot& snapshot)
{
    string callingSet = snapshot.Get("callingSet").string_value();
    string playerId = snapshot.Get("turn").string_value();
    string playerName = Player::GetInstance().GetPlayerName(snapshot, playerId);
    cout << playerName << " is calling " << callingSet << "set" << endl; 
}

void WaitingForTurn::HandleRequestAction(const DocumentSnapshot& snapshot)
{
    MapFieldValue requestMap = snapshot.Get("request").map_value();
    string fromId = requestMap["fromId"].string_value();
    string toId = requestMap["toId"].string_value();
    int requestStatus = requestMap["status"].integer_value();
    string card = requestMap["card"].string_value();

    switch (requestStatus)
    {
    case Actions::ACTION_STATUS_WAITING:
        if(toId == Player::GetInstance().GetPlayerId()) {
            cout << Player::GetInstance().GetPlayerName(snapshot, fromId) << " is requesting " << card << " from you."<< endl;
            HandleRequest(snapshot, requestMap);
        } else {
            cout << Player::GetInstance().GetPlayerName(snapshot, fromId) 
                 << " is requesting " 
                 << card 
                 << " from " 
                 << Player::GetInstance().GetPlayerName(snapshot, toId) 
                 << endl;
        }
        break;

    case Actions::ACTION_STATUS_ACCEPTED:
        cout << Player::GetInstance().GetPlayerName(snapshot, fromId) 
                << " received the card " 
                << card 
                << " from " 
                << Player::GetInstance().GetPlayerName(snapshot, toId) 
                << endl;
        cout << "It is " << Player::GetInstance().GetPlayerName(snapshot, fromId) << "'s turn" << endl;
        break;

    case Actions::ACTION_STATUS_REJECTED:
        cout << Player::GetInstance().GetPlayerName(snapshot, fromId) 
                << " does not have the card " 
                << card 
                << " from " 
                << Player::GetInstance().GetPlayerName(snapshot, toId) 
                << endl;
        cout << "It is " << Player::GetInstance().GetPlayerName(snapshot, toId) << "'s turn" << endl;
        break;
    default:
        break;
    }
}

void WaitingForTurn::HandleRequest(const DocumentSnapshot& snapshot, MapFieldValue& requestMap)
{
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    string gameCode = Game::GetInstance().GetGameCode(); 
    string card = requestMap["card"].string_value();
    string fromId = requestMap["fromId"].string_value();
    string toId = requestMap["toId"].string_value();
    int fromIndex = Player::GetInstance().GetPlayerIndex(snapshot, fromId);
    int toIndex = Player::GetInstance().GetPlayerIndex(snapshot, toId);
    MapFieldValue fromMap = playerList[fromIndex].map_value();;
    MapFieldValue toMap = playerList[toIndex].map_value();;

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
        vector<FieldValue> fromHand = fromMap["hand"].array_value();
        vector<FieldValue> toHand = toMap["hand"].array_value();
        vector<FieldValue> fromHandNew;
        vector<FieldValue> toHandNew;
        vector<string> fromHandString;
        vector<string> toHandString;


        cout << "Requestor hand size before: " << fromHand.size() << endl; 

        for(int i=0; i < fromHand.size(); i++)
        {
            fromHandString.push_back(fromHand[i].string_value()); 
        }

        for(int i=0; i < toHand.size(); i++)
        {
            toHandString.push_back(toHand[i].string_value()); 
        }

        // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

        toHandString.erase(remove(toHandString.begin(), toHandString.end(), card), toHandString.end());
        
        fromHandString.push_back(card); 

        // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

        for(int i = 0; i < toHandString.size(); i++)
        {
            toHandNew.push_back(FieldValue::String(toHandString[i]));
        }

        for(int i = 0; i < fromHandString.size(); i++)
        {
            fromHandNew.push_back(FieldValue::String(fromHandString[i]));
        }
        

        Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
        DocumentReference doc_ref = db->Collection("games").Document(gameCode);

        toMap["hand"] = FieldValue::Array(toHandNew);
        playerList[toIndex] = FieldValue::Map(toMap);
        fromMap["hand"] = FieldValue::Array(fromHandNew); 
        playerList[fromIndex] = FieldValue::Map(fromMap);
        requestMap["status"] = FieldValue::Integer(Actions::ACTION_STATUS_ACCEPTED);

        
        doc_ref.Update({
            {"players",  FieldValue::Array(playerList)},
            {"request", FieldValue::Map(requestMap)}
        });

        cout << "Card " << card << " transfered to " << Player::GetInstance().GetPlayerName(snapshot, fromId) << endl;
        cout << "It's " << Player::GetInstance().GetPlayerName(snapshot, fromId) << "'s turn" << endl;
        Hand::GetInstance().PrettyPrint(toHandString);
        break; 
    }
    case 2:
    {
        string nextTurnPlayerId = Player::GetInstance().GetPlayerId();
        Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
        DocumentReference doc_ref = db->Collection("games").Document(gameCode); 
        requestMap["status"] = FieldValue::Integer(Actions::ACTION_STATUS_REJECTED);
        
        doc_ref.Update({
            {"turn", FieldValue::String(nextTurnPlayerId)},
            {"request", FieldValue::Map(requestMap)}
            });

        cout << "It's your turn to play!" << endl;
        Player::GetInstance().SetState(&PlayingMyTurn::GetInstance());
        PlayingMyTurn::GetInstance().PlayTurn(snapshot);
        break; 
    }
    }
}