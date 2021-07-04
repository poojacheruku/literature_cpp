#include "StoppedPlaying.hpp"
#include "uuid.h"
#include "utils.h"
#include "Actions.hpp"
#include "auth/LiteratureAuth.hpp"

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

StoppedPlaying &StoppedPlaying::getInstance()
{
    static StoppedPlaying instance; // Gets destroyed at the end
    return instance;
}

int getChoice() {
    cout << "What would you like to do?" << endl;
    cout << "1. Start a new game" << endl;
    cout << "2. Join a game" << endl;
    cout << endl;

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

void startNewGame(string displayName) {
    string gameCode = uuid::generate_game_code();
    cout << "Share this game code: " << gameCode << endl;
    Actions::createPlayer(displayName, gameCode, true);
}

void joinGame(string displayName, string gameCode) {
    Actions::createPlayer(displayName, gameCode, false);
}

void StoppedPlaying::Handle()
{
    cout << GetName() << endl;
    int choice = 0;
    string gameCode;
    string displayName = getDisplayNameFromUser();
    
    do {
        choice = getChoice();
    } while(choice != 1 && choice != 2);

    switch (choice)
    {
    case 1:
        cout << "You chose to start a new game" << endl;
        Player::getInstance().SetPlayerType(Player::OWNER);
        startNewGame(displayName);
        break;

    case 2:
        cout << "You chose to join a game" << endl;
        Player::getInstance().SetPlayerType(Player::PLAYER);
        gameCode = getGameCodeFromUser();
        joinGame(displayName, gameCode);
        break; 
   
    default:
        cout << "Not sure how I came here" << endl;
    }
}
