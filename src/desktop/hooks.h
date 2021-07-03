#ifndef HOOKS_H
#define HOOKS_H

#include "auth/LiteratureAuth.hpp"
#include <iostream>
#include <string>
#include "firebase/app.h"
#include "firebase/firestore.h"

using namespace std;

using firebase::firestore::DocumentReference;
using firebase::firestore::DocumentSnapshot;
using firebase::firestore::Error;
using firebase::firestore::FieldValue;

bool gameUpdated = false;

void listenToGameChanges(DocumentReference doc_ref) {
    doc_ref.AddSnapshotListener([](const DocumentSnapshot& snapshot,
                                 Error error, string test) {
        if (error == Error::kErrorOk) {
        string source =
            snapshot.metadata().has_pending_writes() ? "Local" : "Server";
        if (snapshot.exists()) {
            std::cout << source << " data: " << snapshot.Get("name").string_value()
                    << '\n';
            if(source == "Server") {
                gameUpdated = true;
            }
        } else {
            std::cout << source << " data: null\n";
        }
        } else {
        std::cout << "Listen failed: " << error << '\n';
        }
    });
    // [END listen_document_local]
}

#endif // HOOKS_H