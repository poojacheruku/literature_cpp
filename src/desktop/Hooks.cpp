#include "Hooks.hpp"
#include "firebase/firestore.h"

#include <iostream>
#include <string>

using namespace std;

using firebase::firestore::DocumentSnapshot;
using firebase::firestore::Error;
using firebase::firestore::FieldValue;

bool Hooks::gameUpdated = false;

void Hooks::listenToGameChanges(DocumentReference doc_ref) {
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
