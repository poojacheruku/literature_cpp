#ifndef LITERATURE_WAITING_FOR_TURN_H
#define LITERATURE_WAITING_FOR_TURN_H

#include "PlayerState.hpp"
#include "Player.hpp"

using ::firebase::firestore::MapFieldValue;

class WaitingForTurn : public PlayerState
{
private:
    /* data */

    /* constructor */
    WaitingForTurn();
    WaitingForTurn(WaitingForTurn const&);              // Don't implement
    void operator=(WaitingForTurn const&);    // Don't implement
    void HandleRequestAction(const DocumentSnapshot& snapshot);
    void HandleRequest(const DocumentSnapshot& snapshot, MapFieldValue& requestMap);

public:
    static WaitingForTurn& GetInstance();
	void Handle(const DocumentSnapshot& snapshot);
    void PlayTurn(const DocumentSnapshot& snapshot);
};

#endif  // LITERATURE_WAITING_FOR_TURN_H  // NOLINT