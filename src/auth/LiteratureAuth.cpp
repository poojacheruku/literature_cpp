#include <iostream>
#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/firestore.h"

using namespace std;
using ::firebase::App;
using ::firebase::Future;
using ::firebase::auth::Auth;
using ::firebase::firestore::Firestore;
using ::firebase::auth::User;
using ::firebase::auth::kAuthErrorNone;

#include "LiteratureAuth.hpp"
#include "utils.h"
#include "auth_utils.h"

static const char kFirebaseProviderId[] =
#if defined(__ANDROID__)
    "firebase";
#else   // !defined(__ANDROID__)
    "Firebase";
#endif  // !defined(__ANDROID__)

/* constructor */
LiteratureAuth::LiteratureAuth()
{
    Initialize();
}

LiteratureAuth& LiteratureAuth::GetInstance()
{
    static LiteratureAuth instance;       // Gets destroyed at the end
    return instance;
}

void LiteratureAuth::Initialize()
{
    firebase::AppOptions options;
    options.set_database_url("literature-316716.firebaseapp.com");
    m_app = firebase::App::Create(options);
    m_auth = firebase::auth::Auth::GetAuth(m_app);
    m_db = Firestore::GetInstance(m_app);
}

bool LiteratureAuth::SignIn()
{
    // Grab the result of the latest sign-in attempt.
    Future<User *> future =
        m_auth->SignInAnonymouslyLastResult();

    // If we're in a state where we can try to sign in, do so.
    if (future.status() == firebase::kFutureStatusInvalid ||
        (future.status() == firebase::kFutureStatusComplete &&
        future.error() != firebase::auth::kAuthErrorNone)) {
        future = m_auth->SignInAnonymously();
    }

    while(future.status() == firebase::kFutureStatusPending) {}

    // We're signed in if the most recent result was successful.
    return (future.status() == firebase::kFutureStatusComplete &&
        future.error() == firebase::auth::kAuthErrorNone);
}

void LiteratureAuth::SignOut()
{
    Future<void> future = m_auth->current_user()->Delete();
    while(future.status() == firebase::kFutureStatusPending) {}
    m_auth->SignOut();
}