#ifndef ACTIONS_H
#define ACTIONS_H

#include "auth/LiteratureAuth.hpp"
#include "utils.h"  // NOLINT
#include "hooks.h"

#include <iostream>
#include <vector>
#include <random>
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

using namespace std;
bool requestReturned = false;
bool gameCreated = false;
bool docExists = false;
string playerId;
string gameCode;
string displayName;

enum gameStatus {waiting, inProgress}; 

void waitForResponse()
{
  while(!requestReturned) {
    ProcessEvents(100);
  }
} 

void createGame(string gameCode, string displayName, string playerId)
{
  cout << "Creating game..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());
  
  // Add a new document with a generated ID
  db->Collection("games")
      .Document(gameCode)
      .Set({
          {"status", FieldValue::Integer(waiting)},
          {"players", FieldValue::Array({FieldValue::String(playerId)})}
  });

  DocumentReference game_ref = db->Collection("games").Document(gameCode);
  
  listenToGameChanges(game_ref);
}

void createPlayer(string code, string name)
{
  cout << "Creating player..." << endl;
  gameCode = code;
  displayName = name;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());

  // Add a new document with a generated ID
  

  Future<DocumentReference> player_ref = 
    db->Collection("players").Add({{"displayName", FieldValue::String(displayName)}}); 
                              

  player_ref.OnCompletion([](const Future<DocumentReference>& future) {
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

#endif // ACTIONS_H