#include "Player.hpp"

#include <iostream>

using namespace std;

Player::Player()
// : m_pState(new StoppedState()){
{
}

Player::~Player() {
	// delete m_pState;
}

void Player::Handle() {

}

void Player::SetState(PlayerState * state)
{
	m_pState = state;
}