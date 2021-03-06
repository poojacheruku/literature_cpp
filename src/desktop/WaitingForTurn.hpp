#ifndef LITERATURE_WAITING_FOR_TURN_H
#define LITERATURE_WAITING_FOR_TURN_H

#include "PlayerState.hpp"
#include "Player.hpp"

using ::firebase::firestore::MapFieldValue;
using ::firebase::firestore::FieldValue;

class WaitingForTurn : public PlayerState
{
private:
    /* data */

    /* constructor */
    WaitingForTurn();
    WaitingForTurn(WaitingForTurn const&);              // Don't implement
    void operator=(WaitingForTurn const&);    // Don't implement
    void HandleRequestAction(const DocumentSnapshot& snapshot);
    void HandleRequestCallSet(const DocumentSnapshot& snapshot);
    void HandleRequest(const DocumentSnapshot& snapshot, MapFieldValue& requestMap);
    void ForfeitSuit(vector<FieldValue>& playerList, vector<FieldValue>& newPlayerList, string setCalled);

public:
    static WaitingForTurn& GetInstance();
	void Handle(const DocumentSnapshot& snapshot);
};

#endif  // LITERATURE_WAITING_FOR_TURN_H  // NOLINT