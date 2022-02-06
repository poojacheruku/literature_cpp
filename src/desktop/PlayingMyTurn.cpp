#include "PlayingMyTurn.hpp"
#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "Game.hpp"
#include "Card.hpp"
#include "Actions.hpp"
#include "Hand.hpp"

#include "auth/LiteratureAuth.hpp"

#include <algorithm>
#include <vector>
#include <string> 

#include <iostream>
using namespace std;

using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::Firestore;

/* constructor */
PlayingMyTurn::PlayingMyTurn()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
    m_name = "It is your turn to play!";
}

PlayingMyTurn& PlayingMyTurn::GetInstance()
{
    static PlayingMyTurn instance;       // Gets destroyed at the end
    return instance;
}

void PlayingMyTurn::Handle(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::Handle" << endl;

    Hand::GetInstance().Initialize(snapshot);

    int gameStatus = snapshot.Get("gameStatus").integer_value();
    int lastAction = snapshot.Get("lastAction").integer_value();
    string playerId = snapshot.Get("turn").string_value();

    if(gameStatus == Actions::GAME_STATUS_STARTED)
    {
        if(lastAction == Actions::ACTION_REQUEST) {
            HandleRequestAction(snapshot);
        }
    }
}

void PlayingMyTurn::PlayTurn(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::PlayTurn" << endl;

    int choice; 
    cout << "What do you want to do? Choose an option (1 or 2)" << endl;
    cout << "1. Ask for a card" << endl;
    cout << "2. Make a set" << endl;
    cout << "3. Declare a set" << endl;
    cin >> choice;

    switch (choice)
    {
        case 1:
        {
            AskForACard(snapshot, false);
            break; 
        }
        case 2:
        {
            MakeASet(snapshot); 
            break;
        }
        case 3:
        {
            DeclareASet(snapshot);
            break;
        }
    }
}

void PlayingMyTurn::HandleRequestAction(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::HandleRequestAction" << endl;
    
    MapFieldValue requestMap = snapshot.Get("request").map_value();
    string fromId = requestMap["fromId"].string_value();
    string toId = requestMap["toId"].string_value();
    int requestStatus = requestMap["status"].integer_value();
    string card = requestMap["card"].string_value();
    string otherPlayer = Player::GetInstance().GetPlayerName(snapshot, toId);
    int lastAction = snapshot.Get("lastAction").integer_value();

    if(requestStatus == Actions::ACTION_STATUS_ACCEPTED) {
        cout << otherPlayer << " transfered the card " << card << " to you" << endl;
        cout << "It's your turn to play!" << endl;
        Hand::GetInstance().PrettyPrint(snapshot);

        if(lastAction == Actions::ACTION_DECLARE) {
            DeclareASet(snapshot);
        } else {
            PlayTurn(snapshot);
        }
    }
    else if(requestStatus == Actions::ACTION_STATUS_REJECTED) {
        cout << otherPlayer << " does not have the card " << card << endl;
        cout << "It's " << otherPlayer << "'s turn" << endl;
        Hand::GetInstance().PrettyPrint();
        Player::GetInstance().SetState(&WaitingForTurn::GetInstance());
    }
}

