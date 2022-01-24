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

void printHand(const DocumentSnapshot& snapshot)
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
    Player::GetInstance().SetState(&WaitingForTurn::GetInstance());    
}

void WaitingForTurn::Handle(const DocumentSnapshot& snapshot)
{
    cout << "WaitingForTurn::Handle" << endl;

    logIt(logINFO) << GetName();
    string changeReason = snapshot.Get("changeReason").string_value(); 
    int gameStatus = snapshot.Get("gameStatus").integer_value();
    int lastAction = snapshot.Get("lastAction").integer_value();
    string playerId = snapshot.Get("turn").string_value();
    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    if(gameStatus == Actions::GAME_STATUS_STARTED)
    {
        if(lastAction == Actions::ACTION_REQUEST) {
            HandleRequestAction(snapshot);
        }
        if(Player::GetInstance().GetPlayerId() == playerId)
        {
            cout << "It's your turn to play!" << endl; 
            printHand(snapshot);
            PlayTurn(snapshot);
        }
        else 
        {
            string playerName = Player::GetInstance().GetPlayerDisplayName(snapshot, playerId);
            cout << "It's " << playerName << "'s turn to play!" << endl;  
        }
    }

    if (changeReason == "ASK" && Player::GetInstance().GetPlayerId() == snapshot.Get("playerBeingAsked").string_value())
    {
        string turnId = snapshot.Get("turn").string_value();
        string beingAskedId = snapshot.Get("playerBeingAsked").string_value(); 
        string card = snapshot.Get("card").string_value(); 
        string gameCode = Game::GetInstance().GetGameCode(); 
        int beingAskedPlayerIndex = 0;
        int askingPlayerIndex = 0; 
        MapFieldValue beingAskedPlayerMap;
        MapFieldValue askingPlayerMap; 

        // Print the message saying card is being asked for
        for(askingPlayerIndex=0; askingPlayerIndex < playerList.size(); askingPlayerIndex++)
        {
            askingPlayerMap = playerList[askingPlayerIndex].map_value();
            if(askingPlayerMap["playerId"].string_value() == turnId)
            {
                cout << askingPlayerMap["displayName"].string_value() << " is asking you for " << card << endl;
                break; 
            }
        }

        cout << "ASKING INDEX: " << askingPlayerIndex << endl; 
        // Find the index of the player of the player being asked (you)
        for(beingAskedPlayerIndex=0; beingAskedPlayerIndex < playerList.size(); beingAskedPlayerIndex++)
        {
            beingAskedPlayerMap = playerList[beingAskedPlayerIndex].map_value();
            if(beingAskedPlayerMap["playerId"].string_value() == beingAskedId)
            {
                // cout << "Found player index: " << playerIndex << endl;
                break; 
            }
        }

        cout << "BEING ASKED INDEX: " << beingAskedPlayerIndex << endl; 

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
            vector<FieldValue> beingAskedHand = beingAskedPlayerMap["hand"].array_value();
            vector<FieldValue> beingAskedNewHand;  
            vector<string> beingAskedHand_string;
            vector<FieldValue> askingHand = askingPlayerMap["hand"].array_value(); 
            vector<FieldValue> askingNewHand;  
            vector<string> askingHand_string;

            cout << "HAND SIZE BEFORE: " << beingAskedHand.size() << endl; 

            for(int i=0; i < beingAskedHand.size(); i++)
            {
                string card = beingAskedHand[i].string_value(); 
                beingAskedHand_string.push_back(card); 
            }

            for(int i=0; i < askingHand.size(); i++)
            {
                string card = askingHand[i].string_value(); 
                askingHand_string.push_back(card); 
            }

            // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

            beingAskedHand_string.erase(remove(beingAskedHand_string.begin(), beingAskedHand_string.end(), card), beingAskedHand_string.end());
            
            askingHand_string.push_back(card); 

            // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

            for(int i = 0; i < beingAskedHand_string.size(); i++)
            {
                cout << FieldValue::String(beingAskedHand_string[i]) << endl; 
                beingAskedNewHand.push_back(FieldValue::String(beingAskedHand_string[i]));
            }

            for(int i = 0; i < askingHand_string.size(); i++)
            {
                cout << FieldValue::String(askingHand_string[i]) << endl; 
                askingNewHand.push_back(FieldValue::String(askingHand_string[i]));
            }
            
            // cout << "HAND SIZE AFTER: " << newHand.size() << endl; 

            // cout << "hand: " << endl; 

            // for(int i = 0; i < newHand.size(); i++)
            // {
            //     cout << newHand[i] << endl; 
            // }

            Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
            DocumentReference doc_ref = db->Collection("games").Document(gameCode);

            // cout << "PLAYER INDEX: " << playerIndex << endl;
            // cout << "Player name: " << playerMap["displayName"].string_value();
            // cout << "Player Id: " << playerMap["playerId"].string_value();
            // cout << "Player team: " << playerMap["team"].integer_value();
            beingAskedPlayerMap["hand"] = FieldValue::Array(beingAskedNewHand);
            playerList[beingAskedPlayerIndex] = FieldValue::Map(beingAskedPlayerMap);
            askingPlayerMap["hand"] = FieldValue::Array(askingNewHand); 
            playerList[askingPlayerIndex] = FieldValue::Map(askingPlayerMap);

            
            doc_ref.Update({
                {"players",  FieldValue::Array(playerList)},
                {"changeReason", FieldValue::String("HASCARD")}
            });

            break; 
        }
        case 2:
        {
            string nextTurnPlayerId = Player::GetInstance().GetPlayerId();
            Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
            DocumentReference doc_ref = db->Collection("games").Document(gameCode); 
            
            doc_ref.Update({
                {"turn", FieldValue::String(nextTurnPlayerId)},
                {"changeReason", FieldValue::String("TURN")}
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

void WaitingForTurn::PlayTurn(const DocumentSnapshot& snapshot)
{
    cout << "WaitingForTurn::PlayTurn" << endl;
    
    string changeReason = snapshot.Get("changeReason").string_value();
    string turnId = snapshot.Get("turn").string_value();
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    int playerIndex = 0;
    MapFieldValue playerMap;

    // for(playerIndex=0; playerIndex < playerList.size(); playerIndex++)
    //     {
    //         playerMap = playerList[playerIndex].map_value();
    //         if(playerMap["playerId"].string_value() == turnId)
    //         {
    //             cout << "Found player index: " << playerIndex << endl;
    //             break; 
    //         }
    //     }

    int choice; 
    cout << "What do you want to do? Choose an option (1 or 2)" << endl;
    cout << "1. Ask for a card" << endl;
    cout << "2. Make a set" << endl;
    cin >> choice;

    // vector<string> playerNames = Game::GetInstance().GetPlayerNames();
    // logIt(logINFO) << playerNames.size();
    switch (choice)
    {
        case 1:
        {
            cout << "Who do you want to ask?" << endl;  

            for(int i = 0; i < playerList.size(); i++)
            {
                MapFieldValue playerMap = playerList[i].map_value();
                string displayName = playerMap["displayName"].string_value();
                int team = playerMap["team"].integer_value();

                // if(Player::GetInstance().GetTeam() != team) {
                    int number = i + 1; 
                    cout << number << ". " << displayName << endl; 
                // }
            }
            int choice; 
            cout << "Enter a number: ";
            cin >> choice; 
            cout << endl; 

            string input;  
            cout << "What card do you want to ask for?" << endl; 
            cout << endl; 
            cout << "Please enter in this format:" << endl; 
            cout << "H-K (king of hearts)" << endl; 
            cout << "D-1 (one of diamonds)" << endl; 
            cout << endl; 
            cin >> input; 
            cout << endl; 

            char suit = input.at(0);
            char value = input.at(2);   

            string card; 
            if(suit == 'S')
            {
                card = "\u2660"; 
                card.push_back(value); 
            }
            else if(suit == 'C')
            {
                card = "\u2664"; 
                card.push_back(value);
            }
            else if(suit == 'H')
            {
                card = "\u2665"; 
                card.push_back(value);
            }
            else if(suit == 'D')
            {
                card = "\u2666"; 
                card.push_back(value);
            }

            int playerNumber = choice - 1; 
            MapFieldValue playerMap = playerList[playerNumber].map_value();
            string askName = playerMap["displayName"].string_value(); 
            string askPlayerId = playerMap["playerId"].string_value();
            string gameCode = Game::GetInstance().GetGameCode();
            MapFieldValue requestMap;
            requestMap["fromId"] = FieldValue::String(Player::GetInstance().GetPlayerId());
            requestMap["toId"] = FieldValue::String(askPlayerId);
            requestMap["status"] = FieldValue::Integer(Actions::ACTION_STATUS_WAITING); 
            requestMap["card"] = FieldValue::String(card);

            cout << "You are asking " << askName << " for " << card << endl;
            cout << endl;  

            Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
            DocumentReference doc_ref = db->Collection("games").Document(gameCode);
            doc_ref.Update({
                {"playerBeingAsked", FieldValue::String(askPlayerId)},
                {"card", FieldValue::String(card)},
                {"lastAction", FieldValue::Integer(Actions::ACTION_REQUEST)},
                {"changeReason", FieldValue::String("ASK")},
                {"request", FieldValue::Map(requestMap)}
            });
            // for(int i = 1; i < playerNames.size(); i++)
            // {
            //     cout << i << ". " << playerNames[i] << endl;
            // }
            break; 
        }
        case 2:
        {
            break;
        }
    }
}

void WaitingForTurn::HandleRequestAction(const DocumentSnapshot& snapshot)
{
    MapFieldValue requestMap = snapshot.Get("request").map_value();
    string fromId = requestMap["fromId"].string_value();
    string toId = requestMap["toId"].string_value();
    int requestStatus = requestMap["status"].integer_value();
    string card = requestMap["card"].string_value();

    if(requestStatus == Actions::ACTION_STATUS_WAITING) {
        if(toId == Player::GetInstance().GetPlayerId()) {
            cout << Player::GetInstance().GetPlayerDisplayName(snapshot, fromId) << " is requesting " << card << " from you."<< endl;
            HandleRequest(snapshot, requestMap);
        }
    }
}

void WaitingForTurn::HandleRequest(const DocumentSnapshot& snapshot, MapFieldValue& requestMap)
{
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    string card = requestMap["card"].string_value();
    string gameCode = Game::GetInstance().GetGameCode(); 
    int beingAskedPlayerIndex = 0;
    int askingPlayerIndex = 0; 
    MapFieldValue beingAskedPlayerMap;
    MapFieldValue askingPlayerMap; 

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
        vector<FieldValue> beingAskedHand = beingAskedPlayerMap["hand"].array_value();
        vector<FieldValue> beingAskedNewHand;  
        vector<string> beingAskedHand_string;
        vector<FieldValue> askingHand = askingPlayerMap["hand"].array_value(); 
        vector<FieldValue> askingNewHand;  
        vector<string> askingHand_string;

        cout << "HAND SIZE BEFORE: " << beingAskedHand.size() << endl; 

        for(int i=0; i < beingAskedHand.size(); i++)
        {
            string card = beingAskedHand[i].string_value(); 
            beingAskedHand_string.push_back(card); 
        }

        for(int i=0; i < askingHand.size(); i++)
        {
            string card = askingHand[i].string_value(); 
            askingHand_string.push_back(card); 
        }

        // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

        beingAskedHand_string.erase(remove(beingAskedHand_string.begin(), beingAskedHand_string.end(), card), beingAskedHand_string.end());
        
        askingHand_string.push_back(card); 

        // cout << "HAND STRING SIZE: " << hand_string.size() << endl; 

        for(int i = 0; i < beingAskedHand_string.size(); i++)
        {
            cout << FieldValue::String(beingAskedHand_string[i]) << endl; 
            beingAskedNewHand.push_back(FieldValue::String(beingAskedHand_string[i]));
        }

        for(int i = 0; i < askingHand_string.size(); i++)
        {
            cout << FieldValue::String(askingHand_string[i]) << endl; 
            askingNewHand.push_back(FieldValue::String(askingHand_string[i]));
        }
        

        Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
        DocumentReference doc_ref = db->Collection("games").Document(gameCode);

        beingAskedPlayerMap["hand"] = FieldValue::Array(beingAskedNewHand);
        playerList[beingAskedPlayerIndex] = FieldValue::Map(beingAskedPlayerMap);
        askingPlayerMap["hand"] = FieldValue::Array(askingNewHand); 
        playerList[askingPlayerIndex] = FieldValue::Map(askingPlayerMap);
        requestMap["status"] = FieldValue::Integer(Actions::ACTION_STATUS_ACCEPTED);

        
        doc_ref.Update({
            {"players",  FieldValue::Array(playerList)},
            {"changeReason", FieldValue::String("HASCARD")},
            {"request", FieldValue::Map(requestMap)}
        });

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
            {"changeReason", FieldValue::String("TURN")},
            {"request", FieldValue::Map(requestMap)}
            });

        cout << "It's your turn to play!" << endl;
        PlayTurn(snapshot);
        break; 
    }
    }
}