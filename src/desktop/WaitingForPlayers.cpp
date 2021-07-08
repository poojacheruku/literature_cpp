#include "WaitingForPlayers.hpp"
#include "LogIt.hpp"
#include "auth/LiteratureAuth.hpp"

using ::firebase::firestore::Firestore;
using ::firebase::firestore::DocumentReference;
using ::firebase::Future;
using ::firebase::firestore::FieldValue;
using ::firebase::firestore::DocumentSnapshot; 

#include <iostream>
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

    cout << "Waiting for status complete" << endl;
    while(game_ref.status() != firebase::kFutureStatusComplete);
    cout << "Status complete" << endl;

    const DocumentSnapshot& document = *game_ref.result();

    log(logINFO) << document; 

    log(logINFO) << document.Get("displayName").string_value();

}


void WaitingForPlayers::Handle(const DocumentSnapshot& snapshot)
{
    string changeReason = snapshot.Get("changeReason").string_value(); 

    vector<FieldValue> playerList = snapshot.Get("players").array_value();

    string playerID = playerList.back().string_value(); 
 
    if(changeReason == "JOIN")
    {
        cout << playerID << " joined the game." << endl; 
    }

    joinNotification(playerID); 
    
}
