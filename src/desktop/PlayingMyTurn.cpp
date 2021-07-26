#include "PlayingMyTurn.hpp"
#include "WaitingForTurn.hpp"
#include "LogIt.hpp"
#include "Game.hpp"
#include "auth/LiteratureAuth.hpp"
#include "Card.hpp"
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
    logIt(logINFO) << GetName();
}

void PlayingMyTurn::PlayTurn(const DocumentSnapshot& snapshot)
{
    int choice; 
    cout << "What do you want to do? Choose an option (1 or 2)" << endl;
    cout << "1. Ask for a card" << endl;
    cout << "2. Make a set" << endl;
    cin >> choice;
    vector<FieldValue> playerList = snapshot.Get("players").array_value();
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
            int number = i + 1; 
            cout << number << ". " << displayName << endl; 
        }
        int choice; 
        cout << "Enter a number: ";
        cin >> choice; 

        string input;  
        cout << "What card do you want to ask for?" << endl; 
        cout << "Please enter in this format:" << endl; 
        cout << "H-K (king of hearts)" << endl; 
        cout << "D-1 (one of diamonds)" << endl; 
        cin >> input; 

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

        int playerNumber = choice -1; 
        MapFieldValue playerMap = playerList[playerNumber].map_value();
        string askName = playerMap["displayName"].string_value(); 
        string askPlayerId = playerMap["playerId"].string_value();
        string gameCode = Game::GetInstance().GetGameCode();

        cout << "You are asking " << askName << " for " << card << endl; 

        Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
        DocumentReference doc_ref = db->Collection("games").Document(gameCode);
        doc_ref.Update({
            {"playerBeingAsked", FieldValue::String(askPlayerId)},
            {"card", FieldValue::String(card)},
            {"changeReason", FieldValue::String("ASK")},
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

    cout << "got here" << endl; 
    string changeReason = snapshot.Get("changeReason").string_value();

    if(changeReason == "NOCARD")
    {
        vector<FieldValue> playerList = snapshot.Get("players").array_value();
        string card = snapshot.Get("card").string_value();
        for(int i = 0; i < playerList.size(); i++)
        {
            cout << "got here too" << endl; 
            MapFieldValue playerMap = playerList[i].map_value();
            if(playerMap["playerId"].string_value() == snapshot.Get("turn").string_value())
            {
                    cout << playerMap["displayName"].string_value() << " does not have " << card << endl; 
                    cout << endl; 
                    cout << "It's " << playerMap["displayName"].string_value() << "'s turn to play!" << endl; 
            }
        }
        Player::GetInstance().SetState(&WaitingForTurn::GetInstance());
    }
}
