#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"
#include "utils.h"  // NOLINT
#include "Hooks.hpp"
#include "LogIt.hpp"
#include "Player.hpp"
#include "PlayerSettings.hpp"
#include "Game.hpp"

#include <vector>
#include <iostream>
using namespace std;

#include "firebase/app.h"
#include "firebase/firestore.h"
using ::firebase::App;
using ::firebase::firestore::Firestore;
using ::firebase::firestore::DocumentReference;
using ::firebase::Future;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::Error;
using ::firebase::FutureHandleId;
using ::firebase::firestore::Query; 
using ::firebase::firestore::QuerySnapshot; 
using ::firebase::firestore::DocumentSnapshot; 

bool Actions::requestReturned = false;
bool Actions::gameCreated = false;
bool Actions::docExists = false;
bool Actions::exitGame = false;

void Actions::waitForResponse()
{
  requestReturned = false;
  while(!requestReturned) {
    ProcessEvents(100);
  }
} 

void Actions::waitForGameUpdates()
{
  Hooks::setGameUpdated(false);
  while(Hooks::isGameUpdated()) {
    ProcessEvents(100);
  }
} 

void Actions::waitForGameExit()
{
  exitGame = false;
  while(!exitGame) {
    ProcessEvents(100);
  }
} 

DocumentReference getGameSnapShot(DocumentSnapshot& document)
{
  string gameCode = Game::GetInstance().GetGameCode();
  cout << "Game code: " << gameCode << endl;
  Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);

  // Start listening to game changes
  Hooks::listenToGameChanges(doc_ref);

  Future<DocumentSnapshot> game_ref = doc_ref.Get();

  while(game_ref.status() != firebase::kFutureStatusComplete);

  if (game_ref.error() == 0) {
    logIt(logINFO) << "Got document";
    document = *game_ref.result();
  } 

  return doc_ref;
}

void Actions::CreateGame(string gameCode, string displayName, string playerId)
{
  logIt(logINFO) << "Creating game...";
  cout << endl; 

  int numberOfPlayers; 
  cout << "Enter number of players (6 or 8): ";
  cin >> numberOfPlayers; 
  cout << endl; 

  // while(numberOfPlayers != 6 && numberOfPlayers != 8)
  // {
  //   cout << "Error. Please enter a valid number of players (6 or 8): "; 
  //   cin >> numberOfPlayers;
  //   cout << endl; 
  // }
  
  Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
  
  // Add a new document with a generated ID
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);
  Hooks::listenToGameChanges(doc_ref);
  MapFieldValue playerMap;
  playerMap["displayName"] = FieldValue::String(displayName);
  playerMap["playerId"] = FieldValue::String(playerId);
  playerMap["team"] = FieldValue::Integer(1); 
  for( const std::pair<std::string, FieldValue>& n : playerMap ) {
    logIt(logINFO) << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    break;
  }
  doc_ref.Set({
      {"numberOfPlayers", FieldValue::Integer(numberOfPlayers)}, 
      {"status", FieldValue::Integer(Actions::GS_WAITING)},
      {"players", FieldValue::Array({FieldValue::Map(playerMap)})}
  })

  .OnCompletion([gameCode, numberOfPlayers](const Future<void>& future) {
    logIt(logINFO) << "Done.";
    Game::GetInstance().SetNumberOfPlayers(numberOfPlayers);
    // PlayerSettings::GetInstance().AddNewGame(gameCode);
    cout << "SHARE THIS GAME CODE WITH YOUR PLAYERS: " << gameCode << endl;
  });
}

int getTeam(int size) {
    if(size % 2) {
      return 2;
    }
    return 1;
}

