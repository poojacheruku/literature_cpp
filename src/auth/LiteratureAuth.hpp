#ifndef LITERATURE_CPP_AUTH_H
#define LITERATURE_CPP_AUTH_H

#include "firebase/app.h"
#include "firebase/auth.h"

class LiteratureAuth
{
private:
    /* data */
    firebase::auth::Auth* auth;

    /* constructor */
    LiteratureAuth();
    LiteratureAuth(LiteratureAuth const&);              // Don't implement
    void operator=(LiteratureAuth const&);    // Don't implement

public:
    static LiteratureAuth& getInstance();
    void signIn();
};

#endif  // LITERATURE_CPP_AUTH_H_  // NOLINT