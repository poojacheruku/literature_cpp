#include "Player.hpp"
#include "PlayerState.hpp"
#include "StoppedPlaying.hpp"
#include "LogIt.hpp"

#include <iostream>
#include <future>
#include <thread>

using namespace std;

/* constructor */
Player::Player()
{
}

Player& Player::GetInstance()
{
    static Player instance;       // Gets destroyed at the end
    return instance;
}

void Player::Handle(const DocumentSnapshot& snapshot) {
    log(logINFO) << "Passing the handle to State";
	// m_pState->Handle(snapshot);
    std::thread threadObj(&PlayerState::Handle, m_pState, snapshot);
    threadObj.detach();
}

void Player::WaitForPlayers() {
    log(logINFO) << "WaitForPlayers called";
    m_pState->WaitForPlayers();
}

void Player::Start() {
    SetState(&StoppedPlaying::GetInstance());
	m_pState->Start();
}

void Player::SetState(PlayerState * state)
{
	m_pState = state;
}