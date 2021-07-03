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
#include "hooks.h"
#include "actions.h"
#include "uuid.h"

#include <iostream>

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
    string gameCode = uuid::generate_game_code();
    cout << "Your game code is: " << gameCode; 
    code.pop_back();
    cout << endl; 
    createPlayer(gameCode, displayName);
    waitForResponse();
  }else if (choice == 2)
  {
    string code; 
    cout << "Enter the game code: "; 
    cin >> code;

    cout << "Code: " << code << endl;
    
    Firestore* db = Firestore::GetInstance(LiteratureAuth::getInstance().getFirebaseApp());
    requestReturned = false;
    DocumentReference doc_ref = db->Collection("games").Document(code);
    doc_ref.Get().OnCompletion([](const Future<DocumentSnapshot>& future) {
      if (future.error() == Error::kErrorOk) {
        const DocumentSnapshot& document = *future.result();
        if (document.exists()) {
          std::cout << "DocumentSnapshot id: " << document.id() << '\n';
          docExists = true;
          requestReturned = true;
        } else {
          std::cout << "no such document\n";
        }
      } else {
        std::cout << "Get failed with: " << future.error_message() << '\n';
      }
    });
    waitForResponse();
    if(docExists) {
      requestReturned = false;
      doc_ref.Update({{"capital", FieldValue::Boolean(true)}})
        .OnCompletion([](const Future<void>& future) {
          cout << "Added the field capital" << endl;
          requestReturned = true;
      });
      waitForResponse();
    }
  }

  waitForGameUpdates();

  return 1;
}
