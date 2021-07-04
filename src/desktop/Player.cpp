#include "Player.hpp"
#include "PlayerState.hpp"

#include <iostream>

using namespace std;

/* constructor */
Player::Player()
{
}

Player& Player::getInstance()
{
    static Player instance;       // Gets destroyed at the end
    return instance;
}

void Player::Handle() {
	m_pState->Handle();
}

void Player::SetState(PlayerState * state)
{
	m_pState = state;
}