#ifndef LITERATURE_AUTH_H
#define LITERATURE_AUTH_H

#include "firebase/app.h"
#include "firebase/auth.h"

using ::firebase::App;
using ::firebase::auth::Auth;

class LiteratureAuth
{
private:
    /* data */
    App* app;
    Auth* auth;

    /* constructor */
    LiteratureAuth();
    LiteratureAuth(LiteratureAuth const&);              // Don't implement
    void operator=(LiteratureAuth const&);    // Don't implement

public:
    static LiteratureAuth& getInstance();
    void signIn();
    App* getFirebaseApp();
};

#endif  // LITERATURE_AUTH_H  // NOLINT