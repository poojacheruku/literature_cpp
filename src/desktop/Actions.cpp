#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"
#include "utils.h"  // NOLINT
#include "Hooks.hpp"

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


void createGame(string displayName, string gameCode, string playerId)
{
  cout << "Creating game..." << endl;

  // firebase::InitResult result;
  Firestore* db = LiteratureAuth::getInstance().getFirestoreDb();
  
  // Add a new document with a generated ID
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);
  Hooks::listenToGameChanges(doc_ref);
  doc_ref
      .Set({
          {"status", FieldValue::Integer(Actions::GS_WAITING)},
          {"players", FieldValue::Array({FieldValue::String(playerId)})}
  });
}

void joinGame(string displayName, string gameCode, string playerId) {
  cout << "Joining game..." << endl;

  Actions::setDocExists(false);

  Firestore* db = LiteratureAuth::getInstance().getFirestoreDb();
  DocumentReference doc_ref = db->Collection("games").Document(gameCode);

  // Start listening to game changes
  Hooks::listenToGameChanges(doc_ref);

  Future<DocumentSnapshot> game_ref = doc_ref.Get();

  while(game_ref.status() != firebase::kFutureStatusComplete);

  if (game_ref.error() == 0) {
    LogMessage("Got document");
    const DocumentSnapshot& document = *game_ref.result();
    if (document.exists()) {
      std::cout << "DocumentSnapshot id: " << document.id() << '\n';
      Actions::setDocExists(true);
      Actions::setRequestReturned(true);
      FieldValue players = document.Get("players");
      if(players.is_array()) {
        vector<FieldValue> playerList = players.array_value();
        playerList.push_back(FieldValue::String(playerId));
        for (auto & element : playerList) {
            cout << element.ToString() << endl;;
        }
        doc_ref.Update({{"players", FieldValue::Array(playerList)}})
          .OnCompletion([](const Future<void>& future) {
            cout << "Updated the players array" << endl;
            Actions::setRequestReturned(true);
        });        
      }
    } else {
      std::cout << "no such document\n";
  }
  }
  else {
    LogMessage("Error %d: %s", game_ref.error(), game_ref.error_message());
  }

  // doc_ref.Get().OnCompletion([playerId](const Future<DocumentSnapshot>& future) {
  //   if (future.error() == Error::kErrorOk) {
  //     const DocumentSnapshot& document = *future.result();
  //     if (document.exists()) {
  //       std::cout << "DocumentSnapshot id: " << document.id() << '\n';
  //       Actions::setDocExists(true);
  //       Actions::setRequestReturned(true);
  //       FieldValue players = document.Get("players");
  //       if(players.is_array()) {
  //         vector<FieldValue> playerList = players.array_value();
  //         playerList.push_back(FieldValue::String(playerId));
  //         for (auto & element : playerList) {
  //             cout << element.ToString() << endl;;
  //         }
  //       }
  //     } else {
  //       std::cout << "no such document\n";
  //     }
  //   } else {
  //     std::cout << "Get failed with: " << future.error_message() << '\n';
  //   }
  // });

  if(Actions::isDocExists()) {
    // doc_ref.Update({{"players", FieldValue::Array({FieldValue::String(playerId)})}})
    //   .OnCompletion([](const Future<void>& future) {
    //     cout << "Updated the players array" << endl;
    //     Actions::setRequestReturned(true);
    // });
    Actions::waitForResponse();
  }
}

void Actions::createPlayer(string displayName, string gameCode, bool newGame)
{
  cout << "Creating player..." << endl;

  firebase::InitResult result;
  Firestore* db = LiteratureAuth::getInstance().getFirestoreDb();

  // Add a new document with a generated ID
  DocumentReference doc_ref = db->Collection("players").Document();
  // Start listening to game changes
  Hooks::listenToPlayerChanges(doc_ref);

  Future<DocumentReference> player_ref = 
    db->Collection("players").Add({{"displayName", FieldValue::String(displayName)}}); 
                              
  while(player_ref.status() != firebase::kFutureStatusComplete);

  if (player_ref.error() == 0) {
    LogMessage("Created player");
    string playerId = player_ref.result()->id();
    if(newGame) {
      createGame(displayName, gameCode, playerId);
    } else {
      joinGame(displayName, gameCode, playerId);
    }
  }
  else {
    LogMessage("Error %d: %s", player_ref.error(), player_ref.error_message());
  }
}
