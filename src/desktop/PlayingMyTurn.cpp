#include "PlayingMyTurn.hpp"
#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "Game.hpp"
#include "Card.hpp"
#include "Actions.hpp"

#include "auth/LiteratureAuth.hpp"

#include <iostream>
using namespace std;

using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::Firestore;

/* constructor */
PlayingMyTurn::PlayingMyTurn()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
    m_name = "It is your turn";
}

PlayingMyTurn& PlayingMyTurn::GetInstance()
{
    static PlayingMyTurn instance;       // Gets destroyed at the end
    return instance;
}

void PlayingMyTurn::Handle(const DocumentSnapshot& snapshot)
{
    cout << "PlayingMyTurn::Handle" << endl;

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
    cout << "WaitingForTurn::PlayTurn" << endl;
    
    int choice; 
    cout << "What do you want to do? Choose an option (1 or 2)" << endl;
    cout << "1. Ask for a card" << endl;
    cout << "2. Make a set" << endl;
    cin >> choice;

    switch (choice)
    {
        case 1:
        {
            AskForACard(snapshot);
            break; 
        }
        case 2:
        {
            break;
        }
    }
}

void PlayingMyTurn::HandleRequestAction(const DocumentSnapshot& snapshot)
{
    cout << "WaitingForTurn::HandleRequestAction" << endl;
    
    MapFieldValue requestMap = snapshot.Get("request").map_value();
    string fromId = requestMap["fromId"].string_value();
    string toId = requestMap["toId"].string_value();
    int requestStatus = requestMap["status"].integer_value();
    string card = requestMap["card"].string_value();
    string otherPlayer = Player::GetInstance().GetPlayerName(snapshot, toId);

    if(requestStatus == Actions::ACTION_STATUS_ACCEPTED) {
        cout << otherPlayer << " transfered the card " << card << " to you" << endl;
        cout << "It's your turn" << endl;
        PlayTurn(snapshot);
    }
    else if(requestStatus == Actions::ACTION_STATUS_REJECTED) {
        cout << otherPlayer << " does not have the card " << card << endl;
        cout << "It's " << otherPlayer << "'s turn" << endl;
        Player::GetInstance().PrintHand(snapshot);
        Player::GetInstance().SetState(&WaitingForTurn::GetInstance());
    }
}

void PlayingMyTurn::AskForACard(const DocumentSnapshot& snapshot)
{
    cout << "WaitingForTurn::AskForACard" << endl;
    
    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    cout << "Whom do you want to ask?" << endl;  

    for(int i = 0; i < playerList.size(); i++)
    {
        MapFieldValue playerMap = playerList[i].map_value();
        string displayName = playerMap["displayName"].string_value();
        int team = playerMap["team"].integer_value();

        if(Player::GetInstance().GetTeam() != team) {
            cout << (i+1) << ". " << displayName << endl; 
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
        {"lastAction", FieldValue::Integer(Actions::ACTION_REQUEST)},
        {"request", FieldValue::Map(requestMap)}
    });
}