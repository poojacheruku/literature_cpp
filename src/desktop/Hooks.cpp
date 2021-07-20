#include "Hooks.hpp"
#include "Player.hpp"
#include "firebase/firestore.h"
#include "utils.h"
#include "LogIt.hpp"
#include "Actions.hpp"

#include <iostream>
#include <string>

using namespace std;

using firebase::firestore::DocumentSnapshot;
using firebase::firestore::Error;
using firebase::firestore::FieldValue;

bool Hooks::gameUpdated = false;
bool Hooks::playerUpdated = false;

void Hooks::listenToGameChanges(DocumentReference doc_ref) {
    doc_ref.AddSnapshotListener([](const DocumentSnapshot& snapshot,
                                 Error error, string test) {
        if (error == Error::kErrorOk) {
            logIt(logINFO) << "Game added / updated";
            string source = snapshot.metadata().has_pending_writes() ? "Local" : "Server";
            if (snapshot.exists()) {
                logIt(logINFO) << source << " data: " << snapshot.Get("changeReason").string_value(); 
                if(source == "Server") {
                    gameUpdated = true;
                    logIt(logINFO) << "Passing the handle to Player";
                    Player::GetInstance().Handle(snapshot);
                }
                // Player::GetInstance().Handle(snapshot); 
            } else {
                logIt(logDEBUG1) << source << " data: null";
            }
        } else {
            logIt(logERROR) << "Listen failed: " << error;
        }
    });
}

void Hooks::listenToPlayerChanges(DocumentReference doc_ref) {
    doc_ref.AddSnapshotListener([](const DocumentSnapshot& snapshot,
                                 Error error, string test) {
        if (error == Error::kErrorOk) {
            logIt(logINFO) << "Player added / updated";
            string source =
                snapshot.metadata().has_pending_writes() ? "Local" : "Server";
            if (snapshot.exists()) {
                logIt(logDEBUG1) << source << " data: " << snapshot.Get("name").string_value();
                if(source == "Server") {
                    playerUpdated = true;
                }
            } else {
                logIt(logDEBUG1) << source << " data: null";
            }
        } else {
            logIt(logERROR) << "Listen failed: " << error;
        }
    });
    // [END listen_document_local]
}
