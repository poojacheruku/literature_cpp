#include "PausedState.h"

#include <iostream>
#include <string>

using namespace std;

PausedState::PausedState()
: PlayerState(std::string("Paused")) {
}

PausedState::~PausedState() {
}

void PausedState::Play(Player * player)
{
	player->SetState(Player::ST_PLAYING);
}

void PausedState::Stop(Player * player)
{
	player->SetState(Player::ST_STOPPED);
}