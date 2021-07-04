#ifndef PLAYER_H
#define PLAYER_H

class PlayerState;

class Player {
private:
    /* data */
	PlayerState * m_pState;
	int m_type;

    /* constructor */
    Player();
    Player(Player const&);              // Don't implement
    void operator=(Player const&);    // Don't implement

public:
    static Player& getInstance();
	void Handle();
	void SetState(PlayerState * state);
	void SetPlayerType(int type) { m_type = type; }

	enum State
	{
		ST_STOPPED_PLAYING,
		ST_WAITING_FOR_PLAYERS,
		ST_WAITING_FOR_TURN,
		ST_PLAYING_MY_TURN
	};

	enum Type
	{
		OWNER,
		PLAYER
	};
};

#endif // PLAYER_H