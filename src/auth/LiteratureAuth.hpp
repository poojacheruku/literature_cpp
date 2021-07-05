#ifndef LITERATURE_AUTH_H
#define LITERATURE_AUTH_H

#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/firestore.h"

using ::firebase::App;
using ::firebase::auth::Auth;
using ::firebase::firestore::Firestore;

class LiteratureAuth
{
private:
    /* data */
    App* app;
    Auth* auth;
    Firestore* db;

    /* constructor */
    LiteratureAuth();
    LiteratureAuth(LiteratureAuth const&);              // Don't implement
    void operator=(LiteratureAuth const&);    // Don't implement

public:
    static LiteratureAuth& getInstance();
    void signIn();
    App* getFirebaseApp();
    Firestore* getFirestoreDb();
};

#endif  // LITERATURE_AUTH_H  // NOLINT