#include "StoppedState.h"
#include "Player.h"

#include <string>

using namespace std;

StoppedState::StoppedState()
: PlayerState(std::string("Stopped")) {
}

StoppedState::~StoppedState() {
}

void StoppedState::Play(Player * player)
{
	player->SetState(Player::ST_PLAYING);
}