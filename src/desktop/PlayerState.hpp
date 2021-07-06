#ifndef LITERATURE_PLAYER_STATE_H
#define LITERATURE_PLAYER_STATE_H

#include "Player.hpp"

#include "firebase/firestore.h"
using firebase::firestore::DocumentSnapshot;

#include <string>
using namespace std;

class PlayerState {
protected:
	string  		m_name;
	Player::State 	m_state;

public:
	PlayerState();
	virtual ~PlayerState();
	virtual void Handle(const DocumentSnapshot& snapshot);
	virtual void Start();
	string GetName() { return m_name; }
};

#endif // LITERATURE_PLAYER_STATE_H