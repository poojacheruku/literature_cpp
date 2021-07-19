#include "PlayerState.hpp"
#include "LogIt.hpp"

#include <iostream>
#include <string>

using namespace std;

PlayerState::PlayerState() {

}

PlayerState::~PlayerState() {
}

void PlayerState::Handle(const DocumentSnapshot& snapshot)
{
	logIt(logERROR) << "Illegal state transition from " << GetName() << " to Playing";
}

void PlayerState::WaitForPlayers() {
	logIt(logERROR) << "Illegal state transition from " << GetName() << " to Playing";
}

void PlayerState::Start()
{
	logIt(logERROR) << "Illegal state transition from " << GetName() << " to Playing";
}
