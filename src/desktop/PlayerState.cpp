#include "PlayerState.hpp"

#include <iostream>
#include <string>

using namespace std;

PlayerState::PlayerState() {

}

PlayerState::~PlayerState() {
}

void PlayerState::Handle(Player *)
{
	std::cout << "Illegal state transition from " << GetName() << " to Playing\n";
}