int Actions::JoinGame(string gameCode, string displayName, string playerId)
{
  logIt(logINFO) << "Joining game...";

  Actions::setDocExists(false);
  int team = 0;

  Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);

  // Start listening to game changes
  Hooks::listenToGameChanges(doc_ref);

  Future<DocumentSnapshot> game_ref = doc_ref.Get();

  while(game_ref.status() != firebase::kFutureStatusComplete);

  if (game_ref.error() == 0) {
    logIt(logINFO) << "Got document";
    const DocumentSnapshot& document = *game_ref.result();
    if (document.exists()) {
      logIt(logDEBUG1) << "DocumentSnapshot id: ";
      Actions::setDocExists(true);
      Actions::setRequestReturned(true);
      FieldValue players = document.Get("players");

      vector<FieldValue> playerList = players.array_value();

      team = getTeam(playerList.size());

      MapFieldValue playerMap;
      playerMap["displayName"] = FieldValue::String(displayName);
      playerMap["playerId"] = FieldValue::String(playerId);
      playerMap["team"] = FieldValue::Integer(team); 

      if(players.is_array()) {
        playerList.push_back(FieldValue::Map(playerMap));

        for( const std::pair<std::string, FieldValue>& n : playerMap ) {
          logIt(logINFO) << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
          break;
        }

        doc_ref.Update({
          {"players", FieldValue::Array(playerList)},
          {"changeReason", FieldValue::String("JOIN")}})
          .OnCompletion([](const Future<void>& future) {
            logIt(logINFO) << "Done.";
            logIt(logINFO) << "Updated the players array";
            Actions::setRequestReturned(true);
        }); 
      }
    } else {
      logIt(logERROR) << "no such document\n";
    }
  }
  else {
    logIt(logERROR) << "Error " << game_ref.error() << ": " << game_ref.error_message();
  }
  return team;
}

string Actions::CreatePlayer(string displayName)
{
  logIt(logINFO) << "Creating player...";
  string playerId = "";

  firebase::InitResult result;
  Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();

  // Add a new document with a generated ID
  DocumentReference doc_ref = db->Collection("players").Document();
  // Start listening to game changes
  Hooks::listenToPlayerChanges(doc_ref);

  Future<DocumentReference> player_ref = 
    db->Collection("players").Add({{"displayName", FieldValue::String(displayName)}}); 
                              
  while(player_ref.status() != firebase::kFutureStatusComplete);

  if (player_ref.error() == 0) {
    playerId = player_ref.result()->id();
    logIt(logINFO) << "Done.";
  }
  else {
    logIt(logERROR) << "Error " << player_ref.error() << ": " << player_ref.error_message();
  }

  return playerId;
}

void Actions::AddPlayerHand(vector<Card> hand, string playerId) {
  logIt(logINFO) << "AddPlayerHand called for playerId: " << playerId;
}

void Actions::DealCards(vector<Card>& cardDeck)
{
    DocumentSnapshot document;
    DocumentReference doc_ref = getGameSnapShot(document);

    FieldValue players = document.Get("players");
    vector<FieldValue> playerList = players.array_value();
    vector<Card> hand;
    for (size_t i = 0; i < cardDeck.size(); ++i) 
    { 
        hand.push_back(cardDeck[i]);
    }
    
    vector<MapFieldValue> newPlayerList;

    // for( const std::pair<std::string, FieldValue>& n : playerMap ) {
    //   logIt(logINFO) << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    //   break;
    // }

    for (size_t i = 0; i < playerList.size(); ++i) 
    {
        MapFieldValue playerMap = playerList[i].map_value();
        string playerId = playerMap["playerId"].string_value();
        if(playerId == Player::GetInstance().GetPlayerId()) {
            cout << "Found player: " << Player::GetInstance().GetPlayerId() << endl;
            // playerMap["hand"] = FieldValue::Array(hand);
        }
        // newPlayerList.push_back(FieldValue::Map(playerMap));
    }

    // doc_ref.Update({
    //   {"players", FieldValue::Array(newPlayerList)},
    //   {"changeReason", FieldValue::String("DEAL")}})
    //   .OnCompletion([](const Future<void>& future) {
    //     logIt(logINFO)  << "Updated the players array";
    //     Actions::setRequestReturned(true);
    // }); 

    // for (size_t i = 0; i < cardDeck.size(); ++i) 
    // { 
    //     cout << cardDeck[i].GetFaceValue() << endl; 
    // } 
}

void Actions::UpdatePlayers(string gameCode, vector<FieldValue>& players, string reason)
{
  logIt(logINFO) << "Updating players...";

  Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);

  doc_ref.Update({
    {"players", FieldValue::Array(players)},
    {"changeReason", FieldValue::String(reason)}})
    .OnCompletion([](const Future<void>& future) {
      logIt(logINFO) << "Done.";
      logIt(logINFO) << "Updated the players array";
  }); 
}