void PlayingMyTurn::AskForACard(const DocumentSnapshot& snapshot, bool ownTeam)
{
    cout << "PlayingMyTurn::AskForACard" << endl;
    
    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    cout << "Whom do you want to ask?" << endl;  

    for(int i = 0; i < playerList.size(); i++)
    {
        MapFieldValue playerMap = playerList[i].map_value();
        string displayName = playerMap["displayName"].string_value();
        int team = playerMap["team"].integer_value();

        if(ownTeam) {
            if(Player::GetInstance().GetTeam() == team) {
                cout << (i+1) << ". " << displayName << endl; 
            }
        } else {
            if(Player::GetInstance().GetTeam() != team) {
                cout << (i+1) << ". " << displayName << endl; 
            }
        }
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

    vector<string> tokens;
    Hand::GetInstance().TokenizeCardString(tokens, input);

    char suit = tokens[0].at(0);
    string value = tokens[1];   

    string card; 
    if(suit == 'S')
    {
        card = "\u2660-"; 
    }
    else if(suit == 'C')
    {
        card = "\u2663-"; 
    }
    else if(suit == 'H')
    {
        card = "\u2665-"; 
    }
    else if(suit == 'D')
    {
        card = "\u2666-"; 
    }

    if(value == "10") value = "\u2469";
    card += value;

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

    if(ownTeam) {
        doc_ref.Update({
            {"lastAction", FieldValue::Integer(Actions::ACTION_DECLARE)},
            {"request", FieldValue::Map(requestMap)}
        });
    } else {
        doc_ref.Update({
            {"lastAction", FieldValue::Integer(Actions::ACTION_REQUEST)},
            {"request", FieldValue::Map(requestMap)}
        });
    }
}

void PlayingMyTurn::MakeASet(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::MakeASet" << endl; 

    string suit; 
    string set; 

    cout << "What set do you want to call?" << endl; 
    cout << "Please enter the set in this format: " << endl; 
    cout << "H low  (low hearts set)" << endl; 
    cout << "D high (high diamonds set)" << endl; 

    cin >> suit; 
    cin >> set; 

    string card;
    string removeCard;
    if(suit == "S")
    {
        card = "\u2660-"; 
    }
    else if(suit == "C")
    {
        card = "\u2663-"; 
    }
    else if(suit == "H")
    {
        card = "\u2665-"; 
    }
    else if(suit == "D")
    {
        card = "\u2666-"; 
    }
    
    Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
    string gameCode = Game::GetInstance().GetGameCode();
    DocumentReference doc_ref = db->Collection("games").Document(gameCode);

    string setCalled = set + card; 
  
    // doc_ref.Set({
    //     {"setCalled", FieldValue::String(setCalled)},
    // }); 

    doc_ref.Update({
        {"lastAction", FieldValue::Integer(Actions::ACTION_CALL_SET)},
        {"setCalled", FieldValue::String(setCalled)},
    });
    
    cout << "You called the " << setCalled <<  " set" << endl; 

    string turnID = snapshot.Get("turn").string_value(); 
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
    int index = Player::GetInstance().GetPlayerIndex(snapshot, turnID);
    MapFieldValue map = playerList[index].map_value();;
    vector<FieldValue> hand = map["hand"].array_value();
    vector<string> handString;
    vector<FieldValue> newHand; 

    for(int i=0; i < hand.size(); i++)
    {
        handString.push_back(hand[i].string_value()); 
    }

    if (set == "low") 
    {
        for(int i = 2; i <= 8; i++)
        {
            string num = std::to_string(i); 
            string removeCard = card + num; 
            // cout << removeCard << endl; 
            handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());
            removeCard = card; 
            // cout << removeCard << endl; 
        } 

    } 
    else if (set == "high")
    {
        removeCard = card + "9";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());

        removeCard = card + "\u2469";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());

        removeCard = card + "J";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());

        removeCard = card + "Q";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());

        removeCard = card + "K";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());

        removeCard = card + "A";
        handString.erase(remove(handString.begin(), handString.end(), removeCard), handString.end());
    }

    for(int i = 0; i < handString.size(); i++)
        {
            // cout << FieldValue::String(handString[i]) << endl; 
            newHand.push_back(FieldValue::String(handString[i]));
        }


    map["hand"] = FieldValue::Array(newHand);
    playerList[index] = FieldValue::Map(map); 

     doc_ref.Update({
        {"players", FieldValue::Array(playerList)},
    }); 
    Hand::GetInstance().PrettyPrint(handString);
    PlayTurn(snapshot); 
    cout << "It's your turn to play again!" << endl;
}

void PlayingMyTurn::DeclareASet(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::DeclareASet" << endl;

    int lastAction = snapshot.Get("lastAction").integer_value();

    if(lastAction == Actions::ACTION_DECLARE) {

        int choice; 
        cout << "What do you want to do? Choose an option (1 or 2)" << endl;
        cout << "1. Ask for another card" << endl;
        cout << "2. Declare a set" << endl;
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                AskForACard(snapshot, true);
                break; 
            }
            case 2:
            {
                MakeASet(snapshot); 
                break;
            }
        }
    } else {
        AskForACard(snapshot, true);
    }
}