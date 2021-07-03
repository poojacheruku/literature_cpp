#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "Player.h"

#include <string>

using namespace std;

class PlayerState {
public:
	PlayerState();
	virtual ~PlayerState();

	virtual void Handle(Player * player);

	string GetName() { return m_name; }

protected:
	string  		m_name;
	Player::State 	state;
};

#endif // PLAYER_STATE_H