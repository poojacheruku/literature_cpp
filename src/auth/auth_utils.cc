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

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include "firebase/app.h"
#include "firebase/auth.h"

#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif  // _WIN32

#ifdef _WIN32
#include <windows.h>
#endif  // _WIN32

#include <algorithm>
#include <string>

using ::firebase::Future;
using ::firebase::FutureBase;
using ::firebase::auth::Auth;
using ::firebase::auth::AuthError;
using ::firebase::auth::User;
using ::firebase::auth::kAuthErrorNone;

#include "utils.h"  // NOLINT

// Don't return until `future` is complete.
// Print a message for whether the result mathes our expectations.
// Returns true if the application should exit.
bool WaitForFuture(const FutureBase& future, const char* fn,
                          AuthError expected_error, bool log_error = true) {
  // Note if the future has not be started properly.
  if (future.status() == ::firebase::kFutureStatusInvalid) {
    LogMessage("ERROR: Future for %s is invalid", fn);
    return false;
  }

  // Wait for future to complete.
  LogMessage("  Calling %s...", fn);
  while (future.status() == ::firebase::kFutureStatusPending) {
    if (ProcessEvents(100)) return true;
  }

  // Log error result.
  if (log_error) {
    const AuthError error = static_cast<AuthError>(future.error());
    if (error == expected_error) {
      const char* error_message = future.error_message();
      if (error_message) {
        LogMessage("%s completed as expected", fn);
      } else {
        LogMessage("%s completed as expected, error: %d '%s'", fn, error,
                   error_message);
      }
    } else {
      LogMessage("ERROR: %s completed with error: %d, `%s`", fn, error,
                 future.error_message());
    }
  }
  return false;
}

bool WaitForSignInFuture(Future<User*> sign_in_future, const char* fn,
                                AuthError expected_error, Auth* auth) {
  if (WaitForFuture(sign_in_future, fn, expected_error)) return true;

  const User* const* sign_in_user_ptr = sign_in_future.result();
  const User* sign_in_user =
      sign_in_user_ptr == nullptr ? nullptr : *sign_in_user_ptr;
  const User* auth_user = auth->current_user();

  if (expected_error == ::firebase::auth::kAuthErrorNone &&
      sign_in_user != auth_user) {
    LogMessage("ERROR: future's user (%x) and current_user (%x) don't match",
               static_cast<int>(reinterpret_cast<intptr_t>(sign_in_user)),
               static_cast<int>(reinterpret_cast<intptr_t>(auth_user)));
  }

  return false;
}
