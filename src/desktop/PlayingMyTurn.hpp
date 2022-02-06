#ifndef LITERATURE_PLAYING_MY_TURN_H
#define LITERATURE_PLAYING_MY_TURN_H

#include "PlayerState.hpp"
#include "Player.hpp"

class PlayingMyTurn : public PlayerState
{
private:
    /* data */

    /* constructor */
    PlayingMyTurn();
    PlayingMyTurn(PlayingMyTurn const&);              // Don't implement
    void operator=(PlayingMyTurn const&);    // Don't implement
    void HandleRequestAction(const DocumentSnapshot& snapshot);
    void AskForACard(const DocumentSnapshot& snapshot, bool ownTeam);
    void MakeASet(const DocumentSnapshot& snapshot); 
    void DeclareASet(const DocumentSnapshot& snapshot); 

public:
    static PlayingMyTurn& GetInstance();
	void Handle(const DocumentSnapshot& snapshot);
    void PlayTurn(const DocumentSnapshot& snapshot); 
};

#endif  // LITERATURE_PLAYING_MY_TURN_H  // NOLINT