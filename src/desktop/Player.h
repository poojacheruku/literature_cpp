#ifndef PLAYER_H
#define PLAYER_H

class PlayerState;

class Player {
public:
	enum State
	{
		ST_STOPPED,
		ST_PLAYING,
		ST_PAUSED
	};

	Player();
	virtual ~Player();

	void Play();
	void Pause();
	void Stop();

	void SetState(State state);

private:
	PlayerState * m_pState;
};

#endif // PLAYER_H