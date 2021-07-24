#include "PlayingMyTurn.hpp"
#include "LogIt.hpp"
#include "Game.hpp"
#include <iostream>
using namespace std;

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
        cout << "Who do you want to ask?" << endl;  

        for(int i = 0; i < playerList.size(); i++)
        {
            MapFieldValue playerMap = playerList[i].map_value();
            string displayName = playerMap["displayName"].string_value();
            int number = i + 1; 
            cout << number << ". " << displayName << endl; 
        }
        
        // for(int i = 1; i < playerNames.size(); i++)
        // {
        //     cout << i << ". " << playerNames[i] << endl;
        // }
        break; 
    
    case 2:

        break;
    }
}
