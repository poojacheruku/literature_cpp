#include "StoppedPlaying.hpp"
#include "WaitingForPlayers.hpp"
#include "WaitingForTurn.hpp"
#include "uuid.h"
#include "utils.h"
#include "Actions.hpp"
#include "auth/LiteratureAuth.hpp"
#include "LogIt.hpp"
#include "Game.hpp"

#include "firebase/app.h"
#include "firebase/firestore.h"

#include <iostream>
#include <string>
using namespace std;

using ::firebase::firestore::Firestore;
using ::firebase::Future;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::DocumentSnapshot;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::Error;

/* constructor */
StoppedPlaying::StoppedPlaying()
    : PlayerState()
{
    m_state = Player::ST_STOPPED_PLAYING;
    m_name = "Ready for a new game";
}

StoppedPlaying &StoppedPlaying::GetInstance()
{
    static StoppedPlaying instance; // Gets destroyed at the end
    return instance;
}

int getChoice() {
    cout << "What would you like to do?" << endl;
    cout << "1. Start a new game" << endl;
    cout << "2. Join a game" << endl;

    int choice;
    cout << "Choose your option (1 or 2): ";
    cin >> choice;

    return choice;
}

string getDisplayNameFromUser() {
    string displayName;
    cout << "Enter your display name: ";
    cin >> displayName;
    return displayName;
}

string getGameCodeFromUser() {
    string gameCode;
    cout << "Enter the game code to join: ";
    cin >> gameCode;
    return gameCode;
}

void StoppedPlaying::Start()
{
    cout << "StoppedPlaying::Start" << endl;
    
    logIt(logINFO) << GetName();
    int choice = 0;
    int team = 0;
    string gameCode;
    string displayName = getDisplayNameFromUser();
    Player::GetInstance().SetDisplayName(displayName);
    string playerId = Actions::CreatePlayer(displayName);
    Player::GetInstance().SetPlayerId(playerId);
    // do {
        choice = getChoice();
    // } while(choice != 1 && choice != 2);

    switch (choice)
    {
    case 1:
        logIt(logINFO) << "You chose to start a new game";
        Player::GetInstance().SetPlayerType(Player::OWNER);
        gameCode = uuid::generate_game_code();
        Actions::CreateGame(gameCode, displayName, playerId);
        Player::GetInstance().SetTeam(Game::TEAM_A);
        Game::GetInstance().AddPlayer(displayName, playerId, Game::TEAM_A);
        Player::GetInstance().AddGame(gameCode);
        Game::GetInstance().SetGameCode(gameCode);
        Player::GetInstance().SetState(&WaitingForPlayers::GetInstance()); 
        Player::GetInstance().WaitForPlayers();
        break;

    case 2:
        logIt(logINFO) << "You chose to join a game";
        Player::GetInstance().SetPlayerType(Player::PLAYER);
        gameCode = getGameCodeFromUser();
        team = Actions::JoinGame(gameCode, displayName, playerId);
        Player::GetInstance().SetTeam(team);
        Game::GetInstance().AddPlayer(displayName, playerId, team);
        Player::GetInstance().AddGame(gameCode);
        Game::GetInstance().SetGameCode(gameCode);
        Player::GetInstance().SetState(&WaitingForTurn::GetInstance()); 
        break; 
   
    default:
        logIt(logERROR) << "Not sure how I came here";
    }

}

void StoppedPlaying::Handle(const DocumentSnapshot& snapshot)
{
    logIt(logINFO) << "Should not be here. Report issue!"; 
}
