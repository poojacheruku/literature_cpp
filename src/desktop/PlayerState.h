#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "Player.h"

#include <string>

using namespace std;

class PlayerState {
public:
	enum State
	{
		ST_STOPPED,
		ST_PLAYING,
		ST_PAUSED
	};

	PlayerState(string name);
	virtual ~PlayerState();

	virtual void Play(Player * player);
	virtual void Pause(Player * player);
	virtual void Stop(Player * player);

	string GetName() { return m_name; }

private:
	string   m_name;
};

#endif // PLAYER_STATE_H