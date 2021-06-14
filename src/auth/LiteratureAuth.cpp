#include <iostream>
#include "firebase/app.h"
#include "firebase/auth.h"

using namespace std;
using ::firebase::Future;
using ::firebase::auth::Auth;
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
    firebase::App* app = firebase::App::Create(firebase::AppOptions());
    auth = firebase::auth::Auth::GetAuth(app);
}

LiteratureAuth& LiteratureAuth::getInstance()
{
    static LiteratureAuth instance;       // Gets destroyed at the end
    cout << "Returning auth instance" << endl;
    return instance;
}

void LiteratureAuth::signIn()
{
    Future<User*> anon_sign_in_for_user = auth->SignInAnonymously();
    WaitForSignInFuture(anon_sign_in_for_user,
                        "Auth::SignInAnonymously() for User", kAuthErrorNone,
                        auth);
    if (anon_sign_in_for_user.status() == ::firebase::kFutureStatusComplete) {
      User* anonymous_user = anon_sign_in_for_user.result()
                                 ? *anon_sign_in_for_user.result()
                                 : nullptr;
      if (anonymous_user != nullptr) {
        LogMessage("Anonymous uid is %s", anonymous_user->uid().c_str());
        ExpectStringsEqual("Anonymous user email", "",
                           anonymous_user->email().c_str());
        ExpectStringsEqual("Anonymous user display_name", "",
                           anonymous_user->display_name().c_str());
        ExpectStringsEqual("Anonymous user photo_url", "",
                           anonymous_user->photo_url().c_str());
        ExpectStringsEqual("Anonymous user provider_id", kFirebaseProviderId,
                           anonymous_user->provider_id().c_str());
        ExpectTrue("Anonymous user is_anonymous()",
                   anonymous_user->is_anonymous());
        ExpectFalse("Anonymous user is_email_verified()",
                    anonymous_user->is_email_verified());
        ExpectTrue("Anonymous user metadata().last_sign_in_timestamp != 0",
                   anonymous_user->metadata().last_sign_in_timestamp != 0);
        ExpectTrue("Anonymous user metadata().creation_timestamp != 0",
                   anonymous_user->metadata().creation_timestamp != 0);
      }
    }
    cout << "\u2660" << "10" << endl;
    cout << "\u2665" << "J" << endl;
    cout << "\u2663" << "4" << endl;
    cout << "\u2666" << "8" << endl;
}
