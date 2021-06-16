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
using namespace std;

int main(int argc, const char* argv[]) {
  LiteratureAuth::getInstance().signIn();
  cout << "1. Start a new game" << endl;
  cout << "2. Join a game" << endl ; 
  cout << endl; 
  
  int choice; 
  cout << "Choose your option (1 or 2): " << endl; 
  cin >> choice; 

  // if (choice == 1)
  // {

  // }
  return 1;
}
