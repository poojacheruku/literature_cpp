#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"
#include "utils.h"  // NOLINT
#include "Hooks.hpp"

using ::firebase::firestore::Firestore;
using ::firebase::Future;
using ::firebase::firestore::DocumentReference;
using ::firebase::firestore::DocumentSnapshot;
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


void createGame(string displayName, string gameCode, string playerId)
{
  cout << "Creating game..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());
  
  // Add a new document with a generated ID
  db->Collection("games")
      .Document(gameCode)
      .Set({
          {"status", FieldValue::Integer(Actions::GS_WAITING)},
          {"players", FieldValue::Array({FieldValue::String(playerId)})}
  });

  DocumentReference game_ref = db->Collection("games").Document(gameCode);
  
  Hooks::listenToGameChanges(game_ref);
}

void joinGame(string displayName, string gameCode, string playerId) {
    Actions::setDocExists(false);

    Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());
    DocumentReference doc_ref = db->Collection("games").Document(gameCode);
    doc_ref.Get().OnCompletion([](const Future<DocumentSnapshot>& future) {
      if (future.error() == Error::kErrorOk) {
        const DocumentSnapshot& document = *future.result();
        if (document.exists()) {
          std::cout << "DocumentSnapshot id: " << document.id() << '\n';
          Actions::setDocExists(true);
          Actions::setRequestReturned(true);
        } else {
          std::cout << "no such document\n";
        }
      } else {
        std::cout << "Get failed with: " << future.error_message() << '\n';
      }
    });

    Actions::waitForResponse();

    if(Actions::isDocExists()) {
      doc_ref.Update({{"capital", FieldValue::Boolean(true)}})
        .OnCompletion([](const Future<void>& future) {
          cout << "Added the field capital" << endl;
          Actions::setRequestReturned(true);
      });
      Actions::waitForResponse();
    }
}

void Actions::createPlayer(string gameCode, string displayName, bool newGame)
{
  cout << "Creating player..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());

  // Add a new document with a generated ID
  

  Future<DocumentReference> player_ref = 
    db->Collection("players").Add({{"displayName", FieldValue::String(displayName)}}); 
                              

  player_ref.OnCompletion([gameCode, displayName, newGame](const Future<DocumentReference>& future) {
    if (future.error() == Error::kErrorOk) {
      string playerId = future.result()->id();

      if(newGame) {
        createGame(gameCode, displayName, playerId);
      } else {
        joinGame(gameCode, displayName, playerId);
      }
    } else {
      std::cout << "Error adding document: " << future.error_message() << '\n';
    }
    requestReturned = true;
  });
}
