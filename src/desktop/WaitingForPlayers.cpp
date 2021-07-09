#include "WaitingForPlayers.hpp"
#include "LogIt.hpp"
#include "auth/LiteratureAuth.hpp"
#include "Actions.hpp"

#include "firebase/firestore.h"

using ::firebase::firestore::Firestore;
using ::firebase::firestore::DocumentReference;
using ::firebase::Future;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::DocumentSnapshot; 

#include <iostream>
#include <future>
#include <thread>
using namespace std;

/* constructor */
WaitingForPlayers::WaitingForPlayers()
{
	m_state = Player::ST_WAITING_FOR_PLAYERS;
}

WaitingForPlayers& WaitingForPlayers::GetInstance()
{
    static WaitingForPlayers instance;       // Gets destroyed at the end
    return instance;
}

void WaitingForPlayers::WaitForPlayers()
{
    cout << "Waiting for other players!" << endl;
}


void joinNotification(string playerID)
{
    Firestore* db = LiteratureAuth::GetInstance().getFirestoreDb();
    DocumentReference doc_ref = db->Collection("players").Document(playerID);

    Future<DocumentSnapshot> game_ref = doc_ref.Get();

    log(logINFO) << "Waiting for status complete";
    while(game_ref.status() != firebase::kFutureStatusComplete);
    log(logINFO) << "Status complete";

    const DocumentSnapshot& document = *game_ref.result();

    cout << document.Get("displayName").string_value() << " joined the game" << endl;
}


void WaitingForPlayers::Handle(const DocumentSnapshot& snapshot)
{
    string changeReason = snapshot.Get("changeReason").string_value(); 

    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    string playerID = playerList.back().string_value(); 
 
    if(changeReason == "JOIN")
    {
        // std::thread threadObj(joinNotification, playerID);
        // threadObj.detach();
        joinNotification(playerID);
    }

}
