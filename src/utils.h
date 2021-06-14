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

#ifndef LITERATURE_CPP_UTILS_H  // NOLINT
#define LITERATURE_CPP_UTILS_H  // NOLINT

#include <string>

// Cross platform logging method.
// Implemented by android/android_main.cc or ios/ios_main.mm.
extern "C" void LogMessage(const char* format, ...);

// Platform-independent method to flush pending events for the main thread.
// Returns true when an event requesting program-exit is received.
bool ProcessEvents(int msec);

void ExpectFalse(const char* test, bool value);
void ExpectTrue(const char* test, bool value);
// Log results of a string comparison for `test`.
void ExpectStringsEqual(const char* test, const char* expected,
                               const char* actual);


#endif  // LITERATURE_CPP_UTILS_H_  // NOLINT
