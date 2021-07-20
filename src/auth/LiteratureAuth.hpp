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
    App* m_app;
    Auth* m_auth;
    Firestore* m_db;

    /* constructor */
    LiteratureAuth();
    LiteratureAuth(LiteratureAuth const&);              // Don't implement
    void operator=(LiteratureAuth const&);    // Don't implement

    void Initialize();

public:
    static LiteratureAuth& GetInstance();
    App* getFirebaseApp() { return m_app; }
    Firestore* getFirestoreDb() { return m_db; }
    bool SignIn();
    void SignOut();
};

#endif  // LITERATURE_AUTH_H  // NOLINT