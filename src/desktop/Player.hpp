#ifndef LITERATURE_PLAYER_H
#define LITERATURE_PLAYER_H

#include <string>
using namespace std;

#include "firebase/firestore.h"
using firebase::firestore::DocumentSnapshot;

class PlayerState;

class Player {
private:
    /* data */
	PlayerState * m_pState;
	int m_type;
	string m_displayName;
	string m_gameCode;
	vector<string> m_games;
	string m_playerId;
	int m_playerIndex;
	int m_team;

    /* constructor */
    Player();
    Player(Player const&);              // Don't implement
    void operator=(Player const&);    // Don't implement

public:
    static Player& GetInstance();
	void Handle(const DocumentSnapshot& snapshot);
	void WaitForPlayers();
	void Start();
	void SetState(PlayerState * state);
	void SetPlayerType(int type) { m_type = type; }
	void SetDisplayName(string displayName) { m_displayName = displayName; }
	string GetDisplayName() { return m_displayName; }
	void SetPlayerId(string playerId) { m_playerId = playerId; }
	string GetPlayerId() { return m_playerId; }
	void SetTeam(int team) { m_team = team; }
	int GetTeam() { return m_team; }
	void AddGame(string gameCode) { m_games.push_back(gameCode); }
	int GetPlayerType() { return m_type; }
	string GetPlayerName(const DocumentSnapshot& snapshot, string playerTurnId);
	int GetPlayerIndex(const DocumentSnapshot& snapshot, string playerTurnId);
	int GetPlayerIndex() { return m_playerIndex; }
	void SetPlayerIndex(int playerIndex) { m_playerIndex = playerIndex; }
	string GetPlayerNameAndIndex(const DocumentSnapshot& snapshot, string playerTurnId, int& playerIndex);
	void PrintHand(const DocumentSnapshot& snapshot);
	
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

#endif // LITERATURE_PLAYER_H