#include "PlayerState.h"

#include <iostream>
#include <string>

using namespace std;

PlayerState::PlayerState(std::string name)
: m_name(name) {

}

PlayerState::~PlayerState() {
}

void PlayerState::Play(Player *)
{
	std::cout << "Illegal state transition from " << GetName() << " to Playing\n";
}

void PlayerState::Pause(Player *)
{
	std::cout << "Illegal state transition from " << GetName() << " to Paused\n";
}

void PlayerState::Stop(Player *)
{
	std::cout << "Illegal state transition from " << GetName() << " to Stopped\n";
}