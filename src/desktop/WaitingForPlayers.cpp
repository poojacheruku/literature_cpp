#include "WaitingForPlayers.hpp"
#include "LogIt.hpp"
#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"
#include "Game.hpp"

#include "firebase/firestore.h"

using ::firebase::firestore::Firestore;
using ::firebase::firestore::DocumentReference;
using ::firebase::Future;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::DocumentSnapshot; 

#include <iostream>
#include <future>
#include <thread>
using namespace std;

/* constructor */
WaitingForPlayers::WaitingForPlayers()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForPlayers& WaitingForPlayers::GetInstance()
{
    static WaitingForPlayers instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForPlayers::WaitForPlayers()
{
    cout << "Waiting for other players!" << endl;
}

void WaitingForPlayers::Handle(const DocumentSnapshot& snapshot)
{
    string changeReason = snapshot.Get("changeReason").string_value(); 
    int numberOfPlayers = snapshot.Get("numberOfPlayers").integer_value(); 
 
    if(changeReason == "JOIN")
    {

        vector<FieldValue> playerList = snapshot.Get("players").array_value();

        // string playerID = playerList.back().string_value();
        MapFieldValue playerMap = playerList.back().map_value();
        string displayName = playerMap["displayName"].string_value();
        string playerId = playerMap["playerId"].string_value();
        int team = playerMap["team"].integer_value();
        cout << displayName << " joined the game in team " << team << endl;
        cout << endl;
        Game::GetInstance().AddPlayer(displayName, playerId, team);

        if (playerList.size() == numberOfPlayers && Player::GetInstance().GetPlayerType() == Player::OWNER)
        {
            int choice; 
            cout << "Do you want to start the game? Please select an option (1 or 2): " << endl;
            cout << "1. Start the game" << endl;
            cout << "2. End the game" << endl; 
            cin >> choice;

            if(choice == 1)
            {
                Game::GetInstance().CreateAndShuffleDeck();
                Game::GetInstance().DealCards();
                Game::GetInstance().PrintGameInfo();
            }
        
        }        
    }
}
