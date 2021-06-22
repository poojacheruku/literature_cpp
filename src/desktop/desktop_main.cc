// Copyright 2016 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "auth/LiteratureAuth.hpp"
#include "utils.h"  // NOLINT
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

using namespace std;

bool createGameReturned = false;
bool gameCreated = false;
string playerId;
int gameCode;
string displayName;

bool waitForGameCreation()
{
  while(!createGameReturned) {
    ProcessEvents(100);
  }
} 

void createGame(int gameCode, string displayName, string playerId)
{
  cout << "Creating game..." << endl;

  firebase::InitResult result;
  Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());

  // Add a new document with a generated ID
  Future<DocumentReference> game_ref =
    db->Collection("games").Add({
            {"code", FieldValue::Integer(gameCode)},
            {"status", FieldValue::String("Waiting for game be started...")},
            {"playerId", FieldValue::String(playerId)}
    });
  
  game_ref.OnCompletion([](const Future<DocumentReference>& future) {
    if (future.error() == Error::kErrorOk) {
      std::cout << "DocumentSnapshot added with ID: " << future.result()->id()
                << '\n';
      gameCreated = true;
    } else {
      std::cout << "Error adding document: " << future.error_message() << '\n';
    }
  });
}

void createPlayer(int code, string name)
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
    createGameReturned = true;
  });
}


int main(int argc, const char* argv[]) {
  LiteratureAuth::getInstance();
  cout << "1. Start a new game" << endl;
  cout << "2. Join a game" << endl ; 
  cout << endl; 

  int choice; 
  cout << "Choose your option (1 or 2): "; 
  cin >> choice; 
  cout << endl; 

  while (!(choice == 1) && !(choice == 2))
  {
    cout << "Please enter a valid option: "; 
    cin >> choice; 
  }
  string displayName; 
  cout << "Enter your name: "; 
  cin >> displayName;
  cout << endl; 

  if (choice == 1)
  {
    std::random_device rd;
    std::default_random_engine rng(rd()); 
    
    std::vector<int> code(99999);
    std::iota(code.begin(), code.end(), 00000);

    shuffle(code.begin(), code.end(), rng);
    int gameCode = code.back(); 
    cout << "Your game code is: " << gameCode; 
    code.pop_back();
    cout << endl; 
    createPlayer(gameCode, displayName);
    waitForGameCreation();
  }else if (choice == 2)
  {
    int code; 
    cout << "Enter a 5 digit game code: "; 
    cin >> code; 

  }

  return 1;
}
