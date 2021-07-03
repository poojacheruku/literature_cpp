#ifndef PLAYER_H
#define PLAYER_H

class PlayerState;

class Player {
public:
	enum State
	{
		ST_STOPPED_PLAYING,
		ST_WAITING_FOR_PLAYERS,
		ST_WAITING_FOR_TURN,
		ST_PLAYING_MY_TURN
	};

	Player();
	virtual ~Player();

	void Handle();

	void SetState(PlayerState * state);

private:
	PlayerState * m_pState;
};

#endif // PLAYER_H