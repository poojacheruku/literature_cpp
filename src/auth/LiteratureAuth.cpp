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

void LiteratureAuth::signIn()
{
    Future<User*> anon_sign_in_for_user = m_auth->SignInAnonymously();
    WaitForSignInFuture(anon_sign_in_for_user,
                        "Auth::SignInAnonymously() for User", kAuthErrorNone,
                        m_auth);
    if (anon_sign_in_for_user.status() == ::firebase::kFutureStatusComplete) {
      User* anonymous_user = anon_sign_in_for_user.result()
                                 ? *anon_sign_in_for_user.result()
                                 : nullptr;
    }
}
