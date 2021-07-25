#include "PlayingMyTurn.hpp"
#include "LogIt.hpp"
#include "Game.hpp"
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

        int playerNumber = choice -1; 
        MapFieldValue playerMap = playerList[playerNumber].map_value();
        string askName = playerMap["displayName"].string_value(); 
        string askPlayerId = playerMap["playerId"].string_value();
        string gameCode = Game::GetInstance().GetGameCode();

        Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
        DocumentReference doc_ref = db->Collection("games").Document(gameCode);
        doc_ref.Update({
            {"playerBeingAsked", FieldValue::String(askPlayerId)},
            {"changeReason", FieldValue::String("ASK")}
        });

        cout << "You are asking " << askName << " for a card..." << endl; 
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
