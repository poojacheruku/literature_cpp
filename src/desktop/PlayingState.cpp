#include "PlayingState.h"
#include "PlayerState.h"
#include "Player.h"

#include <iostream>
#include <string>

using namespace std;

PlayingState::PlayingState()
: PlayerState(std::string("Playing")) {
}

PlayingState::~PlayingState() {
}

void PlayingState::Pause(Player * player)
{
	player->SetState(Player::ST_PAUSED);
}

void PlayingState::Stop(Player * player)
{
	player->SetState(Player::ST_STOPPED);
}