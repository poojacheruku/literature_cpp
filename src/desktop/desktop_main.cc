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
#include "Actions.hpp"
#include "Player.hpp"
#include "LogIt.hpp"
#include "PlayerSettings.hpp"
#include "Card.hpp"
#include "Hand.hpp"

#include <signal.h>

#include <iostream>
#include <set>

using namespace std;

// define and turn off for the rest of the test suite
loglevel_e loglevel = logINFO;

void my_handler(int s) {
    LiteratureAuth::GetInstance().SignOut();
    exit(1); 
}

void subscribe_to_sigint() {
  struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = my_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}

int main(int argc, const char* argv[]) {
  subscribe_to_sigint();

  LiteratureAuth::GetInstance().SignIn();

  Player::GetInstance().Start();

  Actions::waitForGameExit();

  // set<Card> cards;
  // cards.insert(Card(1, 10));
  // cards.insert(Card(3, 13));
  // cards.insert(Card(0, 5));
  // cards.insert(Card(2, 11));
  // cards.insert(Card(2, 1));
  // cards.insert(Card(2, 6));

  // Hand::GetInstance().Initialize(cards);
  // Hand::GetInstance().AddCard(3, 8);
  // Hand::GetInstance().PrettyPrint();
  
  return 1;
}
