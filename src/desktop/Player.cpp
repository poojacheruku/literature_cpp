#include "Player.h"
#include "StoppedState.h"
#include "PlayingState.h"
#include "PausedState.h"

#include <iostream>

using namespace std;

Player::Player()
: m_pState(new StoppedState()){

}

Player::~Player() {
	delete m_pState;
}

void Player::Play() {
	m_pState->Play(this);
}

void Player::Pause() {
	m_pState->Pause(this);
}

void Player::Stop() {
	m_pState->Stop(this);
}

void Player::SetState(State state)
{
	cout << "changing from " << m_pState->GetName() << " to ";
	delete m_pState;

	if(state == ST_STOPPED)
	{
		m_pState = new StoppedState();
	}
	else if(state == ST_PLAYING)
	{
		m_pState = new PlayingState();
	}
	else
	{
		m_pState = new PausedState();
	}

	cout << m_pState->GetName() << " state\n";
}