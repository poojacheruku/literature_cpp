#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"
#include "utils.h"  // NOLINT
#include "Hooks.hpp"

using ::firebase::firestore::Firestore;
using ::firebase::Future;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::Error;

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


void Actions::createGame(string gameCode, string displayName, string playerId)
{
  cout << "Creating game..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());
  
  // Add a new document with a generated ID
  db->Collection("games")
      .Document(gameCode)
      .Set({
          {"status", FieldValue::Integer(GS_WAITING)},
          {"players", FieldValue::Array({FieldValue::String(playerId)})}
  });

  DocumentReference game_ref = db->Collection("games").Document(gameCode);
  
  Hooks::listenToGameChanges(game_ref);
}

void Actions::createPlayer(string gameCode, string displayName)
{
  cout << "Creating player..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());

  // Add a new document with a generated ID
  

  Future<DocumentReference> player_ref = 
    db->Collection("players").Add({{"displayName", FieldValue::String(displayName)}}); 
                              

  player_ref.OnCompletion([gameCode, displayName](const Future<DocumentReference>& future) {
    if (future.error() == Error::kErrorOk) {
      std::cout << "DocumentSnapshot added with ID: " << future.result()->id()
                << '\n';
      // gameCreated = true;
      string playerId = future.result()->id();
      createGame(gameCode, displayName, playerId);
    } else {
      std::cout << "Error adding document: " << future.error_message() << '\n';
    }
    requestReturned = true;
  });
}
